
#ifndef CC_H
#define CC_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <assert.h>

#define CC_PACKED_BEGIN __pragma(pack(push, 1))
#define CC_PACKED_END   __pragma(pack(pop))
#define CC_PACKED

#define CC_TO_LE32(x)   (x)
#define CC_TO_LE16(x)   (x)
#define CC_FROM_LE32(x) (x)
#define CC_FROM_LE16(x) (x)


#define CC_BIT(n)       (1 << n##U)

#define CC_ASSERT(exp) assert (exp)
#define CC_STATIC_ASSERT(exp) static_assert ((exp),"")

#ifdef __cplusplus
}
#endif

#endif /* CC_H */
