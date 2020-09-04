#ifndef __NAIVE_MEMORY_H__
#define __NAIVE_MEMORY_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "naivedsp/err.h"
#include "naivedsp/platdefs.h"
#include "naivedsp/math_types.h"

#ifdef __cplusplus
#define NAIVE_ALIGNOF(type) alignof(type)
#else
#if __STDC_VERSION__ >= 201112L
#define NAIVE_ALIGNOF(type) _Alignof(type)
#else
#include <stddef.h>
#define NAIVE_ALIGNOF(type) offsetof(struct { char c; type d; }, d)
#endif
#endif

#if NAIVE_GCC || NAIVE_CLANG
#define NAIVE_ATTRIBUTE_MALLOC __attribute__((malloc))
#else
#define NAIVE_ATTRIBUTE_MALLOC
#endif

typedef enum {
    NAIVE_MEM_PARAM,
    NAIVE_MEM_STATE,
    NAIVE_MEM_SCRATCH,
} NaiveMemType;

typedef void *NaiveAllocFunc(void *context, NaiveMemType type, NaiveUSize size);

typedef struct _NaiveDefaultAllocator {
    NaiveUSize  _num_blocks_cap;
    NaiveUSize  num_blocks;
    void        **blocks;
    NaiveUSize  scratch_size;
    void        *scratch;
} NaiveDefaultAllocator;

NaiveErr naive_default_allocator_init(NaiveDefaultAllocator *self);
void naive_default_allocator_finalize(NaiveDefaultAllocator *self);

NAIVE_ATTRIBUTE_MALLOC void *naive_default_alloc(void *allocator, NaiveMemType type, NaiveUSize size);

#define NAIVE_CEIL_8_BYTES(size) ((((NaiveUSize)(size) + 7) / 8) * 8)

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: __NAIVE_MEMORY_H__ */
