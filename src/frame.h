
#ifndef FRAME_H
#define FRAME_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "ec_api.h"
#include "ec_types.h"

CC_PACKED_BEGIN
typedef struct CC_PACKED ec_frame
{
   uint8_t  da[6];              /** Destination MAC */
   uint8_t  sa[6];              /** Source MAC */
   uint8_t  etype[2];           /** Ethernet type */
   uint16_t length;             /** length of frame (minus 2 bytes) */
   uint8_t  data[];
} ec_frame_t;
CC_PACKED_END

#define EC_FRAME_LENGTH_DLPDU     CC_BIT (12)

/* CC_STATIC_ASSERT (sizeof (ec_frame_t) == 0x10); */

CC_PACKED_BEGIN
typedef struct CC_PACKED ec_pdu
{
   uint8_t  cmd;                /** EtherCAT cmd */
   uint8_t  idx;                /** Master identifier */
   uint16_t adp;                /** auto-increment address */
   uint16_t ado;                /** physical memory or register address */
   uint16_t len;                /** length of data field */
   uint16_t irq;                /** external event */
   uint8_t  data[];             /**  */
} ec_pdu_t;
CC_PACKED_END

/* CC_STATIC_ASSERT (sizeof (ec_pdu_t) == 0x0A); */

#define EC_PDU_LENGTH_NEXT      CC_BIT (15)

CC_PACKED_BEGIN
typedef struct CC_PACKED ec_mbox
{
   uint16_t length;             /** length of data field */
   uint16_t address;            /** station address */
   uint8_t  priority;           /** mailbox priority (0 - lowest, 3 - highest)*/
   uint8_t  type;               /** mailbox type */
   uint8_t  data[];             /**  */
} ec_mbox_t;
CC_PACKED_END

/* CC_STATIC_ASSERT (sizeof (ec_mbox_t) == 0x06); */

/** Ethercat command types */
typedef enum
{
   EC_CMD_NOP = 0x00,           /** No operation */
   EC_CMD_APRD,                 /** Auto Increment Read */
   EC_CMD_APWR,                 /** Auto Increment Write */
   EC_CMD_APRW,                 /** Auto Increment Read Write */
   EC_CMD_FPRD,                 /** Configured Address Read */
   EC_CMD_FPWR,                 /** Configured Address Write */
   EC_CMD_FPRW,                 /** Configured Address Read Write */
   EC_CMD_BRD,                  /** Broadcast Read */
   EC_CMD_BWR,                  /** Broadcast Write */
   EC_CMD_BRW,                  /** Broadcast Read Write */
   EC_CMD_LRD,                  /** Logical Memory Read */
   EC_CMD_LWR,                  /** Logical Memory Write */
   EC_CMD_LRW,                  /** Logical Memory Read Write */
   EC_CMD_ARMW,                 /** Auto Increment Read Mulitple Write */
   EC_CMD_FRMW                  /** Configured Read Multiple Write */
} ec_cmd_t;

/* CC_STATIC_ASSERT (EC_CMD_FRMW == 0x0E); */

/** Ethercat registers */
typedef enum
{
   EC_REG_TYPE        = 0x0000,
   EC_REG_PORTDES     = 0x0007,
   EC_REG_ESCSUP      = 0x0008,
   EC_REG_STADR       = 0x0010,
   EC_REG_ALIAS       = 0x0012,
   EC_REG_DLCTL       = 0x0100,
   EC_REG_DLPORT      = 0x0101,
   EC_REG_DLALIAS     = 0x0103,
   EC_REG_DLSTAT      = 0x0110,
   EC_REG_ALCTL       = 0x0120,
   EC_REG_ALSTAT      = 0x0130,
   EC_REG_ALSTATCODE  = 0x0134,
   EC_REG_PDICTL      = 0x0140,
   EC_REG_IRQMASK     = 0x0200,
   EC_REG_RXERR       = 0x0300,
   EC_REG_FRXERR      = 0x0308,
   EC_REG_EPUECNT     = 0x030C,
   EC_REG_PECNT       = 0x030D,
   EC_REG_PECODE      = 0x030E,
   EC_REG_LLCNT       = 0x0310,
   EC_REG_WDCNT       = 0x0442,
   EC_REG_EEPCFG      = 0x0500,
   EC_REG_EEPCTL      = 0x0502,
   EC_REG_EEPSTAT     = 0x0502,
   EC_REG_EEPADR      = 0x0504,
   EC_REG_EEPDAT      = 0x0508,
   EC_REG_FMMU0       = 0x0600,
   EC_REG_FMMU1       = EC_REG_FMMU0 + 0x10,
   EC_REG_FMMU2       = EC_REG_FMMU1 + 0x10,
   EC_REG_FMMU3       = EC_REG_FMMU2 + 0x10,
   EC_REG_SM0         = 0x0800,
   EC_REG_SM1         = EC_REG_SM0 + 0x08,
   EC_REG_SM2         = EC_REG_SM1 + 0x08,
   EC_REG_SM3         = EC_REG_SM2 + 0x08,
   EC_REG_SM0STAT     = EC_REG_SM0 + 0x05,
   EC_REG_SM1STAT     = EC_REG_SM1 + 0x05,
   EC_REG_SM1ACT      = EC_REG_SM1 + 0x06,
   EC_REG_SM1CONTR    = EC_REG_SM1 + 0x07,
   EC_REG_DCTIME0     = 0x0900,
   EC_REG_DCTIME1     = 0x0904,
   EC_REG_DCTIME2     = 0x0908,
   EC_REG_DCTIME3     = 0x090C,
   EC_REG_DCSYSTIME   = 0x0910,
   EC_REG_DCSOF       = 0x0918,
   EC_REG_DCSYSOFFSET = 0x0920,
   EC_REG_DCSYSDELAY  = 0x0928,
   EC_REG_DCSYSDIFF   = 0x092C,
   EC_REG_DCSPEEDCNT  = 0x0930,
   EC_REG_DCTIMEFILT  = 0x0934,
   EC_REG_DCCUC       = 0x0980,
   EC_REG_DCSYNCACT   = 0x0981,
   EC_REG_DCSTART0    = 0x0990,
   EC_REG_DCCYCLE0    = 0x09A0,
   EC_REG_DCCYCLE1    = 0x09A4
} ec_reg_t;

ec_frame_t * ec_frame_init (uint8_t * buffer);
ec_pdu_t * ec_frame_add_pdu (ec_frame_t * frame, int len);

ec_pdu_t * ec_frame_first_pdu (ec_frame_t * frame);
ec_pdu_t * ec_frame_next_pdu (ec_frame_t * frame, ec_pdu_t * pdu);
int ec_frame_pdu_get_wkc (ec_pdu_t * pdu);
ec_pdu_t * ec_frame_traverse (ec_frame_t * frame, void * arg,
        int (*fn)(void * arg, ec_pdu_t * pdu, int wkc));

ec_pdu_t * ec_frame_APRD (ec_frame_t * frame, uint16_t adp, uint16_t ado,
                          int len);
ec_pdu_t * ec_frame_BWR (ec_frame_t * frame, uint16_t ado, int len);

void ec_frame_BRD8 (ec_frame_t * frame, uint16_t ado);
void ec_frame_BRD16 (ec_frame_t * frame, uint16_t ado);

void ec_frame_BWR8 (ec_frame_t * frame, uint16_t ado, uint8_t value);
void ec_frame_BWR16 (ec_frame_t * frame, uint16_t ado, uint16_t value);
void ec_frame_BWR32 (ec_frame_t * frame, uint16_t ado, uint32_t value);

void ec_frame_APWR8 (ec_frame_t * frame, uint16_t adp, uint16_t ado,
                     uint8_t value);
void ec_frame_APWR16 (ec_frame_t * frame, uint16_t adp, uint16_t ado,
                      uint16_t value);
void ec_frame_APWR32 (ec_frame_t * frame, uint16_t adp, uint16_t ado,
                      uint32_t value);

void ec_frame_FPRD16 (ec_frame_t * frame, uint16_t adp, uint16_t ado);
void ec_frame_FPRD32 (ec_frame_t * frame, uint16_t adp, uint16_t ado);

ec_pdu_t * ec_frame_FPWR (ec_frame_t * frame, uint16_t adp, uint16_t ado,
                          int len);
void ec_frame_FPWR8 (ec_frame_t * frame, uint16_t adp, uint16_t ado,
                     uint8_t value);
void ec_frame_FPWR16 (ec_frame_t * frame, uint16_t adp, uint16_t ado,
                      uint16_t value);
void ec_frame_FPWR32 (ec_frame_t * frame, uint16_t adp, uint16_t ado,
                      uint32_t value);

#include "nic.h"
int ec_frame_txrx (ec_nic_t * nic, ec_frame_t * frame);

#ifdef __cplusplus
}
#endif

#endif /* FRAME_H */
