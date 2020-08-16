#include <string.h>
#include "naivedsp/stereo-delay.h"
#include "naivedsp/fifo.h"
#include "naivedsp/math.h"

NaiveResult naive_stereo_delay_init(NaiveStereoDelay *self,
                                NaiveI32 max_delay_len,
                                NaiveI32 max_block_size,
                                void *allocator)
{
  int err = NAIVE_OK;

  err = naive_fifo_init(&self->left_in_delay, max_delay_len * sizeof(float), allocator);
  if (!err)
    err = naive_fifo_init(&self->left_out_delay,
                        NAIVE_MAX(max_delay_len, max_block_size) * sizeof(float), allocator);

  if (!err)
    err = naive_fifo_init(&self->right_in_delay, max_delay_len * sizeof(float), allocator);

  if (!err)
    err = naive_fifo_init(&self->right_out_delay,
                        NAIVE_MAX(max_delay_len, max_block_size) * sizeof(float), allocator);

  if (!err) {
    self->max_delay_len = max_delay_len;
    self->max_block_size = max_block_size;

    self->left_delay_len = 0;
    self->left_feedback_gain = 0.0f;
    self->left_mix_level = 0.0f;
    self->left_crossfeed_gain = 0.0f;
    self->right_delay_len = 0;
    self->right_feedback_gain = 0.0f;
    self->right_mix_level = 0.0f;
    self->right_crossfeed_gain = 0.0f;
  }

  return err;
}

void naive_stereo_delay_finalize(NaiveStereoDelay *self)
{
  if (self) {
    naive_fifo_finalize(&self->right_out_delay);
    naive_fifo_finalize(&self->right_in_delay);
    naive_fifo_finalize(&self->left_out_delay);
    naive_fifo_finalize(&self->left_in_delay);
  }
}

NaiveResult naive_stereo_delay_process(NaiveStereoDelay *self,
                                   float *left, float *right,
                                   NaiveI32 block_size, float *scratch)
{
  if (block_size > self->max_block_size) {
    return NAIVE_ERR_INVALID_PARAMETER;
  }

  NaiveI32 left_delay_len = self->left_delay_len;
  float left_feedback_gain = self->left_feedback_gain;
  float left_mix_level = self->left_mix_level;
  float left_crossfeed_gain = self->left_crossfeed_gain;

  NaiveI32 right_delay_len = self->right_delay_len;
  float right_feedback_gain = self->right_feedback_gain;
  float right_mix_level = self->right_mix_level;
  float right_crossfeed_gain = self->right_crossfeed_gain;

  float *left_x = left;
  float *right_x = right;
  float *left_y = scratch;
  float *right_y = (float *)((NaiveIntPtr)scratch +
                             NAIVE_CEIL_8_BYTES(sizeof(float) * block_size));

  float *cur_left_x = left_x;
  float *cur_left_y = left_y;
  while (cur_left_x < left + block_size &&
         naive_fifo_get_size(&self->left_in_delay) < sizeof(float) * left_delay_len)
  {
    NaiveI32 sub_block_size = NAIVE_MIN(
      left_delay_len - naive_fifo_get_size(&self->left_in_delay) / sizeof(float),
      (NaiveU32)(cur_left_x + block_size - left));

    naive_fifo_write(&self->left_in_delay, cur_left_x, sizeof(float) * sub_block_size);
    memcpy(cur_left_y, cur_left_x, sizeof(float) * sub_block_size);

    cur_left_x += sub_block_size;
    cur_left_y += sub_block_size;
  }

  while (cur_left_x < left + block_size &&
         naive_fifo_get_size(&self->left_in_delay) >= sizeof(float) * left_delay_len)
  {
    NaiveI32 sub_block_size = NAIVE_MIN(left_delay_len, (left + block_size - cur_left_x));

    float *left_x_delayed = (float *)((NaiveIntPtr)right_y +
                                      NAIVE_CEIL_8_BYTES(sizeof(float) * block_size));
    float *left_y_delayed = (float *)((NaiveIntPtr)left_x_delayed +
                                      NAIVE_CEIL_8_BYTES(sizeof(float) * sub_block_size));

    naive_fifo_read(&self->left_in_delay, left_x_delayed, sizeof(float) * sub_block_size);
    naive_fifo_read(&self->left_out_delay, left_y_delayed, sizeof(float) * sub_block_size);
    naive_fifo_write(&self->left_in_delay, cur_left_x, sizeof(float) * sub_block_size);

    for (NaiveI32 i = 0; i <sub_block_size; i++) {
      cur_left_y[i] = left_x_delayed[i] + left_feedback_gain * left_y_delayed[i];
    }

    cur_left_x += sub_block_size;
    cur_left_y += sub_block_size;
  }

  float *cur_right_x = right_x;
  float *cur_right_y = right_y;
  while (cur_right_x < right + block_size &&
         naive_fifo_get_size(&self->right_in_delay) < sizeof(float) * right_delay_len)
  {
    NaiveI32 sub_block_size = NAIVE_MIN(
      right_delay_len - naive_fifo_get_size(&self->right_in_delay) / sizeof(float),
      (NaiveU32)(cur_right_x + block_size - right));

    naive_fifo_write(&self->right_in_delay, cur_right_x, sizeof(float) * sub_block_size);
    memcpy(cur_right_y, cur_right_x, sizeof(float) * sub_block_size);

    cur_right_x += sub_block_size;
    cur_right_y += sub_block_size;
  }

  while (cur_right_x < right + block_size &&
         naive_fifo_get_size(&self->right_in_delay) >= sizeof(float) * right_delay_len)
  {
    NaiveI32 sub_block_size = NAIVE_MIN(right_delay_len, (right + block_size - cur_right_x));

    float *right_x_delayed = (float *)((NaiveIntPtr)right_y +
                                      NAIVE_CEIL_8_BYTES(sizeof(float) * block_size));
    float *right_y_delayed = (float *)((NaiveIntPtr)right_x_delayed +
                                       NAIVE_CEIL_8_BYTES(
                                         sizeof(float) * sub_block_size));

    naive_fifo_read(&self->right_in_delay, right_x_delayed, sizeof(float) * sub_block_size);
    naive_fifo_read(&self->right_out_delay, right_y_delayed, sizeof(float) * sub_block_size);
    naive_fifo_write(&self->right_in_delay, cur_right_x, sizeof(float) * sub_block_size);

    for (NaiveI32 i = 0; i < sub_block_size; i++) {
      cur_right_y[i] = right_x_delayed[i] + right_feedback_gain * right_y_delayed[i];
    }

    cur_right_x += sub_block_size;
    cur_right_y += sub_block_size;
  }

  /* 进行 crossfeed */
  for (NaiveI32 i = 0; i < block_size; i++) {
    left_y[i] += left_crossfeed_gain * right_y[i];
    right_y[i] += right_crossfeed_gain * left_y[i];
  }

  naive_fifo_write(&self->left_out_delay, left_y, sizeof(float) * block_size);
  naive_fifo_write(&self->right_out_delay, right_y, sizeof(float) * block_size);

  /* 干湿比例 */
  for (NaiveI32 i = 0; i < block_size; i++) {
    left[i] = (1 - left_mix_level) * left[i] + left_mix_level * left_y[i];
    right[i] = (1 - right_mix_level) * right[i] + right_mix_level * right_y[i];
  }

  return NAIVE_OK;
}

NaiveResult naive_stereo_delay_set_left_delay_length(NaiveStereoDelay *self, NaiveI32 len)
{
  if (len < 0 || len > self->max_delay_len) {
    return NAIVE_ERR_INVALID_PARAMETER;
  }
  self->left_delay_len = len;
  return NAIVE_OK;
}

NaiveResult naive_stereo_delay_set_left_feedback_gain(NaiveStereoDelay *self, float gain)
{
  if (gain < 0.0f) {
    return NAIVE_ERR_INVALID_PARAMETER;
  }
  self->left_feedback_gain = gain;
  return NAIVE_OK;
}

NaiveResult naive_stereo_delay_set_left_mix_level(NaiveStereoDelay *self, float mix_level)
{
  if (mix_level < 0.0f || mix_level > 1.0f) {
    return NAIVE_ERR_INVALID_PARAMETER;
  }
  self->left_mix_level = mix_level;
  return NAIVE_OK;
}

NaiveResult naive_stereo_delay_set_left_crossfeed_gain(NaiveStereoDelay *self, float gain)
{
  if (gain < 0.0f) {
    return NAIVE_ERR_INVALID_PARAMETER;
  }
  self->left_crossfeed_gain = gain;
  return NAIVE_OK;
}

NaiveResult naive_stereo_delay_set_right_delay_length(NaiveStereoDelay *self, NaiveI32 len)
{
  if (len < 0 || len > self->max_delay_len) {
    return NAIVE_ERR_INVALID_PARAMETER;
  }
  self->right_delay_len = len;
  return NAIVE_OK;
}

NaiveResult naive_stereo_delay_set_right_feedback_gain(NaiveStereoDelay *self, float gain)
{
  if (gain < 0.0f) {
    return NAIVE_ERR_INVALID_PARAMETER;
  }
  self->right_feedback_gain = gain;
  return NAIVE_OK;
}

NaiveResult naive_stereo_delay_set_right_mix_level(NaiveStereoDelay *self, float mix_level)
{
  if (mix_level < 0.0f || mix_level > 1.0f) {
    return NAIVE_ERR_INVALID_PARAMETER;
  }
  self->right_mix_level = mix_level;
  return NAIVE_OK;
}

NaiveResult naive_stereo_delay_set_right_crossfeed_gain(NaiveStereoDelay *self, float gain)
{
  if (gain < 0.0f) {
    return NAIVE_ERR_INVALID_PARAMETER;
  }
  self->right_crossfeed_gain = gain;
  return NAIVE_OK;
}

NaiveI32 naive_stereo_delay_get_left_delay_length(NaiveStereoDelay *self)
{
  return self->left_delay_len;
}

float naive_stereo_delay_get_left_feedback_gain(NaiveStereoDelay *self)
{
  return self->left_feedback_gain;
}

float naive_stereo_delay_get_left_mix_level(NaiveStereoDelay *self)
{
  return self->left_mix_level;
}

float naive_stereo_delay_get_left_crossfeed_gain(NaiveStereoDelay *self)
{
  return self->left_crossfeed_gain;
}

NaiveI32 naive_stereo_delay_get_right_delay_length(NaiveStereoDelay *self)
{
  return self->right_delay_len;
}

float naive_stereo_delay_get_right_feedback_gain(NaiveStereoDelay *self)
{
  return self->right_feedback_gain;
}

float naive_stereo_delay_get_right_mix_level(NaiveStereoDelay *self)
{
  return self->right_mix_level;
}

float naive_stereo_delay_get_right_crossfeed_gain(NaiveStereoDelay *self)
{
  return self->right_crossfeed_gain;
}
