
#include "slave.h"

#include "options.h"
#include "log.h"
#include "osal.h"

ec_slave_t * ec_slave_get_next (ec_slave_t * slave)
{
   return slave->next;
}

uint32_t ec_slave_get_vendor (ec_slave_t * slave)
{
   return slave->vendor;
}

uint32_t ec_slave_get_product (ec_slave_t * slave)
{
   return slave->product;
}

uint32_t ec_slave_get_revision (ec_slave_t * slave)
{
   return slave->revision;
}

uint32_t ec_slave_get_serial (ec_slave_t * slave)
{
   return slave->serial;
}

int ec_slave_is_coe (ec_slave_t * slave)
{
   return slave->mboxproto == 0x0C; /* TODO */
}
