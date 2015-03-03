
#ifndef CC_H
#define CC_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <kern.h>

#define CC_PACKED_BEGIN
#define CC_PACKED_END
#define CC_PACKED       __attribute__((packed))

#define CC_TO_LE32(x)   (x)
#define CC_TO_LE16(x)   (x)
#define CC_FROM_LE32(x) (x)
#define CC_FROM_LE16(x) (x)

#define CC_BIT(n)       (1 << n##U)

#define CC_ASSERT(exp) /* assert (exp) */
#define CC_STATIC_ASSERT(exp) _Static_assert (exp, "")
/* #define CC_STATIC_ASSERT(exp) COMPILETIME_ASSERT (exp) */

#ifdef __cplusplus
}
#endif

#endif /* CC_H */
