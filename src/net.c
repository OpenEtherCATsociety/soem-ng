
#include "net.h"
#include "nic.h"

#include "options.h"
#include "version.h"
#include "log.h"
#include "osal.h"

#include "frame.h"
#include "eeprom.h"
#include <string.h>
#include "alloc.h"

static void ec_net_add_slave (ec_net_t * net, ec_slave_t * slave)
{
   if (net->head == NULL)
   {
      net->head = slave;
      net->tail = slave;
   }
   else
   {
      net->tail->next = slave;
      net->tail = slave;
   }
}

ec_slave_t * ec_net_get_slave (ec_net_t * net, int ix)
{
   ec_slave_t * slave;

   for (slave = net->head; ix > 0 && slave != NULL; slave = slave->next)
   {
      ix--;
   }

   return slave;
}

ec_net_t * ec_net_init (const char * ifname)
{
   ec_net_t * net;
   uint8_t buffer[1400];
   ec_slave_t * slave;
   int ix;
   int wkc;
   ec_frame_t * frame;

   LOG_DEBUG (EC_NET_DEBUG, "Version %d.%d\n", SOEM_VERSION_MAJOR, SOEM_VERSION_MINOR);
   LOG_DEBUG (EC_NET_DEBUG, "Using %s\n", ifname);

   /* Allocate the net */
   net = ec_alloc_net();
   if (net == NULL)
      return NULL;

   /* Initialise NIC */
   net->nic = ec_nic_init (ifname);
   if (net->nic == NULL)
   {
      LOG_ERROR (EC_NET_DEBUG, "Unable to open %s", ifname);
      return NULL;
   }

   /* Set INIT state */
   frame = ec_frame_init (buffer);
   ec_frame_BWR8 (frame, EC_REG_ALCTL, 0x11);
   ec_frame_BRD8 (frame, EC_REG_ALCTL);
   wkc = ec_frame_txrx (net->nic, frame);
   if (wkc > 0)
   {
      net->nslaves = wkc;
   }

   LOG_DEBUG (EC_NET_DEBUG, "Found %d slaves\n", net->nslaves);

   /* Initialise slave states */
   /* TODO: clear fmmu / sm settings? */
   frame = ec_frame_init (buffer);
   ec_frame_BWR8 (frame, EC_REG_DLPORT, 0);
   ec_frame_BWR16 (frame, EC_REG_IRQMASK, 0);
   ec_frame_BWR8 (frame, EC_REG_RXERR, 0);
   ec_frame_BWR32 (frame, EC_REG_DCSYSTIME, 0);
   ec_frame_BWR16 (frame, EC_REG_DCSPEEDCNT, 0);
   ec_frame_BWR16 (frame, EC_REG_DCTIMEFILT, 0);
   ec_frame_BWR16 (frame, EC_REG_DLALIAS, 0);
   ec_frame_BWR8 (frame, EC_REG_EEPCFG, 0x02);
   ec_frame_BWR8 (frame, EC_REG_EEPCFG, 0x00);

   wkc = ec_frame_txrx (net->nic, frame);
   if (wkc != net->nslaves)
   {
      LOG_ERROR (EC_NET_DEBUG, "Could not reset all slaves (wkc = %d)\n", wkc);
   }

   /* Set addresses */

   frame = ec_frame_init (buffer);
   for (ix = 0; ix < net->nslaves; ix++)
   {
      slave = ec_alloc_slave();
      ec_net_add_slave (net, slave);

      slave->address = 0x1000 + ix;
      ec_frame_APWR16 (frame, ix, EC_REG_STADR, slave->address);
   }

   wkc = ec_frame_txrx (net->nic, frame);
   if (wkc != 1)
   {
      LOG_ERROR (EC_NET_DEBUG, "Could not set address (wkc = %d)\n", wkc);
   }

   /* Read slave info */
   ec_eeprom_broadcast_read (net, 16, offsetof (ec_slave_t, vendor));
   ec_eeprom_broadcast_read (net, 20, offsetof (ec_slave_t, product));
   ec_eeprom_broadcast_read (net, 24, offsetof (ec_slave_t, revision));
   ec_eeprom_broadcast_read (net, 28, offsetof (ec_slave_t, serial));
   ec_eeprom_broadcast_read (net, 48, offsetof (ec_slave_t, rxmbox));
   ec_eeprom_broadcast_read (net, 52, offsetof (ec_slave_t, txmbox));
   ec_eeprom_broadcast_read (net, 56, offsetof (ec_slave_t, mboxproto));

   return net;
}
