#ifndef __NAIVE_MEMORY_H__
#define __NAIVE_MEMORY_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include "naivedsp/platdefs.h"
#include "naivedsp/typedefs.h"

#ifdef __cplusplus
#define NAIVE_ALIGNOF(type) alignof(type)
#else
#if __STDC_VERSION__ >= 201112L
#define NAIVE_ALIGNOF(type) _Alignof(type)
#else
#define NAIVE_ALIGNOF(type) offsetof(struct { char c; type d; }, d)
#endif
#endif

#if NAIVE_GCC || NAIVE_CLANG
#define NAIVE_ATTRIBUTE_MALLOC __attribute__((malloc))
#else
#define NAIVE_ATTRIBUTE_MALLOC
#endif

typedef enum {
    NAIVE_MEM_COEFF,
    NAIVE_MEM_STATE,
    NAIVE_MEM_SCRATCH,
} NaiveMemType;

typedef void *NaiveAllocFunc(void *context, NaiveMemType type, NaiveUSize alignment, NaiveUSize size);

#if !NAIVE_ADSP21489

typedef struct _NaiveDefaultAllocator {
    NaiveUSize    num_blocks;
    NaiveUSize    num_blocks_cap;
    void        **blocks;
} NaiveDefaultAllocator;

void naive_default_allocator_init(NaiveDefaultAllocator *self);
void naive_default_allocator_finalize(NaiveDefaultAllocator *self);

NAIVE_ATTRIBUTE_MALLOC void *naive_default_alloc(void *allocator, NaiveMemType type, NaiveUSize alignment, NaiveUSize size);

#endif

#if NAIVE_ADSP21489
#define NAIVE_CEIL_8_BYTES(size) ((NaiveUSize)(size))
#else
#define NAIVE_CEIL_8_BYTES(size) ((((NaiveUSize)(size) + 7) / 8) * 8)
#endif

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: __NAIVE_MEMORY_H__ */
