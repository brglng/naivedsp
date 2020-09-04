#include <string.h>
#include "naivedsp/stereo_delay.h"

NAIVE_INLINE NaiveUSize naive_stereo_delay_scratch_size(NaiveI32 block_size_cap)
{
    return 2 * NAIVE_CEIL_8_BYTES(sizeof(NaiveF32) * (NaiveUSize)block_size_cap);
}

NaiveErr naive_stereo_delay_init(NaiveStereoDelay *self,
                                 void *alloc_context,
                                 NaiveAllocFunc *alloc,
                                 NaiveI32 block_size_cap,
                                 NaiveI32 delay_len_cap)
{
    NaiveErr err = NAIVE_OK;
    NaiveErr err1 = NAIVE_OK;

    self->block_size_cap = block_size_cap;
    self->delay_len_cap = delay_len_cap;

    err1 = naive_delay_buf_init(&self->left_in_delay, alloc_context, alloc, delay_len_cap);
    if (err1) err = err1;

    err1 = naive_delay_buf_init(&self->left_out_delay, alloc_context, alloc, NAIVE_MAX(delay_len_cap, block_size_cap));
    if (err1) err = err1;

    err1 = naive_delay_buf_init(&self->right_in_delay, alloc_context, alloc, delay_len_cap);
    if (err1) err = err1;

    err1 = naive_delay_buf_init(&self->right_out_delay, alloc_context, alloc, NAIVE_MAX(delay_len_cap, block_size_cap));
    if (err1) err = err1;

    self->scratch = alloc(alloc_context, NAIVE_MEM_SCRATCH, naive_stereo_delay_scratch_size(block_size_cap));

    self->left_delay_len = 0;
    self->left_feedback_gain = 0.0f;
    self->left_crossfeed_gain = 0.0f;
    self->left_dry_gain = 1.0f;
    self->left_wet_gain = 0.0f;
    self->right_delay_len = 0;
    self->right_feedback_gain = 0.0f;
    self->right_crossfeed_gain = 0.0f;
    self->right_dry_gain = 1.0f;
    self->right_wet_gain = 0.0f;

    return err;
}

NaiveErr naive_stereo_delay_process(NaiveStereoDelay *self, NaiveF32 *left, NaiveF32 *right, NaiveI32 block_size)
{
    if (block_size < 0 || block_size > self->block_size_cap) {
        return NAIVE_ERR_INVALID_PARAMETER;
    }

    NaiveI32 left_delay_len = self->left_delay_len;
    NaiveF32 left_feedback_gain = self->left_feedback_gain;
    NaiveF32 left_crossfeed_gain = self->left_crossfeed_gain;
    NaiveF32 left_dry_gain = self->left_dry_gain;
    NaiveF32 left_wet_gain = self->left_wet_gain;

    NaiveI32 right_delay_len = self->right_delay_len;
    NaiveF32 right_feedback_gain = self->right_feedback_gain;
    NaiveF32 right_crossfeed_gain = self->right_crossfeed_gain;
    NaiveF32 right_dry_gain = self->right_dry_gain;
    NaiveF32 right_wet_gain = self->right_wet_gain;

    NaiveF32 *left_x = left;
    NaiveF32 *right_x = right;
    NaiveF32 *left_y = self->scratch;
    NaiveF32 *right_y = (NaiveF32 *)((NaiveUIntPtr)left_y + NAIVE_CEIL_8_BYTES(sizeof(NaiveF32) * (NaiveUSize)block_size));

    NaiveF32 *cur_left_x = left_x;
    NaiveF32 *cur_left_y = left_y;
    while (cur_left_x < left + block_size && self->left_in_delay.len < left_delay_len) {
        NaiveI32 len = NAIVE_MIN(
            left_delay_len - self->left_in_delay.len,
            (NaiveI32)(cur_left_x + block_size - left));

        naive_delay_buf_write(&self->left_in_delay, cur_left_x, len);
        memcpy(cur_left_y, cur_left_x, sizeof(NaiveF32) * (NaiveUSize)len);

        cur_left_x += len;
        cur_left_y += len;
    }

    while (cur_left_x < left + block_size && self->left_in_delay.len >= left_delay_len) {
        NaiveI32 len = NAIVE_MIN(left_delay_len, (left + block_size - cur_left_x));

        NaiveF32 *left_x_delayed = naive_delay_buf_front(&self->left_in_delay);
        NaiveF32 *left_y_delayed = naive_delay_buf_front(&self->left_out_delay);

        for (NaiveI32 i = 0; i < len; i++) {
            cur_left_y[i] = left_x_delayed[i] + left_feedback_gain * left_y_delayed[i];
        }

        naive_delay_buf_drain(&self->left_in_delay, len);
        naive_delay_buf_drain(&self->left_out_delay, len);
        naive_delay_buf_write(&self->left_in_delay, cur_left_x, len);

        cur_left_x += len;
        cur_left_y += len;
    }

    NaiveF32 *cur_right_x = right_x;
    NaiveF32 *cur_right_y = right_y;
    while (cur_right_x < right + block_size && self->right_in_delay.len < right_delay_len) {
        NaiveI32 len = NAIVE_MIN(
            right_delay_len - self->right_in_delay.len,
            (NaiveI32)(cur_right_x + block_size - right));

        naive_delay_buf_write(&self->right_in_delay, cur_right_x, len);
        memcpy(cur_right_y, cur_right_x, sizeof(NaiveF32) * (NaiveUSize)len);

        cur_right_x += len;
        cur_right_y += len;
    }

    while (cur_right_x < right + block_size && self->right_in_delay.len >= right_delay_len) {
        NaiveI32 len = NAIVE_MIN(right_delay_len, (right + block_size - cur_right_x));

        NaiveF32 *right_x_delayed = naive_delay_buf_front(&self->right_in_delay);
        NaiveF32 *right_y_delayed = naive_delay_buf_front(&self->right_out_delay);

        for (NaiveI32 i = 0; i < len; i++) {
            cur_right_y[i] = right_x_delayed[i] + right_feedback_gain * right_y_delayed[i];
        }

        naive_delay_buf_drain(&self->right_in_delay, len);
        naive_delay_buf_drain(&self->right_out_delay, len);
        naive_delay_buf_write(&self->right_in_delay, cur_right_x, len);

        cur_right_x += len;
        cur_right_y += len;
    }

    /* crossfeed */
    for (NaiveI32 i = 0; i < block_size; i++) {
        left_y[i] += left_crossfeed_gain * right_y[i];
        right_y[i] += right_crossfeed_gain * left_y[i];
    }

    naive_delay_buf_write(&self->left_out_delay, left_y, block_size);
    naive_delay_buf_write(&self->right_out_delay, right_y, block_size);

    /* dry wet */
    for (NaiveI32 i = 0; i < block_size; i++) {
        left[i] = left_dry_gain * left[i] + left_wet_gain * left_y[i];
        right[i] = right_dry_gain * right[i] + right_wet_gain * right_y[i];
    }

    return NAIVE_OK;
}

NaiveErr naive_stereo_delay_set_left_delay_len(NaiveStereoDelay *self, NaiveI32 len)
{
    if (len < 0 || len > self->delay_len_cap) {
        return NAIVE_ERR_INVALID_PARAMETER;
    }
    if (len < self->left_delay_len) {
        naive_delay_buf_drain(&self->left_in_delay, self->left_delay_len - len);
        naive_delay_buf_drain(&self->left_out_delay, self->left_delay_len - len);
    }
    self->left_delay_len = len;
    return NAIVE_OK;
}

NaiveErr naive_stereo_delay_set_left_feedback_gain(NaiveStereoDelay *self, NaiveF32 gain)
{
    self->left_feedback_gain = gain;
    return NAIVE_OK;
}

NaiveErr naive_stereo_delay_set_left_crossfeed_gain(NaiveStereoDelay *self, NaiveF32 gain)
{
    if (gain < 0.0f) {
        return NAIVE_ERR_INVALID_PARAMETER;
    }
    self->left_crossfeed_gain = gain;
    return NAIVE_OK;
}

NaiveErr naive_stereo_delay_set_left_dry_gain(NaiveStereoDelay *self, NaiveF32 dry_gain)
{
    self->left_dry_gain = dry_gain;
    return NAIVE_OK;
}

NaiveErr naive_stereo_delay_set_left_wet_gain(NaiveStereoDelay *self, NaiveF32 wet_gain)
{
    self->left_wet_gain = wet_gain;
    return NAIVE_OK;
}

NaiveErr naive_stereo_delay_set_right_delay_length(NaiveStereoDelay *self, NaiveI32 len)
{
    if (len < 0 || len > self->delay_len_cap) {
        return NAIVE_ERR_INVALID_PARAMETER;
    }
    if (len < self->right_delay_len) {
        naive_delay_buf_drain(&self->right_in_delay, self->right_delay_len - len);
        naive_delay_buf_drain(&self->right_out_delay, self->right_delay_len - len);
    }
    self->right_delay_len = len;
    return NAIVE_OK;
}

NaiveErr naive_stereo_delay_set_right_feedback_gain(NaiveStereoDelay *self, NaiveF32 gain)
{
    self->right_feedback_gain = gain;
    return NAIVE_OK;
}

NaiveErr naive_stereo_delay_set_right_crossfeed_gain(NaiveStereoDelay *self, NaiveF32 gain)
{
    self->right_crossfeed_gain = gain;
    return NAIVE_OK;
}

NaiveErr naive_stereo_delay_set_right_dry_gain(NaiveStereoDelay *self, NaiveF32 gain)
{
    self->right_dry_gain = gain;
    return NAIVE_OK;
}

NaiveErr naive_stereo_delay_set_right_wet_gain(NaiveStereoDelay *self, NaiveF32 gain)
{
    self->right_wet_gain = gain;
    return NAIVE_OK;
}
