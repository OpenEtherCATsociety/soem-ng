
#include <frame.h>
#include <alloc.h>

#include <options.h>

#include <gtest/gtest.h>

TEST (ec_frame, frame)
{
   uint8_t buffer[100];
   ec_frame_t * frame;
   ec_pdu_t * pdu;

   // Check that frame offsets are correct. Alignment exceptions
   // will be thrown if implementation is incorrect.

   frame = ec_frame_init (buffer);
   ec_frame_BWR8 (frame, 1, 0);
   ec_frame_BWR16 (frame, 2, 0); // Should start at odd offset

   pdu = ec_frame_first_pdu (frame);
   int offset = (uint32_t)pdu - (uint32_t)frame;
   ASSERT_EQ (0, offset % 2);

   pdu = ec_frame_next_pdu (frame, pdu);
   offset = (uint32_t)pdu - (uint32_t)frame;
   ASSERT_EQ (1, offset % 2);
}

TEST (ec_frame, BWRx)
{
   uint8_t buffer[100];
   ec_frame_t * frame;
   ec_pdu_t * pdu;

   frame = ec_frame_init (buffer);
   ec_frame_BWR8 (frame, 1, 0);
   ec_frame_BWR16 (frame, 2, 0);
   ec_frame_BWR32 (frame, 3, 0);

   pdu = ec_frame_first_pdu (frame);
   ASSERT_EQ (EC_CMD_BWR, pdu->cmd);
   ASSERT_EQ (sizeof(uint8_t), pdu->len);

   pdu = ec_frame_next_pdu (frame, pdu);
   ASSERT_EQ (EC_CMD_BWR, pdu->cmd);
   ASSERT_EQ (sizeof(uint16_t), pdu->len);

   pdu = ec_frame_next_pdu (frame, pdu);
   ASSERT_EQ (EC_CMD_BWR, pdu->cmd);
   ASSERT_EQ (sizeof(uint32_t), pdu->len);
}
