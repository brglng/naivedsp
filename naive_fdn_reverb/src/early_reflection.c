#include <string.h>
#include "naivedsp/early_reflection.h"
#include "naivedsp/math.h"

static NAIVE_CONST NaiveF32 moorer_times[] = {0, 0.0199f, 0.0354f, 0.0389f, 0.0414f, 0.0699f, 0.0796f};
static NAIVE_CONST NaiveF32 moorer_gains[] = {1.0f, 1.02f, 0.818f, 0.635f, 0.719f, 0.267f, 0.242f};

static NAIVE_CONST NaiveF32 moorer_hall_times[] = {0, 0.0043f, 0.0215f, 0.0225f, 0.0268f, 0.0270f, 0.0298f, 0.0458f, 0.0485f, 0.0572f, 0.0587f, 0.0595f, 0.0612f, 0.0707f, 0.0708f, 0.0726f, 0.0741f, 0.0753f, 0.0797f};
static NAIVE_CONST NaiveF32 moorer_hall_gains[] = {1.0f, 0.841f, 0.504f, 0.491f, 0.379f, 0.380f, 0.346f, 0.289f, 0.272f, 0.192f, 0.193f, 0.217f, 0.181f, 0.180f, 0.181f, 0.176f, 0.142f, 0.167f, 0.134f};

typedef struct {
    NaiveI32                left_taps;
    NaiveI32                right_taps;
    NAIVE_CONST NaiveF32*   left_times;
    NAIVE_CONST NaiveF32*   left_gains;
    NAIVE_CONST NaiveF32*   right_times;
    NAIVE_CONST NaiveF32*   right_gains;
} NaiveEarlyReflectionPreset;

static NAIVE_CONST NaiveEarlyReflectionPreset presets[] = {
    {7, 7, moorer_times, moorer_gains, moorer_times, moorer_gains},
    {19, 19, moorer_hall_times, moorer_hall_gains, moorer_hall_times, moorer_hall_gains},
};

NaiveErr naive_early_reflection_init(NaiveEarlyReflection *self,
                                     void *alloc_context,
                                     NaiveAllocFunc *alloc,
                                     NaiveI32 num_taps_cap,
                                     NaiveI32 delay_len_cap)
{
    NaiveErr err = NAIVE_OK;
    NaiveErr err1 = NAIVE_OK;

    self->num_taps_cap = num_taps_cap;
    self->delay_len_cap = delay_len_cap;
    self->type = NAIVE_EARLY_REFLECTION_TYPE_CUSTOM_DELAY_TAPS;
    self->left_taps = 1;
    self->right_taps = 1;

    self->left_delay_lens = alloc(alloc_context, NAIVE_MEM_PARAM, sizeof(NaiveI32) * (NaiveUSize)num_taps_cap);
    if (self->left_delay_lens) {
        memset(self->left_delay_lens, 0, sizeof(NaiveI32) * (NaiveUSize)num_taps_cap);
    } else if (!err) {
        err = NAIVE_ERR_NOMEM;
    }

    self->left_delay_gains = alloc(alloc_context, NAIVE_MEM_PARAM, sizeof(NaiveF32) * (NaiveUSize)num_taps_cap);
    if (self->left_delay_gains) {
        memset(self->left_delay_gains, 0, sizeof(NaiveI32) * (NaiveUSize)num_taps_cap);
        self->left_delay_gains[0] = 1.0f;
    } else if (!err) {
        err = NAIVE_ERR_NOMEM;
    }

    self->right_delay_lens = alloc(alloc_context, NAIVE_MEM_PARAM, sizeof(NaiveF32) * (NaiveUSize)num_taps_cap);
    if (self->right_delay_lens) {
        memset(self->right_delay_lens, 0, sizeof(NaiveI32) * (NaiveUSize)num_taps_cap);
    } else if (!err) {
        err = NAIVE_ERR_NOMEM;
    }

    self->right_delay_gains = alloc(alloc_context, NAIVE_MEM_PARAM, sizeof(NaiveF32) * (NaiveUSize)num_taps_cap);
    if (self->right_delay_gains) {
        memset(self->right_delay_gains, 0, sizeof(NaiveI32) * (NaiveUSize)num_taps_cap);
        self->right_delay_gains[0] = 1.0f;
    } else if (!err) {
        err = NAIVE_ERR_NOMEM;
    }

    self->left_delay_bufs = alloc(alloc_context, NAIVE_MEM_STATE, sizeof(NaiveDelayBuf) * (NaiveUSize)num_taps_cap);
    if (self->left_delay_bufs) {
        for (NaiveI32 i = 0; i < num_taps_cap; ++i) {
            err1 = naive_delay_buf_init(&self->left_delay_bufs[i], alloc_context, alloc, delay_len_cap);
            if (err1 && !err)
                err = err1;
        }
    } else if (!err) {
        err = NAIVE_ERR_NOMEM;
    }

    self->right_delay_bufs = alloc(alloc_context, NAIVE_MEM_STATE, sizeof(NaiveDelayBuf) * (NaiveUSize)num_taps_cap);
    if (self->right_delay_bufs) {
        for (NaiveI32 i = 0; i < num_taps_cap; ++i) {
            err1 = naive_delay_buf_init(&self->right_delay_bufs[i], alloc_context, alloc, delay_len_cap);
            if (err1 && !err)
                err = err1;
        }
    } else if (!err) {
        err = NAIVE_ERR_NOMEM;
    }

    err1 = naive_delay_buf_init(&self->left_send_delay_buf, alloc_context, alloc, delay_len_cap);
    if (!err && err1)
        err = err1;

    err1 = naive_delay_buf_init(&self->right_send_delay_buf, alloc_context, alloc, delay_len_cap);
    if (!err && err1)
        err = err1;

    self->left_send_len = 0;
    self->right_send_len = 0;
    self->left_send_delay_len = 0;
    self->right_send_delay_len = 0;

    self->left_delay_filters = alloc(alloc_context, NAIVE_MEM_STATE, sizeof(NaiveCascadedIir1stDf1) * (NaiveUSize)num_taps_cap);
    if (self->left_delay_filters != NULL) {
        for (NaiveI32 i = 0; i < num_taps_cap; ++i) {
            err1 = naive_cascaded_iir_1st_df1_init(&self->left_delay_filters[i], alloc_context, alloc, 2);
            if (!err && err1)
                err = err1;
        }
    } else if (!err) {
        err = NAIVE_ERR_NOMEM;
    }

    self->right_delay_filters = alloc(alloc_context, NAIVE_MEM_STATE, sizeof(NaiveCascadedIir1stDf1) * (NaiveUSize)num_taps_cap);
    if (self->right_delay_filters != NULL) {
        for (NaiveI32 i = 0; i < num_taps_cap; ++i) {
            err1 = naive_cascaded_iir_1st_df1_init(&self->right_delay_filters[i], alloc_context, alloc, 2);
            if (!err && err1)
                err = err1;
        }
    } else if (!err) {
        err = NAIVE_ERR_NOMEM;
    }

    return err;
}

void naive_early_reflection_reset(NaiveEarlyReflection *self)
{
    for (NaiveI32 i = 0; i < self->num_taps_cap; ++i) {
        naive_delay_buf_reset(&self->left_delay_bufs[i]);
        naive_delay_buf_reset(&self->right_delay_bufs[i]);
    }
    naive_delay_buf_reset(&self->left_send_delay_buf);
    naive_delay_buf_reset(&self->right_send_delay_buf);
    for (NaiveI32 i = 0; i < self->num_taps_cap; ++i) {
        naive_cascaded_iir_1st_df1_reset(&self->left_delay_filters[i]);
        naive_cascaded_iir_1st_df1_reset(&self->right_delay_filters[i]);
    }
}

NaiveErr naive_early_reflection_process(NaiveEarlyReflection *self,
                                        NaiveF32 *left_in,
                                        NaiveF32 *right_in,
                                        NaiveF32 *left_out,
                                        NaiveF32 *right_out,
                                        NaiveF32 *left_send_out,
                                        NaiveF32 *right_send_out,
                                        NaiveI32 block_size,
                                        void *scratch)
{
    if (block_size < 0)
        return NAIVE_ERR_INVALID_PARAMETER;

    NaiveI32 i, j;
    NaiveF32 *tmp = scratch;
    NaiveF32 *left_send_in = (NaiveF32*)((NaiveUIntPtr)tmp + NAIVE_CEIL_8_BYTES(sizeof(NaiveF32) * block_size));
    NaiveF32 *right_send_in = (NaiveF32*)((NaiveUIntPtr)left_send_in + NAIVE_CEIL_8_BYTES(sizeof(NaiveF32) * block_size));

    memset(left_send_in, 0, sizeof(NaiveF32) * (NaiveUSize)block_size);
    for (i = 0; i < self->left_taps; ++i) {
        if (self->left_delay_lens[i] > self->left_send_len)
            break;

        if (self->left_delay_lens[i] == 0) {
            memcpy(tmp, left_in, sizeof(NaiveF32) * (NaiveUSize)block_size);
        } else {
            NaiveI32 n = 0;
            while (n < block_size && self->left_delay_bufs[i].len < self->left_delay_lens[i]) {
                NaiveI32 len = NAIVE_MIN(self->left_delay_lens[i] - self->left_delay_bufs[i].len, block_size - n);
                memset(&tmp[n], 0, sizeof(NaiveF32) * (NaiveUSize)len);
                naive_delay_buf_write(&self->left_delay_bufs[i], &left_in[n], len);
                n += len;
            }
            while (n < block_size && self->left_delay_bufs[i].len >= self->left_delay_lens[i]) {
                NaiveI32 len = NAIVE_MIN(self->left_delay_lens[i], block_size - n);
                memcpy(&tmp[n], naive_delay_buf_front(&self->left_delay_bufs[i]), sizeof(NaiveF32) * (NaiveUSize)len);
                naive_delay_buf_drain(&self->left_delay_bufs[i], len);
                naive_delay_buf_write(&self->left_delay_bufs[i], &left_in[n], len);
                n += len;
            }
        }

        naive_cascaded_iir_1st_df1_process(&self->left_delay_filters[i], tmp, block_size);
        naive_mix_with_gain(left_send_in, tmp, self->left_delay_gains[i], block_size);
    }

    memset(right_send_in, 0, sizeof(NaiveF32) * (NaiveUSize)block_size);
    for (j = 0; j < self->right_taps; ++j) {
        if (self->right_delay_lens[j] > self->right_send_len)
            break;

        if (self->right_delay_lens[j] == 0) {
            memcpy(tmp, right_in, sizeof(NaiveF32) * (NaiveUSize)block_size);
        } else {
            NaiveI32 n = 0;
            while (n < block_size && self->right_delay_bufs[j].len < self->right_delay_lens[j]) {
                NaiveI32 len = NAIVE_MIN(self->right_delay_lens[j] - self->right_delay_bufs[j].len, block_size - n);
                memset(&tmp[n], 0, sizeof(NaiveF32) * (NaiveUSize)len);
                naive_delay_buf_write(&self->right_delay_bufs[j], &right_in[n], len);
                n += len;
            }
            while (n < block_size && self->right_delay_bufs[j].len >= self->right_delay_lens[j]) {
                NaiveI32 len = NAIVE_MIN(self->right_delay_lens[j], block_size - n);
                memcpy(&tmp[n], naive_delay_buf_front(&self->right_delay_bufs[j]), sizeof(NaiveF32) * (NaiveUSize)len);
                naive_delay_buf_drain(&self->right_delay_bufs[j], len);
                naive_delay_buf_write(&self->right_delay_bufs[j], &right_in[n], len);
                n += len;
            }
        }

        naive_cascaded_iir_1st_df1_process(&self->right_delay_filters[j], tmp, block_size);
        naive_mix_with_gain(right_send_in, tmp, self->right_delay_gains[j], block_size);
    }

    memcpy(left_out, left_send_in, sizeof(NaiveF32) * (NaiveUSize)block_size);
    memcpy(right_out, right_send_in, sizeof(NaiveF32) * (NaiveUSize)block_size);

    for (; i < self->left_taps; ++i) {
        if (self->left_delay_lens[i] == 0) {
            memcpy(tmp, left_in, sizeof(NaiveF32) * (NaiveUSize)block_size);
        } else {
            NaiveI32 n = 0;
            while (n < block_size && self->left_delay_bufs[i].len < self->left_delay_lens[i]) {
                NaiveI32 len = NAIVE_MIN(self->left_delay_lens[i] - self->left_delay_bufs[i].len, block_size - n);
                memset(&tmp[n], 0, sizeof(NaiveF32) * (NaiveUSize)len);
                naive_delay_buf_write(&self->left_delay_bufs[i], &left_in[n], len);
                n += len;
            }
            while (n < block_size && self->left_delay_bufs[i].len >= self->left_delay_lens[i]) {
                NaiveI32 len = NAIVE_MIN(self->left_delay_lens[i], block_size - n);
                memcpy(&tmp[n], naive_delay_buf_front(&self->left_delay_bufs[i]), sizeof(NaiveF32) * (NaiveUSize)len);
                naive_delay_buf_drain(&self->left_delay_bufs[i], len);
                naive_delay_buf_write(&self->left_delay_bufs[i], &left_in[n], len);
                n += len;
            }
        }

        naive_cascaded_iir_1st_df1_process(&self->left_delay_filters[i], tmp, block_size);
        naive_mix_with_gain(left_out, tmp, self->left_delay_gains[i], block_size);
    }

    for (; j < self->right_taps; ++j) {
        if (self->right_delay_lens[j] == 0) {
            memcpy(tmp, right_in, sizeof(NaiveF32) * (NaiveUSize)block_size);
        } else {
            NaiveI32 n = 0;
            while (n < block_size && self->right_delay_bufs[j].len < self->right_delay_lens[j]) {
                NaiveI32 len = NAIVE_MIN(self->right_delay_lens[j] - self->right_delay_bufs[j].len, block_size - n);
                memset(&tmp[n], 0, sizeof(NaiveF32) * (NaiveUSize)len);
                naive_delay_buf_write(&self->right_delay_bufs[j], &right_in[n], len);
                n += len;
            }
            while (n < block_size && self->right_delay_bufs[j].len >= self->right_delay_lens[j]) {
                NaiveI32 len = NAIVE_MIN(self->right_delay_lens[j], block_size - n);
                memcpy(&tmp[n], naive_delay_buf_front(&self->right_delay_bufs[j]), sizeof(NaiveF32) * (NaiveUSize)len);
                naive_delay_buf_drain(&self->right_delay_bufs[j], len);
                naive_delay_buf_write(&self->right_delay_bufs[j], &right_in[n], len);
                n += len;
            }
        }

        naive_cascaded_iir_1st_df1_process(&self->right_delay_filters[j], tmp, block_size);
        naive_mix_with_gain(right_out, tmp, self->right_delay_gains[j], block_size);
    }

    if (self->left_send_delay_len == 0) {
        memcpy(left_send_out, left_send_in, sizeof(NaiveF32) * (NaiveUSize)block_size);
    } else {
        NaiveI32 n = 0;
        while (n < block_size && self->left_send_delay_buf.len < self->left_send_delay_len) {
            NaiveI32 len = NAIVE_MIN(self->left_send_delay_len - self->left_send_delay_buf.len, block_size - n);
            memset(&left_send_out[n], 0, sizeof(NaiveF32) * (NaiveUSize)len);
            naive_delay_buf_write(&self->left_send_delay_buf, &left_send_in[n], len);
            n += len;
        }
        while (n < block_size && self->left_send_delay_buf.len >= self->left_send_delay_len) {
            NaiveI32 len = NAIVE_MIN(self->left_send_delay_len, block_size - n);
            memcpy(&left_send_out[n], naive_delay_buf_front(&self->left_send_delay_buf), sizeof(NaiveF32) * (NaiveUSize)self->left_send_delay_len);
            naive_delay_buf_drain(&self->left_send_delay_buf, len);
            naive_delay_buf_write(&self->left_send_delay_buf, &left_send_in[n], len);
            n += len;
        }
    }

    if (self->right_send_delay_len == 0) {
        memcpy(right_send_out, right_send_in, sizeof(NaiveF32) * (NaiveUSize)block_size);
    } else {
        NaiveI32 n = 0;
        while (n < block_size && self->right_send_delay_buf.len < self->right_send_delay_len) {
            NaiveI32 len = NAIVE_MIN(self->right_send_delay_len - self->right_send_delay_buf.len, block_size - n);
            memset(&right_send_out[n], 0, sizeof(NaiveF32) * (NaiveUSize)len);
            naive_delay_buf_write(&self->right_send_delay_buf, &right_send_in[n], len);
            n += len;
        }
        while (n < block_size && self->right_send_delay_buf.len >= self->right_send_delay_len) {
            NaiveI32 len = NAIVE_MIN(self->right_send_delay_len, block_size - n);
            memcpy(&right_send_out[n], naive_delay_buf_front(&self->right_send_delay_buf), sizeof(NaiveF32) * (NaiveUSize)self->right_send_delay_len);
            naive_delay_buf_drain(&self->right_send_delay_buf, len);
            naive_delay_buf_write(&self->right_send_delay_buf, &right_send_in[n], len);
            n += len;
        }
    }

    return NAIVE_OK;
}

NaiveErr naive_early_reflection_set_preset(NaiveEarlyReflection *self,
                                           NaiveEarlyReflectionType type,
                                           NaiveI32 sample_rate,
                                           NaiveF32 low_mid_xover_freq,
                                           NaiveF32 high_damp_freq,
                                           NaiveF32 dc_reverb_time,
                                           NaiveF32 mid_freq_reverb_time)
{
    if (type < 0 || type >= NAIVE_EARLY_REFLECTION_TYPE_ROOM_SIM)
        return NAIVE_ERR_INVALID_PARAMETER;

    NAIVE_CONST NaiveEarlyReflectionPreset *preset = &presets[type];

    if (preset->left_taps > self->num_taps_cap || preset->right_taps > self->num_taps_cap)
        return NAIVE_ERR_INVALID_PARAMETER;

    self->left_taps = preset->left_taps;
    self->right_taps = preset->right_taps;

    for (NaiveI32 i = 0; i < preset->left_taps; ++i) {
        if (preset->left_times[i] * (NaiveF32)sample_rate + 0.5f > (NaiveF32)self->delay_len_cap)
            return NAIVE_ERR_INVALID_PARAMETER;
    }
    for (NaiveI32 i = 0; i < preset->right_taps; ++i) {
        if (preset->right_times[i] * (NaiveF32)sample_rate + 0.5f > (NaiveF32)self->delay_len_cap)
            return NAIVE_ERR_INVALID_PARAMETER;
    }

    self->type = type;

    for (NaiveI32 i = 0; i < preset->left_taps; ++i) {
        self->left_delay_lens[i] = (NaiveI32)(preset->left_times[i] * (NaiveF32)sample_rate + 0.5f);
    }
    memcpy(self->left_delay_gains, preset->left_gains, sizeof(NaiveF32) * (NaiveUSize)preset->left_taps);
    for (NaiveI32 i = 0; i < preset->right_taps; ++i) {
        self->right_delay_lens[i] = (NaiveI32)(preset->right_times[i] * (NaiveF32)sample_rate + 0.5f);
    }
    memcpy(self->right_delay_gains, preset->right_gains, sizeof(NaiveF32) * (NaiveUSize)preset->right_taps);

    for (NaiveI32 lr = 0; lr < 2; ++lr) {
        NaiveI32 num_taps = (lr == 0) ? self->left_taps : self->right_taps;
        NaiveI32 *delay_lens = (lr == 0) ? self->left_delay_lens : self->right_delay_lens;
        NaiveCascadedIir1stDf1 *delay_filters = (lr == 0) ? self->left_delay_filters : self->right_delay_filters;

        for (NaiveI32 i = 0; i < num_taps; ++i) {
            if (delay_lens[i] == 0) {
                delay_filters[i].coeffs[0].b0 = 1.0f;
                delay_filters[i].coeffs[0].b1 = 0.0f;
                delay_filters[i].coeffs[0].a1 = 0.0f;
                delay_filters[i].num_fos = 1;
            } else {
                NaiveF32 pl = (1 - NAIVE_PI * low_mid_xover_freq / (NaiveF32)sample_rate) / (1 + NAIVE_PI * low_mid_xover_freq / (NaiveF32)sample_rate);
                NaiveF32 g0 = powf(10.0f, -3 * (NaiveF32)delay_lens[i] / (dc_reverb_time * (NaiveF32)sample_rate));
                NaiveF32 gm = powf(10.0f, -3 * (NaiveF32)delay_lens[i] / (mid_freq_reverb_time * (NaiveF32)sample_rate));
                delay_filters[i].coeffs[0].b0 = gm + (g0 - gm) * (1 - pl) / 2.0f;
                delay_filters[i].coeffs[0].b1 = -gm * pl + (g0 - gm) * (1 - pl) / 2.0f;
                delay_filters[i].coeffs[0].a1 = -pl;

                NaiveF32 neg_half_b = (1 - gm * gm * cosf(2 * NAIVE_PI * high_damp_freq / (NaiveF32)sample_rate)) / (1 - gm * gm);
                NaiveF32 ph = neg_half_b - sqrtf(neg_half_b * neg_half_b - 1.0f);
                delay_filters[i].coeffs[1].b0 = 1 - ph;
                delay_filters[i].coeffs[1].b1 = 0;
                delay_filters[i].coeffs[1].a1 = -ph;

                delay_filters[i].num_fos = 2;
            }
        }
    }

    return NAIVE_OK;
}

NaiveErr naive_early_reflection_set_left_send_time(NaiveEarlyReflection *self, NaiveF32 left_send_time, NaiveI32 sample_rate)
{
    if (left_send_time < 0)
        return NAIVE_ERR_INVALID_PARAMETER;

    NaiveI32 left_send_len = (NaiveI32)(left_send_time * (NaiveF32)sample_rate + 0.5f);

    self->left_send_len = left_send_len;

    return NAIVE_OK;
}

NaiveErr naive_early_reflection_set_right_send_time(NaiveEarlyReflection *self, NaiveF32 right_send_time, NaiveI32 sample_rate)
{
    if (right_send_time < 0)
        return NAIVE_ERR_INVALID_PARAMETER;

    NaiveI32 right_send_len = (NaiveI32)(right_send_time * (NaiveF32)sample_rate + 0.5f);

    self->right_send_len = right_send_len;

    return NAIVE_OK;
}

NaiveErr naive_early_reflection_set_left_send_delay_time(NaiveEarlyReflection *self, NaiveF32 left_send_delay_time, NaiveI32 sample_rate)
{
    if (left_send_delay_time < 0)
        return NAIVE_ERR_INVALID_PARAMETER;

    NaiveI32 left_send_delay_len = (NaiveI32)(left_send_delay_time * (NaiveF32)sample_rate + 0.5f);

    if (left_send_delay_len > self->delay_len_cap)
        return NAIVE_ERR_INVALID_PARAMETER;

    self->left_send_delay_len = left_send_delay_len;

    return NAIVE_OK;
}

NaiveErr naive_early_reflection_set_right_send_delay_time(NaiveEarlyReflection *self, NaiveF32 right_send_delay_time, NaiveI32 sample_rate)
{
    if (right_send_delay_time < 0)
        return NAIVE_ERR_INVALID_PARAMETER;

    NaiveI32 right_send_delay_len = (NaiveI32)(right_send_delay_time * (NaiveF32)sample_rate + 0.5f);

    if (right_send_delay_len > self->delay_len_cap)
        return NAIVE_ERR_INVALID_PARAMETER;

    self->right_send_delay_len = right_send_delay_len;

    return NAIVE_OK;
}
