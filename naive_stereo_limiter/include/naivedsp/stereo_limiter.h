#ifndef __NAIVE_STEREO_LIMITER_H__
#define __NAIVE_STEREO_LIMITER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "naivedsp/delay_buf.h"

typedef struct {
    NaiveI32        sample_rate;
    NaiveBool       enabled;
    NaiveF32        input_gain;
    NaiveF32        output_gain;
    NaiveF32        threshold;
    NaiveI32        delay_len;
    NaiveF32        attack_time;
    NaiveF32        attack_coeff;
    NaiveF32        release_time;
    NaiveF32        release_coeff;
    NaiveF32        prev_left_peak;
    NaiveF32        prev_right_peak;
    NaiveF32        prev_gain;
    NaiveDelayBuf   left_delay_buf;
    NaiveDelayBuf   right_delay_buf;
} NaiveStereoLimiter;

NaiveErr naive_stereo_limiter_init(NaiveStereoLimiter *self, void *alloc_context, NaiveAllocFunc *alloc,
                                   NaiveI32 sample_rate, NaiveI32 delay_len_cap);
NaiveErr naive_stereo_limiter_process(NaiveStereoLimiter *self,
                                      NaiveF32 *in_left, NaiveF32 *in_right,
                                      NaiveF32 *out_left, NaiveF32 *out_right,
                                      NaiveI32 block_size);
void naive_stereo_limiter_reset(NaiveStereoLimiter *self);

NaiveBool naive_stereo_limiter_get_enabled(NAIVE_CONST NaiveStereoLimiter *self);
NaiveF32 naive_stereo_limiter_get_input_gain(NAIVE_CONST NaiveStereoLimiter *self);
NaiveF32 naive_stereo_limiter_get_output_gain(NAIVE_CONST NaiveStereoLimiter *self);
NaiveF32 naive_stereo_limiter_get_threshold(NAIVE_CONST NaiveStereoLimiter *self);
NaiveF32 naive_stereo_limiter_get_attack_time(NAIVE_CONST NaiveStereoLimiter *self);
NaiveF32 naive_stereo_limiter_get_release_time(NAIVE_CONST NaiveStereoLimiter *self);
NaiveI32 naive_stereo_limiter_get_delay_len(NAIVE_CONST NaiveStereoLimiter *self);

NaiveErr naive_stereo_limiter_set_enabled(NaiveStereoLimiter *self, NaiveBool enabled);
NaiveErr naive_stereo_limiter_set_input_gain(NaiveStereoLimiter *self, NaiveF32 input_gain);
NaiveErr naive_stereo_limiter_set_output_gain(NaiveStereoLimiter *self, NaiveF32 output_gain);
NaiveErr naive_stereo_limiter_set_threshold(NaiveStereoLimiter *self, NaiveF32 threshold);
NaiveErr naive_stereo_limiter_set_attack_time(NaiveStereoLimiter *self, NaiveF32 attack_time);
NaiveErr naive_stereo_limiter_set_release_time(NaiveStereoLimiter *self, NaiveF32 release_time);
NaiveErr naive_stereo_limiter_set_delay_len(NaiveStereoLimiter *self, NaiveI32 delay_len);
void naive_stereo_limiter_set_default_params(NaiveStereoLimiter *self);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: __NAIVE_STEREO_LIMITER_H__ */
