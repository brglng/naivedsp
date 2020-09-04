#ifndef __NAIVE_MATH_H__
#define __NAIVE_MATH_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>
#include "naivedsp/assert.h"
#include "naivedsp/platdefs.h"
#include "naivedsp/math_types.h"

void naive_gain(NaiveF32 *inout, NaiveI32 block_size, NaiveF32 gain);
void naive_mix(NaiveF32 *output, NaiveF32 *input, NaiveI32 len);
void naive_mix_with_gain(NaiveF32 *output, NaiveF32 *input, NaiveF32 gain, NaiveI32 len);

NAIVE_INLINE NaiveU64 naive_next_pow_of_two_u64(NaiveU64 x)
{
    NAIVE_ASSERT(x <= 0x8000000000000000UL);
    x--;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    x |= x >> 32;
    x++;
    return x;
}

NAIVE_INLINE NaiveU32 naive_next_pow_of_two_u32(NaiveU32 x)
{
    NAIVE_ASSERT(x <= 0x80000000U);
    x--;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    x++;
    return x;
}

#if NAIVE_X86_64
NAIVE_INLINE NaiveUSize naive_next_pow_of_two_usize(NaiveUSize x) {
    return naive_next_pow_of_two_u64(x);
}
#else
NAIVE_INLINE NaiveUSize naive_next_pow_of_two_usize(NaiveUSize x) {
    return naive_next_pow_of_two_u32(x);
}
#endif

NAIVE_INLINE NaiveI32 naive_next_pow_of_two_i32(NaiveI32 x)
{
    NAIVE_ASSERT(x >= 0 && x <= 0x40000000);
    x--;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    x++;
    return x;
}

#define NAIVE_IS_POW_OF_TWO(x) ((x) && !((x) & ((x) - 1)))
#define NAIVE_MIN(a, b) (((a) < (b)) ? (a) : (b))
#define NAIVE_MAX(a, b) (((a) > (b)) ? (a) : (b))
#define NAIVE_ABS(x)  (((x) < 0) ? (-(x)) : (x))

#define NAIVE_PI  3.14159265358979323f
#define NAIVE_E   2.7182818284590452354f

#define NAIVE_DB(x) (20 * log10f(x))

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: __NAIVE_MATH_H__ */
