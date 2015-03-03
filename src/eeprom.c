
#include <eeprom.h>
#include <ec_types.h>
#include <frame.h>

#include <options.h>
#include <log.h>

#include <string.h>
#include <stdio.h>
#include <ctype.h>

static int wkc_should_be_one (void * arg, ec_pdu_t * pdu, int wkc)
{
   return wkc == 1;
}

static int set_value_in_slave (void * arg, ec_pdu_t * pdu, int wkc)
{
   ec_net_t * net = (ec_net_t *)arg;
   int slaveIx = pdu->adp - 0x1000;
   ec_slave_t * slave = &net->head[slaveIx]; /* FIXME for malloc */
   uint8_t * pSrc = pdu->data;
   uint32_t * pDst = (uint32_t *)slave + slave->offset / sizeof(uint32_t);

   *pDst =
      (pSrc[0] << 0)  |
      (pSrc[1] << 8)  |
      (pSrc[2] << 16) |
      (pSrc[3] << 24);

   return 1;
}

int ec_eeprom_broadcast_read (ec_net_t * net, uint16_t address, uint32_t offset)
{
   uint8_t buffer[1400];
   ec_frame_t * frame;
   int wkc;
   ec_slave_t * slave;

   /* Broadcast read of EEPROM address */
   frame = ec_frame_init (buffer);
   ec_frame_BWR16 (frame, EC_REG_EEPCTL, 0);
   ec_frame_BWR16 (frame, EC_REG_EEPADR, address / 2);
   ec_frame_BWR16 (frame, EC_REG_EEPCTL, (1 << 8));

   wkc = ec_frame_txrx (net->nic, frame);
   if (wkc != net->nslaves)
   {
      LOG_ERROR (EC_EEPROM_DEBUG, "BRW failed");
      return -1;
   }

   /* Wait until no slave is busy */
   int is_busy;
   do
   {
      frame = ec_frame_init (buffer);
      ec_frame_BRD16 (frame, EC_REG_EEPSTAT);

      wkc = ec_frame_txrx (net->nic, frame);
      if (wkc != net->nslaves)
      {
         LOG_ERROR (EC_EEPROM_DEBUG, "BRD failed");
         return -1;
      }

      ec_pdu_t * pdu = ec_frame_first_pdu (frame);
      is_busy = pdu->data[1] & CC_BIT (7);
   } while (is_busy);

   /* Read EEPROM data */
   frame = ec_frame_init (buffer);
   for (slave = net->head; slave != NULL; slave = slave->next)
   {
      ec_frame_FPRD32 (frame, slave->address, EC_REG_EEPDAT);
      slave->offset = offset;
   }

   wkc = ec_frame_txrx (net->nic, frame);
   ec_pdu_t * bad = ec_frame_traverse (frame, net, wkc_should_be_one);
   if (bad != NULL)
   {
      LOG_ERROR (EC_EEPROM_DEBUG,
                 "Could not read eeprom of slave %x", bad->adp);
      return -1;
   }

   /* Set value in each slave */
   ec_frame_traverse (frame, net, set_value_in_slave);

   return 0;
}

static int ec_eeprom_address (ec_net_t * net, ec_slave_t * slave, int address)
{
   uint8_t buffer[1400];
   ec_frame_t * frame;
   int wkc;
   int ado = slave->address;

   frame = ec_frame_init (buffer);
   ec_frame_FPWR16 (frame, ado, EC_REG_EEPCTL, 0);
   ec_frame_FPWR16 (frame, ado, EC_REG_EEPADR, address / 2);
   ec_frame_FPWR16 (frame, ado, EC_REG_EEPCTL, (1 << 8));

   wkc = ec_frame_txrx (net->nic, frame);
   if (wkc != 1)
   {
      LOG_ERROR (EC_EEPROM_DEBUG, "Failed to set address");
      return -1;
   }

   return 0;
}

static int ec_eeprom_is_busy (ec_net_t * net, ec_slave_t * slave)
{
   uint8_t buffer[1400];
   ec_frame_t * frame;
   int wkc;
   int ado = slave->address;
   ec_pdu_t * pdu;
   int is_busy;

   frame = ec_frame_init (buffer);
   ec_frame_FPRD16 (frame, ado, EC_REG_EEPSTAT);

   wkc = ec_frame_txrx (net->nic, frame);
   if (wkc != 1)
   {
      LOG_ERROR (EC_EEPROM_DEBUG, "FPRD failed");
      return -1;
   }

   pdu = ec_frame_first_pdu (frame);
   is_busy = (pdu->data[1] & CC_BIT (7)) ? 1 : 0;
   return is_busy;
}

static int ec_eeprom_read32 (ec_net_t * net, ec_slave_t * slave, int address,
                             void * data, size_t size)
{
   uint8_t buffer[1400];
   ec_frame_t * frame;
   int wkc;
   int ado = slave->address;
   ec_pdu_t * pdu;
   int is_busy;
   int result;

   result = ec_eeprom_address (net, slave, address);
   if (result < 0)
   {
      return -1;
   }

   /* Wait while slave is busy */
   do
   {
      is_busy = ec_eeprom_is_busy (net, slave);
      if (is_busy < 0)
      {
         return -1;
      }
   } while (is_busy);

   /* Read EEPROM data */
   frame = ec_frame_init (buffer);
   ec_frame_FPRD32 (frame, ado, EC_REG_EEPDAT);

   wkc = ec_frame_txrx (net->nic, frame);
   if (wkc != 1)
   {
      LOG_ERROR (EC_EEPROM_DEBUG, "Could not read eeprom");
      return -1;
   }

   pdu = ec_frame_first_pdu (frame);
   size = (size > 4) ? 4 : size;
   memcpy (data, pdu->data, size);

   return size;
}

int ec_eeprom_read (ec_net_t * net, ec_slave_t * slave, int address,
                    void * data, size_t size)
{
   int length;
   int offset = 0;
   uint8_t * p = (uint8_t *)data;

   while (size > 0)
   {
      length = ec_eeprom_read32 (net, slave, address, &p[offset], size);
      if (length < 0)
      {
         return length;
      }

      address += length;
      offset += length;
      size -= length;
   }

   return offset;
}

int ec_eeprom_dump (ec_net_t * net, ec_slave_t * slave)
{
   int address;
   int offset;
   uint8_t data[16];

   for (address = 0; address < 512; address += 16)
   {
      ec_eeprom_read (net, slave, address + 0, &data[0], 4);
      ec_eeprom_read (net, slave, address + 4, &data[4], 4);
      ec_eeprom_read (net, slave, address + 8, &data[8], 4);
      ec_eeprom_read (net, slave, address + 12, &data[12], 4);

      printf ("0x%04x: ", address);
      for (offset = 0; offset < 16; offset++)
      {
         printf ("%02x ", data[offset]);
      }
      printf (" |");
      for (offset = 0; offset < 16; offset++)
      {
         int c = data[offset];
         printf ("%c", isprint (c) ? c : '.');
      }
      printf ("|\n");
   }

   return 0;
}
