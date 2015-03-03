
#ifndef NIC_H
#define NIC_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <ec_api.h>
#include <ec_types.h>

int ec_nic_send (ec_nic_t * nic, const void * buffer, size_t size);
int ec_nic_receive (ec_nic_t * nic, void * buffer, size_t size);

#ifdef __cplusplus
}
#endif

#endif /* NIC_H */
