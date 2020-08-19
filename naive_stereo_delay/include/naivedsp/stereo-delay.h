#ifndef __NAIVE_STEREO_DELAY_H__
#define __NAIVE_STEREO_DELAY_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "naivedsp/delay_buf.h"
#include "naivedsp/memory.h"
#include "naivedsp/math.h"
#include "naivedsp/typedefs.h"

typedef struct {
  NaiveDelayBuf     left_in_delay;
  NaiveDelayBuf     left_out_delay;
  NaiveDelayBuf     right_in_delay;
  NaiveDelayBuf     right_out_delay;
  NaiveI32          delay_len_cap;
  NaiveI32          block_size_cap;
  NaiveI32          left_delay_len;
  NaiveF32          left_feedback_gain;
  NaiveF32          left_mix_level;
  NaiveF32          left_crossfeed_gain;
  NaiveF32          right_delay_len;
  NaiveF32          right_feedback_gain;
  NaiveF32          right_mix_level;
  NaiveF32          right_crossfeed_gain;
} NaiveStereoDelay;

NAIVE_INLINE NaiveUSize naive_stereo_delay_scratch_size(NaiveI32 block_size,
                                                  NaiveI32 left_delay_len,
                                                  NaiveI32 right_delay_len)
{
  return 2 * NAIVE_CEIL_8_BYTES(sizeof(NaiveF32) * block_size) +
         2 * NAIVE_CEIL_8_BYTES(sizeof(NaiveF32) *
                                NAIVE_MAX(NAIVE_MIN(block_size, left_delay_len),
                                        NAIVE_MIN(block_size, right_delay_len)));
}

NaiveResult naive_stereo_delay_init(NaiveStereoDelay *self,
                                NaiveI32 max_delay_len,
                                NaiveI32 max_block_size,
                                void *allocator);

void naive_stereo_delay_finalize(NaiveStereoDelay *self);

NaiveResult naive_stereo_delay_process(NaiveStereoDelay *self,
                                   NaiveF32 *left_inout, NaiveF32 *right_inout,
                                   NaiveI32 block_size, NaiveF32 *scratch);

NaiveResult naive_stereo_delay_set_left_delay_length(NaiveStereoDelay *self, NaiveI32 len);
NaiveResult naive_stereo_delay_set_left_feedback_gain(NaiveStereoDelay *self, NaiveF32 gain);
NaiveResult naive_stereo_delay_set_left_mix_level(NaiveStereoDelay *self, NaiveF32 mix_level);
NaiveResult naive_stereo_delay_set_left_crossfeed_gain(NaiveStereoDelay *self, NaiveF32 crossfeed);
NaiveResult naive_stereo_delay_set_right_delay_length(NaiveStereoDelay *self, NaiveI32 len);
NaiveResult naive_stereo_delay_set_right_feedback_gain(NaiveStereoDelay *self, NaiveF32 gain);
NaiveResult naive_stereo_delay_set_right_mix_level(NaiveStereoDelay *self, NaiveF32 mix_level);
NaiveResult naive_stereo_delay_set_right_crossfeed_gain(NaiveStereoDelay *self, NaiveF32 crossfeed);

NaiveI32  naive_stereo_delay_get_left_delay_length(NaiveStereoDelay *self);
NaiveF32   naive_stereo_delay_get_left_feedback_gain(NaiveStereoDelay *self);
NaiveF32   naive_stereo_delay_get_left_mix_level(NaiveStereoDelay *self);
NaiveF32   naive_stereo_delay_get_left_crossfeed_gain(NaiveStereoDelay *self);
NaiveI32  naive_stereo_delay_get_right_delay_length(NaiveStereoDelay *self);
NaiveF32   naive_stereo_delay_get_right_feedback_gain(NaiveStereoDelay *self);
NaiveF32   naive_stereo_delay_get_right_mix_level(NaiveStereoDelay *self);
NaiveF32   naive_stereo_delay_get_right_crossfeed_gain(NaiveStereoDelay *self);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: __NAIVE_STEREO_DELAY_H__ */
