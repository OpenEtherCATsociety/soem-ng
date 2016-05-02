
#ifndef ALLOC_H
#define ALLOC_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "ec_api.h"
#include "ec_types.h"

ec_net_t * ec_alloc_net (void);
ec_slave_t * ec_alloc_slave (void);
char * ec_alloc_string (size_t size, const char * src);

void ec_alloc_init (void);

#ifdef __cplusplus
}
#endif

#endif /* ALLOC_H */
