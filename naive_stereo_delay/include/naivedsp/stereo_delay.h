#ifndef __NAIVE_STEREO_DELAY_H__
#define __NAIVE_STEREO_DELAY_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "naivedsp/delay_buf.h"
#include "naivedsp/memory.h"
#include "naivedsp/math.h"
#include "naivedsp/math_types.h"

#define NAIVE_STEREO_DELAY_SCRATCH_SIZE(block_size_cap) (2 * NAIVE_CEIL_8_BYTES(sizeof(NaiveF32) * (block_size_cap)))

typedef struct {
    NaiveI32        delay_len_cap;
    NaiveDelayBuf   left_in_delay;
    NaiveDelayBuf   left_out_delay;
    NaiveDelayBuf   right_in_delay;
    NaiveDelayBuf   right_out_delay;
    NaiveI32        left_delay_len;
    NaiveF32        left_feedback_gain;
    NaiveF32        left_crossfeed_gain;
    NaiveF32        left_dry_gain;
    NaiveF32        left_wet_gain;
    NaiveI32        right_delay_len;
    NaiveF32        right_feedback_gain;
    NaiveF32        right_crossfeed_gain;
    NaiveF32        right_dry_gain;
    NaiveF32        right_wet_gain;
} NaiveStereoDelay;

NaiveErr naive_stereo_delay_init(NaiveStereoDelay *self,
                                 void *alloc_context,
                                 NaiveAllocFunc *alloc,
                                 NaiveI32 delay_len_cap);

NaiveErr naive_stereo_delay_process(NaiveStereoDelay *self,
                                    NaiveF32 *left_inout, NaiveF32 *right_inout,
                                    NaiveI32 block_size,
                                    void *scratch);

void naive_stereo_delay_reset(NaiveStereoDelay *self);
void naive_stereo_delay_set_default_params(NaiveStereoDelay *self);

NaiveErr naive_stereo_delay_set_left_delay_len(NaiveStereoDelay *self, NaiveI32 delay_len);
NaiveErr naive_stereo_delay_set_left_feedback_gain(NaiveStereoDelay *self, NaiveF32 feedback_gain);
NaiveErr naive_stereo_delay_set_left_crossfeed_gain(NaiveStereoDelay *self, NaiveF32 crossfeed_gain);
NaiveErr naive_stereo_delay_set_left_dry_gain(NaiveStereoDelay *self, NaiveF32 dry_gain);
NaiveErr naive_stereo_delay_set_left_wet_gain(NaiveStereoDelay *self, NaiveF32 wet_gain);
NaiveErr naive_stereo_delay_set_right_delay_len(NaiveStereoDelay *self, NaiveI32 delay_len);
NaiveErr naive_stereo_delay_set_right_feedback_gain(NaiveStereoDelay *self, NaiveF32 feedback_gain);
NaiveErr naive_stereo_delay_set_right_crossfeed_gain(NaiveStereoDelay *self, NaiveF32 crossfeed_gain);
NaiveErr naive_stereo_delay_set_right_dry_gain(NaiveStereoDelay *self, NaiveF32 dry_gain);
NaiveErr naive_stereo_delay_set_right_wet_gain(NaiveStereoDelay *self, NaiveF32 wet_gain);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: __NAIVE_STEREO_DELAY_H__ */
