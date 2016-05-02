
#ifndef EEPROM_H
#define EEPROM_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "ec_api.h"
#include "ec_types.h"

int ec_eeprom_broadcast_read (ec_net_t * net, uint16_t address, uint32_t offset);
int ec_eeprom_read (ec_net_t * net, ec_slave_t * slave, int address, void * data, size_t size);

#ifdef __cplusplus
}
#endif

#endif /* EEPROM_H */
