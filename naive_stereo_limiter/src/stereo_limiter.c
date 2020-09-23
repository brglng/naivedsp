#include <string.h>
#include "naivedsp/math.h"
#include "naivedsp/stereo_limiter.h"

NaiveErr naive_stereo_limiter_init(NaiveStereoLimiter *self, void *alloc_context, NaiveAllocFunc *alloc, NaiveI32 delay_len_cap)
{
    NaiveErr err = NAIVE_OK;
    NaiveErr err1 = NAIVE_OK;

    self->enabled = NAIVE_FALSE;
    self->input_gain = 1.0f;
    self->output_gain = 1.0f;
    self->threshold = 1.0f;
    self->delay_len = 5;
    self->attack_coeff = 1.0f;
    self->release_coeff = 1.0f - expf(-logf(9) / (0.12f * 44100.0f));
    self->prev_left_peak = 0.0f;
    self->prev_right_peak = 0.0f;
    self->prev_gain = 1.0f;

    err = naive_delay_buf_init(&self->left_delay_buf, alloc_context, alloc, delay_len_cap);
    err1 = naive_delay_buf_init(&self->right_delay_buf, alloc_context, alloc, delay_len_cap);
    if (err1 && !err)
        err = err1;

    return err;
}

NaiveErr naive_stereo_limiter_process(NaiveStereoLimiter *self, NaiveF32 *in_left, NaiveF32 *in_right, NaiveF32 *out_left, NaiveF32 *out_right, NaiveI32 block_size)
{
    if (block_size < 0)
        return NAIVE_ERR_INVALID_PARAMETER;

    if (block_size == 0)
        return NAIVE_OK;

    if (self->delay_len == 0) {
        memcpy(out_left, in_left, sizeof(NaiveF32) * (NaiveUSize)block_size);
        memcpy(out_right, in_right, sizeof(NaiveF32) * (NaiveUSize)block_size);
    } else {
        NaiveI32 n = 0;
        while (n < block_size && self->left_delay_buf.len < self->delay_len) {
            NaiveI32 len = NAIVE_MIN(self->delay_len - self->left_delay_buf.len, block_size - n);
            memset(&out_left[n], 0, sizeof(NaiveF32) * (NaiveUSize)len);
            naive_delay_buf_write(&self->left_delay_buf, &in_left[n], len);
            naive_delay_buf_write(&self->right_delay_buf, &in_right[n], len);
            n += len;
        }
        while (n < block_size && self->left_delay_buf.len >= self->delay_len) {
            NaiveI32 len = NAIVE_MIN(self->delay_len, block_size - n);
            memcpy(&out_left[n], naive_delay_buf_front(&self->left_delay_buf), sizeof(NaiveF32) * (NaiveUSize)len);
            memcpy(&out_right[n], naive_delay_buf_front(&self->right_delay_buf), sizeof(NaiveF32) * (NaiveUSize)len);
            naive_delay_buf_drain(&self->left_delay_buf, len);
            naive_delay_buf_drain(&self->right_delay_buf, len);
            naive_delay_buf_write(&self->left_delay_buf, &in_left[n], len);
            naive_delay_buf_write(&self->right_delay_buf, &in_right[n], len);
            n += len;
        }
    }

    NaiveF32 input_gain = self->input_gain;
    NaiveF32 output_gain = self->output_gain;
    NaiveF32 attack_coeff = self->attack_coeff;
    NaiveF32 release_coeff = self->release_coeff;
    NaiveF32 threshold = self->threshold;
    NaiveF32 left_peak = self->prev_left_peak;
    NaiveF32 right_peak = self->prev_right_peak;
    NaiveF32 gain = self->prev_gain;
    for (NaiveI32 i = 0; i < block_size; ++i) {
        NaiveF32 left_amp = fabsf(in_left[i] * input_gain);
        NaiveF32 right_amp = fabsf(in_right[i] * input_gain);
        NaiveF32 left_coeff = (left_amp > left_peak) ? attack_coeff : release_coeff;
        NaiveF32 right_coeff = (right_amp > right_peak) ? attack_coeff : release_coeff;
        left_peak = (1 - left_coeff) * left_peak + left_coeff * left_amp;
        right_peak = (1 - right_coeff) * right_peak + right_coeff * right_amp;

        NaiveF32 peak = fmaxf(left_peak, right_peak);
        NaiveF32 target_gain = (peak == 0.0f) ? 1.0f : fminf(1.0f, threshold / peak);

        NaiveF32 coeff;
        if (target_gain < gain) {
            coeff = attack_coeff;
        } else {
            coeff = release_coeff;
        }

        gain = (1 - coeff) * gain + coeff * target_gain;

        out_left[i] *= gain;
        out_right[i] *= gain;
    }
    self->prev_left_peak = left_peak;
    self->prev_right_peak = right_peak;
    self->prev_gain = gain;

    naive_gain(out_left, block_size, output_gain);
    naive_gain(out_right, block_size, output_gain);

    return NAIVE_OK;
}

void naive_stereo_limiter_reset(NaiveStereoLimiter *self)
{
    self->prev_left_peak = 0.0f;
    self->prev_right_peak = 0.0f;
    self->prev_gain = 1.0f;
    naive_delay_buf_reset(&self->left_delay_buf);
    naive_delay_buf_reset(&self->right_delay_buf);
}

NaiveErr naive_stereo_limiter_set_enabled(NaiveStereoLimiter *self, NaiveBool enabled)
{
    self->enabled = enabled;
    return NAIVE_OK;
}

NaiveErr naive_stereo_limiter_set_input_gain(NaiveStereoLimiter *self, NaiveF32 input_gain)
{
    self->input_gain = input_gain;
    return NAIVE_OK;
}

NaiveErr naive_stereo_limiter_set_output_gain(NaiveStereoLimiter *self, NaiveF32 output_gain)
{
    self->output_gain = output_gain;
    return NAIVE_OK;
}

NaiveErr naive_stereo_limiter_set_threshold(NaiveStereoLimiter *self, NaiveF32 threshold)
{
    if (threshold < 0.0f || threshold > 1.0f)
        return NAIVE_ERR_INVALID_PARAMETER;

    self->threshold = threshold;
    return NAIVE_OK;
}

NaiveErr naive_stereo_limiter_set_attack_time(NaiveStereoLimiter *self, NaiveF32 attack_time, NaiveI32 sample_rate)
{
    if (attack_time < 0.0f)
        return NAIVE_ERR_INVALID_PARAMETER;

    self->attack_coeff = 1.0f - expf(logf(9.0f) / (attack_time * (NaiveF32)sample_rate));

    return NAIVE_OK;
}

NaiveErr naive_stereo_limiter_set_release_time(NaiveStereoLimiter *self, NaiveF32 release_time, NaiveI32 sample_rate)
{
    if (release_time < 0.0f)
        return NAIVE_ERR_INVALID_PARAMETER;

    self->release_coeff = 1.0f - expf(logf(9.0f) / (release_time * (NaiveF32)sample_rate));

    return NAIVE_OK;
}

NaiveErr naive_stereo_limiter_set_delay_len(NaiveStereoLimiter *self, NaiveI32 delay_len)
{
    if (delay_len < 0 || delay_len > self->left_delay_buf.size)
        return NAIVE_ERR_INVALID_PARAMETER;

    self->delay_len = delay_len;

    return NAIVE_OK;
}

void naive_stereo_limiter_set_default_params(NaiveStereoLimiter *self, NaiveI32 sample_rate)
{
    self->input_gain = 1.0f;
    self->output_gain = 1.0f;
    self->threshold = 1.0f;
    self->delay_len = 5;
    self->attack_coeff = 1.0f;
    self->release_coeff = 1.0f - expf(-logf(9.0f) / (0.12f * (NaiveF32)sample_rate));
}
