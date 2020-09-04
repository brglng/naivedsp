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

NaiveErr naive_iir_df1_init(NaiveIirDf1 *self, NaiveAllocFunc alloc, void *alloc_context, NaiveI32 num_sos_cap)
{
    NaiveErr err = NAIVE_OK;

    naive_iir_1st_df1_states_init(self->fos_states);

    self->num_sos_cap = num_sos_cap;
    self->has_fos = NAIVE_FALSE;
    self->num_sos = 0;
    self->fos_coeffs = NULL;
    self->sos_coeffs = NULL;
    self->fos_states = NULL;
    self->sos_states = NULL;
    self->gain = 1.0f;

    self->fos_coeffs = alloc(alloc_context, NAIVE_MEM_PARAM, sizeof(NaiveIir1stCoeffs));
    if (!err && !self->fos_coeffs)
        err = NAIVE_ERR_NOMEM;

    if (num_sos_cap > 0) {
        self->sos_coeffs = alloc(alloc_context, NAIVE_MEM_PARAM, sizeof(NaiveIir2ndCoeffs));
        if (!err && !self->sos_coeffs)
            err = NAIVE_ERR_NOMEM;
    }

    self->fos_states = alloc(alloc_context, NAIVE_MEM_STATE, sizeof(NaiveIir1stDf1States));
    if (!err && !self->fos_states)
        err = NAIVE_ERR_NOMEM;

    if (num_sos_cap > 0) {
        self->sos_states = alloc(alloc_context, NAIVE_MEM_STATE, sizeof(NaiveIir2ndDf1States) * (NaiveUSize)num_sos_cap);
        if (!err && !self->sos_states)
            err = NAIVE_ERR_NOMEM;
    }

    if (self->fos_coeffs)
        naive_iir_1st_coeffs_init(self->fos_coeffs);

    if (self->sos_coeffs) {
        for (NaiveI32 i = 0; i < num_sos_cap; ++i) {
            naive_iir_2nd_coeffs_init(&self->sos_coeffs[i]);
        }
    }

    if (self->fos_states)
        naive_iir_1st_df1_states_init(self->fos_states);

    if (self->sos_states) {
        for (NaiveI32 i = 0; i < num_sos_cap; ++i) {
            naive_iir_2nd_df1_states_init(&self->sos_states[i]);
        }
    }

    return err;
}

void naive_iir_df1_process(NaiveIirDf1 *self, NaiveF32 *inout, NaiveI32 len)
{
    if (self->has_fos) {
        naive_iir_1st_df1_process(self->fos_states, self->fos_coeffs, inout, len);
    }

    for (NaiveI32 i = 0; i < self->num_sos; ++i) {
        naive_iir_2nd_df1_process(&self->sos_states[i], &self->sos_coeffs[i], inout, len);
    }

    naive_gain(inout, len, self->gain);
}

void naive_iir_df1_reset(NaiveIirDf1 *self)
{
    naive_iir_1st_df1_reset(self->fos_states);

    for (NaiveI32 i = 0; i < self->num_sos; ++i) {
        naive_iir_2nd_df1_reset(&self->sos_states[i]);
    }
}

void naive_iir_df1_set_default_params(NaiveIirDf1 *self) {
    self->has_fos = NAIVE_FALSE;
    self->num_sos = 0;
    naive_iir_1st_coeffs_reset(self->fos_coeffs);
    for (NaiveI32 i = 0; i < self->num_sos_cap; ++i) {
        naive_iir_2nd_coeffs_reset(&self->sos_coeffs[i]);
    }
}

NaiveErr naive_iir_set_butterworth_lowpass(NaiveIirDf1 *self, NaiveI32 order, NaiveI32 sample_rate, NaiveF32 freq)
{
    NaiveErr err = NAIVE_OK;

    NaiveI32 num_sos = order / 2;
    NaiveBool has_fos = (NaiveBool)(order - num_sos * 2);

    if (num_sos > self->num_sos_cap)
        return NAIVE_ERR_INVALID_PARAMETER;

    naive_iir_df1_set_default_params(self);

    if (has_fos) {
        err = naive_iir_1st_coeffs_set_bilinear_lowpass(self->fos_coeffs, sample_rate, freq, 0, 1, 1, 1);
        if (err)
            return err;

        self->has_fos = NAIVE_TRUE;
    }

    for (NaiveI32 i = 0; i < num_sos; ++i) {
        err = naive_iir_2nd_coeffs_set_bilinear_lowpass(&self->sos_coeffs[i], sample_rate, freq, 0, 0, 1, 1, -2 * cosf((2.0f * (NaiveF32)(i + 1) + (NaiveF32)order - 1) / (NaiveF32)(2 * order) * NAIVE_PI), 1);
        if (err)
            return err;
    }

    self->num_sos = num_sos;

    return NAIVE_OK;
}

NaiveErr naive_iir_set_butterworth_highpass(NaiveIirDf1 *self, NaiveI32 order, NaiveI32 sample_rate, NaiveF32 freq)
{
    NaiveErr err = NAIVE_OK;

    NaiveI32 num_sos = order / 2;
    NaiveBool has_fos = (NaiveBool)(order - num_sos * 2);

    if (num_sos > self->num_sos_cap)
        return NAIVE_ERR_INVALID_PARAMETER;

    naive_iir_df1_set_default_params(self);

    if (has_fos) {
        err = naive_iir_1st_coeffs_set_bilinear_highpass(self->fos_coeffs, sample_rate, freq, 0, 1, 1, 1);
        if (err)
            return err;

        self->has_fos = NAIVE_TRUE;
    }

    for (NaiveI32 i = 0; i < num_sos; ++i) {
        err = naive_iir_2nd_coeffs_set_bilinear_highpass(&self->sos_coeffs[i], sample_rate, freq, 0, 0, 1, 1, -2 * cosf((2.0f * (NaiveF32)(i + 1) + (NaiveF32)order - 1) / (NaiveF32)(2 * order) * NAIVE_PI), 1);
        if (err)
            return err;
    }

    self->num_sos = num_sos;

    return NAIVE_OK;
}

NaiveErr naive_iir_set_4th_linkwitz_riley_lowpass(NaiveIirDf1 *self, NaiveI32 sample_rate, NaiveF32 freq)
{
    NaiveErr err = NAIVE_OK;

    if (self->num_sos_cap < 2)
        return NAIVE_ERR_INVALID_PARAMETER;

    naive_iir_df1_set_default_params(self);

    for (NaiveI32 i = 0; i < 2; ++i) {
        err = naive_iir_2nd_coeffs_set_butterworth_lowpass(&self->sos_coeffs[i], sample_rate, freq);
        if (err)
            return err;
    }

    self->num_sos = 2;

    return NAIVE_OK;
}

NaiveErr naive_iir_set_4th_linkwitz_riley_highpass(NaiveIirDf1 *self, NaiveI32 sample_rate, NaiveF32 freq)
{
    NaiveErr err = NAIVE_OK;

    if (self->num_sos_cap < 2)
        return NAIVE_ERR_INVALID_PARAMETER;

    naive_iir_df1_set_default_params(self);

    for (NaiveI32 i = 0; i < 2; ++i) {
        err = naive_iir_2nd_coeffs_set_butterworth_highpass(&self->sos_coeffs[i], sample_rate, freq);
        if (err)
            return err;
    }

    self->num_sos = 2;

    return NAIVE_OK;
}
