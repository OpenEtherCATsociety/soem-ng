
#include <nic.h>
#include <options.h>
#include <log.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define HAVE_REMOTE
#include <pcap.h>

typedef struct ec_nic
{
   pcap_t * handle;
} ec_nic_t;

void ec_nic_show_adapters (void)
{
   pcap_if_t * devs;
   pcap_if_t * dev;
   char errbuf[PCAP_ERRBUF_SIZE];

   /* find all devices */
   if (pcap_findalldevs (&devs, errbuf) == -1)
   {
      LOG_ERROR (EC_NIC_DEBUG, "Error finding adapters: %s\n", errbuf);
      return;
   }

   dev = devs;
   while (dev != NULL)
   {
      printf ("%s %s\n",
              dev->name,
              (dev->description) ? dev->description : "No description available");
      dev = dev->next;
   }

   pcap_freealldevs (devs);
}

int ec_nic_send (ec_nic_t * nic, const void * buffer, size_t size)
{
   int result;
   uint8_t * p;

   p = (uint8_t *)buffer;
   size = size;
   result = pcap_sendpacket (nic->handle, p, size);
   if (result < 0)
   {
      LOG_WARNING (EC_NIC_DEBUG, "Error sending packet: %s\n",
                   pcap_geterr (nic->handle));
      return -1;
   }

   return size;
}

int ec_nic_receive (ec_nic_t * nic, void * buffer, size_t size)
{
   int result;
   struct pcap_pkthdr * header;
   const uint8_t * data;

   result = pcap_next_ex (nic->handle, &header, &data);
   if (result < 0)
   {
      LOG_WARNING (EC_NIC_DEBUG, "Error receiving packet: %s\n",
                   pcap_geterr (nic->handle));
      return -1;
   }

   if (result == 1)
   {
      size = (size < header->len) ? size : header->len;
      memcpy (buffer, data, size);
   }
   else
   {
      size = 0;
   }

   return size;
}

ec_nic_t * ec_nic_init (const char * ifname)
{
   static ec_nic_t nic;
   char errbuf[PCAP_ERRBUF_SIZE];

   LOG_DEBUG (EC_NIC_DEBUG, "%s\n", pcap_lib_version());

   nic.handle = pcap_open (ifname, 65536, /* PCAP_OPENFLAG_PROMISCUOUS | */
                           PCAP_OPENFLAG_MAX_RESPONSIVENESS |
                           PCAP_OPENFLAG_NOCAPTURE_LOCAL, -1, NULL, errbuf);
   if (nic.handle == NULL)
   {
      LOG_ERROR (EC_NIC_DEBUG, "Unable to open adapters %s\n", ifname);
      return NULL;
   }

   return &nic;
}
