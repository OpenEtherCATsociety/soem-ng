
#ifndef OSAL_H
#define OSAL_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdarg.h>
#include <stddef.h>
#include <nic.h>

void os_log (int type, const char * fmt, ...);
void * os_malloc (size_t size);

#ifdef __cplusplus
}
#endif

#endif /* OSAL_H */
