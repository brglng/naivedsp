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

typedef void *NaiveAllocFunc(void *context, NaiveMemType type, NaiveUSize alignment, NaiveUSize size);

typedef struct _NaiveDefaultAllocator {
    NaiveUSize  _num_blocks_cap;
    NaiveUSize  num_blocks;
    void        **blocks;
} NaiveDefaultAllocator;

NaiveErr naive_default_allocator_init(NaiveDefaultAllocator *self);
void naive_default_allocator_finalize(NaiveDefaultAllocator *self);

NAIVE_ATTRIBUTE_MALLOC void *naive_default_alloc(void *allocator, NaiveMemType type, NaiveUSize alignment, NaiveUSize size);

#define _NAIVE_NEW0(_context, _alloc, _memtype, _type) _alloc(_context, _memtype, NAIVE_ALIGNOF(_type), sizeof(_type))
#define _NAIVE_NEW1(_context, _alloc, _memtype, _type, _count) _alloc(_context, _memtype, NAIVE_ALIGNOF(_type), sizeof(_type) * (_count))
#define _NAIVE_NEW(_0, _1, _macro_name, ...) _macro_name
#define NAIVE_NEW(_context, _alloc, _memtype, _type, ...) ((_type*)_NAIVE_NEW(_0, ##__VA_ARGS__, _NAIVE_NEW1, _NAIVE_NEW0)(_context, _alloc, _memtype, _type, ##__VA_ARGS__))

#define NAIVE_CEIL_8_BYTES(size) ((((NaiveUSize)(size) + 7) / 8) * 8)

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: __NAIVE_MEMORY_H__ */
