
#include <frame.h>
#include <nic.h>

#include <options.h>
#include <log.h>

#include <stdint.h>
#include <string.h>

ec_frame_t * ec_frame_init (uint8_t * buffer)
{
   static ec_frame_t _frame =
   {
      .da = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF },
      .sa = { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
      .etype = { 0x88, 0xA4 },
      .length = -2,
   };
   memcpy (buffer, &_frame, sizeof(ec_frame_t));
   return (ec_frame_t *)buffer;
}

ec_pdu_t * ec_frame_add_pdu (ec_frame_t * frame, int len)
{
   uint16_t offset = frame->length + 2;
   ec_pdu_t * pdu = (ec_pdu_t *)&frame->data[offset];

   /* Set length, account for PDU header and WKC */
   frame->length += sizeof(ec_pdu_t) + len + sizeof(uint16_t);

   /* Clear WKC (immediately after data field) */
   pdu->data[len] = 0;
   pdu->data[len + 1] = 0;

   return pdu;
}

ec_pdu_t * ec_frame_first_pdu (ec_frame_t * frame)
{
   return (ec_pdu_t *)frame->data;
}

ec_pdu_t * ec_frame_next_pdu (ec_frame_t * frame, ec_pdu_t * pdu)
{
   int offset = pdu->len & ~EC_PDU_LENGTH_NEXT;
   int length = frame->length & ~EC_FRAME_LENGTH_DLPDU;
   ec_pdu_t * next = (ec_pdu_t *)&pdu->data[offset + sizeof(uint16_t)];

   if (next > (ec_pdu_t*)&frame->data[length])
   {
      return NULL;
   }

   return next;
}

ec_pdu_t * ec_frame_traverse (ec_frame_t * frame, void * arg,
        int (*fn)(void * arg, ec_pdu_t * pdu, int wkc))
{
   ec_pdu_t * pdu = ec_frame_first_pdu (frame);
   int success;

   while (pdu != NULL)
   {
      int wkc = ec_frame_pdu_get_wkc (pdu);
      success = fn (arg, pdu, wkc);
      if (!success)
      {
         break;
      }
      pdu = ec_frame_next_pdu (frame, pdu);
   }
   return pdu;
}

int ec_frame_pdu_get_wkc (ec_pdu_t * pdu)
{
   int offset = pdu->len & ~EC_PDU_LENGTH_NEXT;
   int wkc;

   wkc  = pdu->data[offset];
   wkc |= (pdu->data[offset + 1] << 8);
   return CC_FROM_LE16 (wkc);
}

ec_pdu_t * ec_frame_APRD (ec_frame_t * frame, uint16_t adp, uint16_t ado,
                          int len)
{
   ec_pdu_t * pdu;

   pdu = ec_frame_add_pdu (frame, len);

   pdu->cmd = EC_CMD_APRD;
   pdu->idx = 0x42;
   pdu->adp = 0 - adp;
   pdu->ado = ado;
   pdu->len = len;
   pdu->irq = 0;

   return pdu;
}

ec_pdu_t * ec_frame_APWR (ec_frame_t * frame, uint16_t adp, uint16_t ado,
                          int len)
{
   ec_pdu_t * pdu;

   pdu = ec_frame_add_pdu (frame, len);

   pdu->cmd = EC_CMD_APWR;
   pdu->idx = 0x42;
   pdu->adp = 0 - adp;
   pdu->ado = ado;
   pdu->len = len;
   pdu->irq = 0;

   return pdu;
}

ec_pdu_t * ec_frame_BRD (ec_frame_t * frame, uint16_t ado, int len)
{
   ec_pdu_t * pdu;

   pdu = ec_frame_add_pdu (frame, len);

   pdu->cmd = EC_CMD_BRD;
   pdu->idx = 0x42;
   pdu->adp = 0;
   pdu->ado = ado;
   pdu->len = len;
   pdu->irq = 0;

   /* Clear data so slaves can set active bits */
   memset (pdu->data, 0, len);

   return pdu;
}

ec_pdu_t * ec_frame_BWR (ec_frame_t * frame, uint16_t ado, int len)
{
   ec_pdu_t * pdu;

   pdu = ec_frame_add_pdu (frame, len);

   pdu->cmd = EC_CMD_BWR;
   pdu->idx = 0x42;
   pdu->adp = 0;
   pdu->ado = ado;
   pdu->len = len;
   pdu->irq = 0;

   return pdu;
}

ec_pdu_t * ec_frame_FPRD (ec_frame_t * frame, uint16_t adp, uint16_t ado,
                          int len)
{
   ec_pdu_t * pdu;

   pdu = ec_frame_add_pdu (frame, len);

   pdu->cmd = EC_CMD_FPRD;
   pdu->idx = 0x42;
   pdu->adp = adp;
   pdu->ado = ado;
   pdu->len = len;
   pdu->irq = 0;

   return pdu;
}

ec_pdu_t * ec_frame_FPWR (ec_frame_t * frame, uint16_t adp, uint16_t ado,
                          int len)
{
   ec_pdu_t * pdu;

   pdu = ec_frame_add_pdu (frame, len);

   pdu->cmd = EC_CMD_FPWR;
   pdu->idx = 0x42;
   pdu->adp = adp;
   pdu->ado = ado;
   pdu->len = len;
   pdu->irq = 0;

   return pdu;
}

void ec_frame_BRD8 (ec_frame_t * frame, uint16_t ado)
{
   ec_frame_BRD (frame, ado, sizeof(uint8_t));
}

void ec_frame_BRD16 (ec_frame_t * frame, uint16_t ado)
{
   ec_frame_BRD (frame, ado, sizeof(uint16_t));
}

void ec_frame_BWR8 (ec_frame_t * frame, uint16_t ado, uint8_t value)
{
   ec_pdu_t * pdu = ec_frame_BWR (frame, ado, sizeof(value));
   memcpy (pdu->data, &value, sizeof(value));
}

void ec_frame_BWR16 (ec_frame_t * frame, uint16_t ado, uint16_t value)
{
   ec_pdu_t * pdu = ec_frame_BWR (frame, ado, sizeof(value));
   value = CC_TO_LE16 (value);
   memcpy (pdu->data, &value, sizeof(value));
}

void ec_frame_BWR32 (ec_frame_t * frame, uint16_t ado, uint32_t value)
{
   ec_pdu_t * pdu = ec_frame_BWR (frame, ado, sizeof(value));
   value = CC_TO_LE32 (value);
   memcpy (pdu->data, &value, sizeof(value));
}

void ec_frame_APWR8 (ec_frame_t * frame, uint16_t adp, uint16_t ado,
                     uint8_t value)
{
   ec_pdu_t * pdu = ec_frame_APWR (frame, adp, ado, sizeof(value));
   memcpy (pdu->data, &value, sizeof(value));
}

void ec_frame_APWR16 (ec_frame_t * frame, uint16_t adp, uint16_t ado,
                      uint16_t value)
{
   ec_pdu_t * pdu = ec_frame_APWR (frame, adp, ado, sizeof(value));
   value = CC_TO_LE16 (value);
   memcpy (pdu->data, &value, sizeof(value));
}

void ec_frame_APWR32 (ec_frame_t * frame, uint16_t adp, uint16_t ado,
                      uint32_t value)
{
   ec_pdu_t * pdu = ec_frame_APWR (frame, adp, ado, sizeof(value));
   value = CC_TO_LE32 (value);
   memcpy (pdu->data, &value, sizeof(value));
}

void ec_frame_FPRD16 (ec_frame_t * frame, uint16_t adp, uint16_t ado)
{
   ec_frame_FPRD (frame, adp, ado, sizeof(uint16_t));
}

void ec_frame_FPRD32 (ec_frame_t * frame, uint16_t adp, uint16_t ado)
{
   ec_frame_FPRD (frame, adp, ado, sizeof(uint32_t));
}

void ec_frame_FPWR8 (ec_frame_t * frame, uint16_t adp, uint16_t ado,
                     uint8_t value)
{
   ec_pdu_t * pdu = ec_frame_FPWR (frame, adp, ado, sizeof(value));
   memcpy (pdu->data, &value, sizeof(value));
}

void ec_frame_FPWR16 (ec_frame_t * frame, uint16_t adp, uint16_t ado,
                      uint16_t value)
{
   ec_pdu_t * pdu = ec_frame_FPWR (frame, adp, ado, sizeof(value));
   value = CC_TO_LE16 (value);
   memcpy (pdu->data, &value, sizeof(value));
}

void ec_frame_FPWR32 (ec_frame_t * frame, uint16_t adp, uint16_t ado,
                      uint32_t value)
{
   ec_pdu_t * pdu = ec_frame_FPWR (frame, adp, ado, sizeof(value));
   value = CC_TO_LE32 (value);
   memcpy (pdu->data, &value, sizeof(value));
}

int ec_frame_txrx (ec_nic_t * nic, ec_frame_t * frame)
{
   int length = sizeof(ec_frame_t) + frame->length + sizeof(uint16_t);
   int result;
   ec_pdu_t * pdu;
   ec_pdu_t * previous;
   uint16_t wkc;

   pdu = ec_frame_first_pdu (frame);
   CC_ASSERT (pdu != NULL);

   /* Set NEXT field of PDU:s */
   while (pdu != NULL)
   {
      pdu->len |= EC_PDU_LENGTH_NEXT;
      previous = pdu;
      pdu = ec_frame_next_pdu (frame, pdu);
   }

   /* Clear NEXT field of final PDU */
   previous->len &= ~EC_PDU_LENGTH_NEXT;

   /* Set DLPDU flag */
   frame->length |= EC_FRAME_LENGTH_DLPDU;

   /* Transmit frame */
   result = ec_nic_send (nic, frame, length);
   if (result < 0)
   {
      return result;
   }

   /* Receive frame */
   int rxsize;
   do
   {
      rxsize = ec_nic_receive (nic, frame, length);
   } while (rxsize == 0 || frame->etype[0] != 0x88 || frame->etype[1] != 0xa4);

#if 0
   LOG_DEBUG (EC_NIC_DEBUG, "Got %d bytes\n", rxsize);
#endif

   /* Retrieve wkc of last PDU */
   frame->length &= ~EC_FRAME_LENGTH_DLPDU;
   wkc  = frame->data[frame->length];
   wkc |= (frame->data[frame->length + 1] << 8);

   return wkc;
}
