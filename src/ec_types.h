
#ifndef TYPES_H
#define TYPES_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stddef.h>

#include "cc.h"
#include "osal.h"

typedef struct ec_general
{
   uint8_t groupIdx;
   uint8_t imgIdx;
   uint8_t orderIdx;
   uint8_t nameIdx;
   uint8_t reserved1;
   uint8_t coeDetails;
   uint8_t foeDetails;
   uint8_t eoeDetails;
   uint8_t reserved2[3];
   uint8_t flags;
   uint16_t currentOnEbus;
   uint8_t reserved3[2];
   uint16_t physicalPort;
   uint8_t reserved4[14];
} ec_general_t;

typedef struct ec_slave
{
   struct ec_slave * next;
   uint16_t address;
   uint32_t vendor;
   uint32_t product;
   uint32_t revision;
   uint32_t serial;
   uint32_t rxmbox;
   uint32_t txmbox;
   uint32_t mboxproto;
   uint32_t offset;
   uint16_t eeprom_offset;
   ec_sm_t * sm[4];
} ec_slave_t;

typedef struct ec_net
{
   ec_nic_t * nic;
   int nslaves;
   ec_slave_t * head;
   ec_slave_t * tail;
} ec_net_t;

#ifdef __cplusplus
}
#endif

#endif /* TYPES_H */
