
#include "config.h"

#include "options.h"
#include "log.h"

#include "frame.h"
#include <string.h>

int ec_config_static (ec_net_t * net, ec_slave_config_t * cfg)
{
   uint8_t buffer[1400];
   ec_frame_t * frame;
   ec_pdu_t * pdu;
   int wkc;
   ec_slave_t * slave;

   /* Set sm */

   frame = ec_frame_init (buffer);
   for (slave = net->head; slave != NULL; slave = slave->next)
   {
      pdu = ec_frame_FPWR (frame, slave->address, EC_REG_SM0, sizeof(cfg->sm));
      memcpy (pdu->data, cfg->sm, sizeof(cfg->sm));
      cfg++;
   }

   wkc = ec_frame_txrx (net->nic, frame);
   if (wkc != 1)
   {
      LOG_ERROR (EC_NET_DEBUG, "Could not set std mbox (wkc = %d)\n", wkc);
   }

   /* Set PREOP state */
   frame = ec_frame_init (buffer);
   ec_frame_BWR8 (frame, EC_REG_ALCTL, 0x12);
   wkc = ec_frame_txrx (net->nic, frame);
   if (wkc != net->nslaves)
   {
      LOG_ERROR (EC_NET_DEBUG, "Could not set PREOP state (wkc = %d)\n", wkc);
   }

   /* Check PREOP state */
   frame = ec_frame_init (buffer);
   ec_frame_BRD8 (frame, EC_REG_ALCTL);
   wkc = ec_frame_txrx (net->nic, frame);
   if (wkc != net->nslaves)
   {
      LOG_ERROR (EC_NET_DEBUG, "Could not check state (wkc = %d)\n", wkc);
   }

   return 0;
}
