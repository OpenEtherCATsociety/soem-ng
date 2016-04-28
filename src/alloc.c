
#include "alloc.h"

#include "options.h"
#include "log.h"

#include <string.h>

typedef struct ec_alloc
{
   int nnets;
   int nslaves;
   int nstrings;
   ec_net_t net[MAX_NETS];
   ec_slave_t slave[MAX_SLAVES];
   char strings[MAX_STRINGS][MAX_STRING_LENGTH + 1];
} ec_alloc_t;

#ifndef WITH_MALLOC
static ec_alloc_t alloc;
#endif

ec_net_t * ec_alloc_net (void)
{
   ec_net_t * net = NULL;

#ifdef WITH_MALLOC
   net = malloc (sizeof(ec_net_t));
#else
   if (alloc.nnets < MAX_NETS)
   {
      net = &alloc.net[alloc.nnets++];
   }
#endif
   if (net == NULL)
   {
      LOG_ERROR (EC_ALLOC_DEBUG, "Out of nets\n");
      return NULL;
   }

   memset (net, 0, sizeof(ec_net_t));
   return net;
}

ec_slave_t * ec_alloc_slave (void)
{
   ec_slave_t * slave = NULL;

#ifdef WITH_MALLOC
   slave = malloc (sizeof(ec_slave_t));
#else
   if (alloc.nslaves < MAX_SLAVES)
   {
      slave = &alloc.slave[alloc.nslaves++];
   }
#endif
   if (slave == NULL)
   {
      LOG_ERROR (EC_ALLOC_DEBUG, "Out of slaves\n");
      return NULL;
   }

   memset (slave, 0, sizeof(ec_slave_t));
   return slave;
}

char * ec_alloc_string (size_t size, const char * src)
{
   char * string = NULL;

#ifdef WITH_MALLOC
   string = malloc (size);
#else
   if (alloc.nstrings < MAX_STRINGS)
   {
      string = alloc.strings[alloc.nstrings++];
   }

   if (size > MAX_STRING_LENGTH)
   {
      size = MAX_STRING_LENGTH;
   }
#endif
   if (string == NULL)
   {
      LOG_ERROR (EC_ALLOC_DEBUG, "Out of strings\n");
      return NULL;
   }

   strncpy (string, src, size);
   string[size] = 0;

   return string;
}

void ec_alloc_init (void)
{
   alloc.nnets = 0;
   alloc.nslaves = 0;
   alloc.nstrings = 0;
}
