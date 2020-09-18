#ifndef __NAIVE_DELAY_H__
#define __NAIVE_DELAY_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "naivedsp/delay_buf.h"
#include "naivedsp/math.h"
#include "naivedsp/memory.h"
#include "naivedsp/math_types.h"

#define NAIVE_DELAY_SCRATCH_SIZE(block_size_cap, delay_len_cap) (sizeof(NaiveF32) * NAIVE_MIN(block_size_cap, delay_len_cap))

typedef struct {
    NaiveI32        delay_len_cap;
    NaiveDelayBuf   in_delay_buf;
    NaiveDelayBuf   out_delay_buf;
    NaiveI32        delay_len;
    NaiveF32        feedback_gain;
    NaiveF32        dry_gain;
    NaiveF32        wet_gain;
} NaiveDelay;

NaiveErr naive_delay_init(NaiveDelay *self, void *alloc_context, NaiveAllocFunc *alloc, NaiveI32 delay_len_cap);

void naive_delay_reset(NaiveDelay *self);

NaiveErr naive_delay_process(NaiveDelay *self, NaiveF32 *inout, NaiveI32 block_size, void *scratch);

void naive_delay_set_default_params(NaiveDelay *self);
NaiveErr naive_delay_set_delay_len(NaiveDelay *self, NaiveI32 delay_len);
NaiveErr naive_delay_set_feedback_gain(NaiveDelay *self, NaiveF32 feedback_gain);
NaiveErr naive_delay_set_dry_gain(NaiveDelay *self, NaiveF32 dry_gain);
NaiveErr naive_delay_set_wet_gain(NaiveDelay *self, NaiveF32 wet_gain);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: __NAIVE_DELAY_H__ */
