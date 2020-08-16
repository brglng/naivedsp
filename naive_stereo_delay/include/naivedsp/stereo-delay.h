#ifndef __NAIVE_STEREO_DELAY_H__
#define __NAIVE_STEREO_DELAY_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "naivedsp/fifo.h"
#include "naivedsp/memory.h"
#include "naivedsp/math.h"
#include "naivedsp/misc.h"
#include "naivedsp/typedefs.h"

typedef struct {
  NaiveFifo           left_in_delay;
  NaiveFifo           left_out_delay;
  NaiveFifo           right_in_delay;
  NaiveFifo           right_out_delay;
  NaiveI32            max_delay_len;
  NaiveI32            max_block_size;
  NaiveI32            left_delay_len;
  float             left_feedback_gain;
  float             left_mix_level;
  float             left_crossfeed_gain;
  float             right_delay_len;
  float             right_feedback_gain;
  float             right_mix_level;
  float             right_crossfeed_gain;
} NaiveStereoDelay;

NAIVE_INLINE NaiveUSize naive_stereo_delay_scratch_size(NaiveI32 block_size,
                                                  NaiveI32 left_delay_len,
                                                  NaiveI32 right_delay_len)
{
  return 2 * NAIVE_CEIL_8_BYTES(sizeof(float) * block_size) +
         2 * NAIVE_CEIL_8_BYTES(sizeof(float) *
                                NAIVE_MAX(NAIVE_MIN(block_size, left_delay_len),
                                        NAIVE_MIN(block_size, right_delay_len)));
}

NaiveResult naive_stereo_delay_init(NaiveStereoDelay *self,
                                NaiveI32 max_delay_len,
                                NaiveI32 max_block_size,
                                void *allocator);

void naive_stereo_delay_finalize(NaiveStereoDelay *self);

NaiveResult naive_stereo_delay_process(NaiveStereoDelay *self,
                                   float *left_inout, float *right_inout,
                                   NaiveI32 block_size, float *scratch);

NaiveResult naive_stereo_delay_set_left_delay_length(NaiveStereoDelay *self, NaiveI32 len);
NaiveResult naive_stereo_delay_set_left_feedback_gain(NaiveStereoDelay *self, float gain);
NaiveResult naive_stereo_delay_set_left_mix_level(NaiveStereoDelay *self, float mix_level);
NaiveResult naive_stereo_delay_set_left_crossfeed_gain(NaiveStereoDelay *self, float crossfeed);
NaiveResult naive_stereo_delay_set_right_delay_length(NaiveStereoDelay *self, NaiveI32 len);
NaiveResult naive_stereo_delay_set_right_feedback_gain(NaiveStereoDelay *self, float gain);
NaiveResult naive_stereo_delay_set_right_mix_level(NaiveStereoDelay *self, float mix_level);
NaiveResult naive_stereo_delay_set_right_crossfeed_gain(NaiveStereoDelay *self, float crossfeed);

NaiveI32  naive_stereo_delay_get_left_delay_length(NaiveStereoDelay *self);
float   naive_stereo_delay_get_left_feedback_gain(NaiveStereoDelay *self);
float   naive_stereo_delay_get_left_mix_level(NaiveStereoDelay *self);
float   naive_stereo_delay_get_left_crossfeed_gain(NaiveStereoDelay *self);
NaiveI32  naive_stereo_delay_get_right_delay_length(NaiveStereoDelay *self);
float   naive_stereo_delay_get_right_feedback_gain(NaiveStereoDelay *self);
float   naive_stereo_delay_get_right_mix_level(NaiveStereoDelay *self);
float   naive_stereo_delay_get_right_crossfeed_gain(NaiveStereoDelay *self);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: __NAIVE_STEREO_DELAY_H__ */
