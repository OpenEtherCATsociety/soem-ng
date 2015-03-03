
#ifndef EC_API_H
#define EC_API_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stddef.h>
#include <ec_export.h>

typedef struct ec_net ec_net_t;
typedef struct ec_slave ec_slave_t;
typedef struct ec_nic ec_nic_t;

typedef struct ec_fmmu
{
   uint16_t address;
   uint16_t size;
   uint8_t type;
} ec_fmmu_t;

typedef struct ec_sm
{
   uint16_t address;
   uint16_t length;
   uint8_t control;
   uint8_t status;
   uint8_t enable;
   uint8_t type;
} ec_sm_t;

typedef struct ec_slave_config
{
   uint32_t vendor;
   uint32_t product;
   uint32_t revision;
   struct
   {
      uint16_t address;
      uint16_t length;
      uint32_t flags;
   } sm[4];
   struct
   {
      uint32_t log_start;
      uint16_t log_length;
      uint8_t log_start_bit;
      uint8_t log_stop_bit;
      uint16_t phys_start;
      uint8_t phys_start_bit;
      uint8_t type;
      uint32_t activate;
   } fmmu[4];
} ec_slave_config_t;

EC_EXPORT ec_net_t * ec_net_init (const char * ifname);
EC_EXPORT ec_slave_t * ec_net_get_slave (ec_net_t * net, int ix);

EC_EXPORT int ec_config_static (ec_net_t * net, ec_slave_config_t * cfg);

EC_EXPORT ec_slave_t * ec_slave_get_next (ec_slave_t * slave);

EC_EXPORT uint32_t ec_slave_get_vendor (ec_slave_t * slave);
EC_EXPORT uint32_t ec_slave_get_product (ec_slave_t * slave);
EC_EXPORT uint32_t ec_slave_get_revision (ec_slave_t * slave);
EC_EXPORT uint32_t ec_slave_get_serial (ec_slave_t * slave);
EC_EXPORT int ec_slave_is_coe (ec_slave_t * slave);

EC_EXPORT ec_nic_t * ec_nic_init (const char * ifname);
EC_EXPORT void ec_nic_show_adapters (void);


#ifdef __cplusplus
}
#endif

#endif /* EC_API_H */
