#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "naivedsp/iir.h"
#include "naivedsp/math.h"

void naive_iir_1st_df1_reset(NaiveIir1stDf1States *self)
{
    self->s0 = 0;
    self->s1 = 0;
}

void naive_iir_1st_df1_process(NaiveIir1stDf1States *states, NAIVE_CONST NaiveIir1stCoeffs *params, NaiveF32 *data, NaiveI32 block_size)
{
    NaiveF32 b0 = params->b0;
    NaiveF32 b1 = params->b1;
    NaiveF32 a1 = params->a1;
    NaiveF32 s0 = states->s0; // x[n - 1]
    NaiveF32 s1 = states->s1; // y[n - 1]

    for (NaiveI32 i = 0; i < block_size; i++) {
        NaiveF32 x = data[i];
        NaiveF32 y = b0 * x + b1 * s0 - a1 * s1;
        s0 = x;
        s1 = y;
        data[i] = y;
    }

    states->s0 = s0;
    states->s1 = s1;
}

void naive_iir_2nd_df1_reset(NaiveIir2ndDf1States *self)
{
    self->s0 = 0;
    self->s1 = 0;
    self->s2 = 0;
    self->s3 = 0;
}

void naive_iir_2nd_df1_process(NaiveIir2ndDf1States *states, NAIVE_CONST NaiveIir2ndCoeffs *params, NaiveF32 *data, NaiveI32 block_size)
{
    NaiveF32 b0 = params->b0;
    NaiveF32 b1 = params->b1;
    NaiveF32 b2 = params->b2;
    NaiveF32 a1 = params->a1;
    NaiveF32 a2 = params->a2;
    NaiveF32 s0 = states->s0;
    NaiveF32 s1 = states->s1;
    NaiveF32 s2 = states->s2;
    NaiveF32 s3 = states->s3;

    for (NaiveI32 i = 0; i < block_size; i++) {
        NaiveF32 x = data[i];
        NaiveF32 y = b0 * x + b1 * s0 + b2 * s1 - a1 * s2 - a2 * s3;
        s1 = s0;
        s0 = x;
        s3 = s2;
        s2 = y;
        data[i] = y;
    }

    states->s0 = s0;
    states->s1 = s1;
    states->s2 = s2;
    states->s3 = s3;
}

NaiveErr naive_cascaded_iir_1st_df1_init(NaiveCascadedIir1stDf1 *self, void *alloc_context, NaiveAllocFunc alloc, NaiveI32 num_fos_cap)
{
    NaiveErr err = NAIVE_OK;

    self->num_fos_cap = num_fos_cap;
    self->num_fos = 0;
    self->coeffs = NULL;
    self->states = NULL;
    self->gain = 1.0f;

    if (num_fos_cap > 0) {
        self->coeffs = alloc(alloc_context, NAIVE_MEM_PARAM, sizeof(NaiveIir1stCoeffs) * (NaiveUSize)num_fos_cap);
        if (!err && !self->coeffs)
            err = NAIVE_ERR_NOMEM;
    }

    if (num_fos_cap > 0) {
        self->states = alloc(alloc_context, NAIVE_MEM_STATE, sizeof(NaiveIir1stDf1States) * (NaiveUSize)num_fos_cap);
        if (!err && !self->states)
            err = NAIVE_ERR_NOMEM;
    }

    if (self->coeffs) {
        for (NaiveI32 i = 0; i < num_fos_cap; ++i) {
            naive_iir_1st_coeffs_init(&self->coeffs[i]);
        }
    }

    if (self->states) {
        for (NaiveI32 i = 0; i < num_fos_cap; ++i) {
            naive_iir_1st_df1_states_init(&self->states[i]);
        }
    }

    return err;
}

void naive_cascaded_iir_1st_df1_process(NaiveCascadedIir1stDf1 *self, NaiveF32 *inout, NaiveI32 len)
{
    for (NaiveI32 i = 0; i < self->num_fos; ++i) {
        naive_iir_1st_df1_process(&self->states[i], &self->coeffs[i], inout, len);
    }
    naive_gain(inout, len, self->gain);
}

void naive_cascaded_iir_1st_df1_reset(NaiveCascadedIir1stDf1 *self)
{
    for (NaiveI32 i = 0; i < self->num_fos; ++i) {
        naive_iir_1st_df1_reset(&self->states[i]);
    }
}

void naive_cascaded_iir_1st_df1_set_default_params(NaiveCascadedIir1stDf1 *self)
{
    self->num_fos = 0;
    for (NaiveI32 i = 0; i < self->num_fos_cap; ++i) {
        naive_iir_1st_coeffs_reset(&self->coeffs[i]);
    }
}

NaiveErr naive_cascaded_iir_2nd_df1_init(NaiveCascadedIir2ndDf1 *self, void *alloc_context, NaiveAllocFunc alloc, NaiveI32 num_sos_cap)
{
    NaiveErr err = NAIVE_OK;

    self->num_sos_cap = num_sos_cap;
    self->num_sos = 0;
    self->coeffs = NULL;
    self->states = NULL;
    self->gain = 1.0f;

    if (num_sos_cap > 0) {
        self->coeffs = alloc(alloc_context, NAIVE_MEM_PARAM, sizeof(NaiveIir2ndCoeffs) * (NaiveUSize)num_sos_cap);
        if (!err && !self->coeffs)
            err = NAIVE_ERR_NOMEM;
    }

    if (num_sos_cap > 0) {
        self->states = alloc(alloc_context, NAIVE_MEM_STATE, sizeof(NaiveIir2ndDf1States) * (NaiveUSize)num_sos_cap);
        if (!err && !self->states)
            err = NAIVE_ERR_NOMEM;
    }

    if (self->coeffs) {
        for (NaiveI32 i = 0; i < num_sos_cap; ++i) {
            naive_iir_2nd_coeffs_init(&self->coeffs[i]);
        }
    }

    if (self->states) {
        for (NaiveI32 i = 0; i < num_sos_cap; ++i) {
            naive_iir_2nd_df1_states_init(&self->states[i]);
        }
    }

    return err;
}

void naive_cascaded_iir_2nd_df1_process(NaiveCascadedIir2ndDf1 *self, NaiveF32 *inout, NaiveI32 len)
{
    for (NaiveI32 i = 0; i < self->num_sos; ++i) {
        naive_iir_2nd_df1_process(&self->states[i], &self->coeffs[i], inout, len);
    }
    naive_gain(inout, len, self->gain);
}

void naive_cascaded_iir_2nd_df1_reset(NaiveCascadedIir2ndDf1 *self)
{
    for (NaiveI32 i = 0; i < self->num_sos; ++i) {
        naive_iir_2nd_df1_reset(&self->states[i]);
    }
}

void naive_cascaded_iir_2nd_df1_set_default_params(NaiveCascadedIir2ndDf1 *self)
{
    self->num_sos = 0;
    for (NaiveI32 i = 0; i < self->num_sos_cap; ++i) {
        naive_iir_2nd_coeffs_reset(&self->coeffs[i]);
    }
}

NaiveErr naive_cascaded_iir_2nd_df1_set_butterworth_lowpass(NaiveCascadedIir2ndDf1 *self, NaiveI32 order, NaiveI32 sample_rate, NaiveF32 freq)
{
    NaiveErr err = NAIVE_OK;

    NaiveI32 num_sos = order / 2;

    if (num_sos > self->num_sos_cap)
        return NAIVE_ERR_INVALID_PARAMETER;

    naive_cascaded_iir_2nd_df1_set_default_params(self);

    for (NaiveI32 i = 0; i < num_sos; ++i) {
        err = naive_iir_2nd_coeffs_set_bilinear_lowpass(&self->coeffs[i], sample_rate, freq, 0, 0, 1, 1, -2 * cosf((2.0f * (NaiveF32)(i + 1) + (NaiveF32)order - 1) / (NaiveF32)(2 * order) * NAIVE_PI), 1);
        if (err)
            return err;
    }

    self->num_sos = num_sos;

    return NAIVE_OK;
}

NaiveErr naive_cascaded_iir_2nd_df1_set_butterworth_highpass(NaiveCascadedIir2ndDf1 *self, NaiveI32 order, NaiveI32 sample_rate, NaiveF32 freq)
{
    NaiveErr err = NAIVE_OK;

    NaiveI32 num_sos = order / 2;

    if (num_sos > self->num_sos_cap)
        return NAIVE_ERR_INVALID_PARAMETER;

    naive_cascaded_iir_2nd_df1_set_default_params(self);

    for (NaiveI32 i = 0; i < num_sos; ++i) {
        err = naive_iir_2nd_coeffs_set_bilinear_highpass(&self->coeffs[i], sample_rate, freq, 0, 0, 1, 1, -2 * cosf((2.0f * (NaiveF32)(i + 1) + (NaiveF32)order - 1) / (NaiveF32)(2 * order) * NAIVE_PI), 1);
        if (err)
            return err;
    }

    self->num_sos = num_sos;

    return NAIVE_OK;
}

NaiveErr naive_cascaded_iir_2nd_df1_set_4th_linkwitz_riley_lowpass(NaiveCascadedIir2ndDf1 *self, NaiveI32 sample_rate, NaiveF32 freq)
{
    NaiveErr err = NAIVE_OK;

    if (self->num_sos_cap < 2)
        return NAIVE_ERR_INVALID_PARAMETER;

    naive_cascaded_iir_2nd_df1_set_default_params(self);

    for (NaiveI32 i = 0; i < 2; ++i) {
        err = naive_iir_2nd_coeffs_set_butterworth_lowpass(&self->coeffs[i], sample_rate, freq);
        if (err)
            return err;
    }

    self->num_sos = 2;

    return NAIVE_OK;
}

NaiveErr naive_cascaded_iir_2nd_df1_set_4th_linkwitz_riley_highpass(NaiveCascadedIir2ndDf1 *self, NaiveI32 sample_rate, NaiveF32 freq)
{
    NaiveErr err = NAIVE_OK;

    if (self->num_sos_cap < 2)
        return NAIVE_ERR_INVALID_PARAMETER;

    naive_cascaded_iir_2nd_df1_set_default_params(self);

    for (NaiveI32 i = 0; i < 2; ++i) {
        err = naive_iir_2nd_coeffs_set_butterworth_highpass(&self->coeffs[i], sample_rate, freq);
        if (err)
            return err;
    }

    self->num_sos = 2;

    return NAIVE_OK;
}

NaiveErr naive_cascaded_iir_df1_init(NaiveCascadedIirDf1 *self, void *alloc_context, NaiveAllocFunc alloc, NaiveI32 num_fos_cap, NaiveI32 num_sos_cap)
{
    NaiveErr err = NAIVE_OK;
    NaiveErr err1 = NAIVE_OK;

    err1 = naive_cascaded_iir_1st_df1_init(&self->fos, alloc_context, alloc, num_fos_cap);
    if (!err && err1)
        err = err1;

    err1 = naive_cascaded_iir_2nd_df1_init(&self->sos, alloc_context, alloc, num_sos_cap);
    if (!err && err1)
        err = err1;

    return err;
}

void naive_cascaded_iir_df1_process(NaiveCascadedIirDf1 *self, NaiveF32 *inout, NaiveI32 len)
{
    naive_cascaded_iir_1st_df1_process(&self->fos, inout, len);
    naive_cascaded_iir_2nd_df1_process(&self->sos, inout, len);
}

void naive_cascaded_iir_df1_reset(NaiveCascadedIirDf1 *self)
{
    naive_cascaded_iir_1st_df1_reset(&self->fos);
    naive_cascaded_iir_2nd_df1_reset(&self->sos);
}

void naive_cascaded_iir_df1_set_default_params(NaiveCascadedIirDf1 *self)
{
    naive_cascaded_iir_1st_df1_set_default_params(&self->fos);
    naive_cascaded_iir_2nd_df1_set_default_params(&self->sos);
}
