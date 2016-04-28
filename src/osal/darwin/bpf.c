
#include "options.h"
#include "log.h"
#include <net/if.h>
#include <net/bpf.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

typedef struct ec_nic
{
    int handle;
    int rx_buf_size;
} ec_nic_t;

void ec_nic_show_adapters (void)
{
    struct if_nameindex *ids;
    int ix;
    
    ids = if_nameindex();
    for (ix = 0; ids[ix].if_index != 0; ix++)
    {
        printf ("%s\n", ids[ix].if_name);
    }
    
    if_freenameindex (ids);
}

int ec_nic_send (ec_nic_t * nic, const void * buffer, size_t size)
{
    ssize_t result;
    
    result = write (nic->handle, buffer, size);
    if (result < 0)
    {
        LOG_ERROR (EC_NIC_DEBUG, "ec_nic_send(): %s\n", strerror(errno));
        return -1;
    }
    if (result != size) {
        LOG_ERROR (EC_NIC_DEBUG, "Did not send all bytes? (%d of %d)\n", result, size);
    }
    
    return (int)size;
}

int ec_nic_receive (ec_nic_t * nic, void * buffer, size_t size)
{
    ssize_t bytesrx;
    char* buf = alloca(nic->rx_buf_size);
    struct bpf_hdr* bpf_hdr = (struct bpf_hdr*)buf;
    
    if (ioctl(nic->handle, FIONREAD, &bytesrx) < 0) {
        LOG_ERROR (EC_NIC_DEBUG, "ec_nic_receive(): FIONREAD: %s\n", strerror(errno));
        return 0;
    }
    
    if (bytesrx <= 0) {
        return 0;
    }
    
    /* TODO: can read() receive more than one packet frame? 
     Perhaps not since EtherCAT is a master-slave protocol
     and a receive filter is setup at initialization. */
    bytesrx = read(nic->handle, buf, nic->rx_buf_size);
    if (bytesrx == -1) {
        LOG_ERROR (EC_NIC_DEBUG, "ec_nic_receive(): %s\n", strerror(errno));
        return 0;
    }
    
    if (bytesrx > bpf_hdr->bh_hdrlen) {
        bytesrx -= bpf_hdr->bh_hdrlen;
        if (bytesrx > size) {
            bytesrx = size;
        }
        memcpy(buffer, &buf[bpf_hdr->bh_hdrlen], bytesrx);
    }
    else {
        bytesrx = 0;
    }
    
    return (int)bytesrx;
}

ec_nic_t * ec_nic_init (const char * ifname)
{
    static ec_nic_t nic;

    struct ifreq bound_if;
    char buf[BUFSIZ];
    int i;
    
    /* Find and open BPF device */
    for (i = 0; i < 99; i++) {
        sprintf(buf, "/dev/bpf%i", i);
        nic.handle = open(buf, O_RDWR);
        if (nic.handle != -1) {
            break;
        }
    }
    if (i == 99) {
        LOG_ERROR (EC_NIC_DEBUG, "Unable to open adapter %s\n", ifname);
        return NULL;
    }
    
    /* Associate bpf device with a physical ethernet interface */
    strcpy(bound_if.ifr_name, ifname);
    if (ioctl(nic.handle, BIOCSETIF, &bound_if) > 0) {
        LOG_ERROR (EC_NIC_DEBUG, "BIOCSETIF: %s", strerror(errno));
        return NULL;
    }
    LOG_INFO(EC_NIC_DEBUG, "Bound bpf device %s to physical device %s\n", buf, ifname);
    
    /* Activate immediate mode. In this mode, packets are delivered as they
     arrive, rather than being buffered for efficiency. */
    i = 1;
    if (ioctl(nic.handle, BIOCIMMEDIATE, &i) == -1 ) {
        LOG_ERROR (EC_NIC_DEBUG, "BIOCIMMEDIATE: %s", strerror(errno));
        return NULL;
    }
    
    /* Sets or gets the flag determining whether locally generated packets on the
     interface should be returned by BPF.  Set to zero to see only incoming packets on the
     interface.  Set to one to see packets originating locally and remotely on the interface.
     This flag is initialized to one by default. */
    i = 0;
    if (ioctl(nic.handle, BIOCSSEESENT, &i) == -1 ) {
        LOG_ERROR (EC_NIC_DEBUG, "BIOCSSEESENT: %s", strerror(errno));
        return NULL;
    }
    
    /* Request buffer length. When reading this length MUST be used. */
    if (ioctl(nic.handle, BIOCGBLEN, &nic.rx_buf_size) == -1 ) {
        LOG_ERROR (EC_NIC_DEBUG, "BIOCGBLEN: %s", strerror(errno));
        return NULL;
    }
    
    /* Create receive filter to filter out everything except EtherCAT
     packages. tcpdump can be used to create filters.
     See 'man 4 bpf' for details on how to create a filter.
     
     tcpdump -i en0 ether proto 0x88A4 -d
     (000) ldh      [12]
     (001) jeq      #0x88a4  jt 2  jf 3
     (002) ret      #262144
     (003) ret      #0
     
     tcpdump -i en0 ether proto 0x88A4 -dd
     { 0x28, 0, 0, 0x0000000c },
     { 0x15, 0, 1, 0x000088a4 },
     { 0x6,  0, 0, 0x00040000 },
     { 0x6,  0, 0, 0x00000000 },
     */
    
    struct bpf_program bpf_program;
    struct bpf_insn bpf_instructions[] = {
        { 0x28, 0, 0, 0x0000000c },
        { 0x15, 0, 1, 0x000088a4 },
        { 0x6,  0, 0, 0x00040000 },
        { 0x6,  0, 0, 0x00000000 },
    };
    
    bpf_program.bf_len = sizeof(bpf_instructions)/sizeof(struct bpf_insn);
    bpf_program.bf_insns = bpf_instructions;
    if (ioctl(nic.handle, BIOCSETF, &bpf_program) < 0)
    {
        LOG_ERROR (EC_NIC_DEBUG, "BIOCSETF: %s", strerror(errno));
        return NULL;
    }
    
    /* Flushes the buffer of incoming packets, and resets the statistics */
    i = 1;
    if (ioctl(nic.handle, BIOCFLUSH, &i) == -1 ) {
        LOG_ERROR (EC_NIC_DEBUG, "BIOCFLUSH: %s", strerror(errno));
        return NULL;
    }
    
    /* Forces the interface into promiscuous mode.  All packets, not just those destined for
     the local host, are processed. */
    i = 1;
    if (ioctl(nic.handle, BIOCPROMISC, &i) == -1 ) {
        LOG_ERROR (EC_NIC_DEBUG, "BIOCPROMISC: %s", strerror(errno));
        return NULL;
    }
    
    return &nic;
}

