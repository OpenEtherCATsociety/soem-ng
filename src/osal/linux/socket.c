
#include <ec_nic.h>
#include <options.h>
#include <log.h>
#include <net/if.h>
#include <sys/socket.h>
#include <stdio.h>

void ec_nic_show_adapters (void)
{
   struct if_nameindex *ids;
   int ix;

   ids = if_nameindex();
   for (ix = 0; ids[ix].if_index != 0; ix++)
   {
      printf ("%s\n", ids[ix].if_name);
   }

   if_freenameindex (ids);
}
