
#include "osal.h"
#include "options.h"
#include "log.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>


void os_log (int type, const char * fmt, ...)
{
   va_list list;

   switch(LOG_LEVEL_GET (type))
   {
   case LOG_LEVEL_DEBUG: printf ("DEBUG - "); break;
   case LOG_LEVEL_INFO: printf ("INFO - "); break;
   case LOG_LEVEL_WARNING: printf ("WARNING - "); break;
   case LOG_LEVEL_ERROR: printf ("ERROR - "); break;
   default: break;
   }

   va_start (list, fmt);
   vprintf (fmt, list);
   va_end (list);
}

void * os_malloc (size_t size)
{
    return malloc (size);
}

