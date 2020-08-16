#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "naivedsp/iir.h"
#include "naivedsp/math.h"

void naive_iir_1st_df1_states_init(NaiveIir1stDf1States *self)
{
    self->s0 = 0;
    self->s1 = 0;
}

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

void naive_iir_1st_coeffs_init(NaiveIir1stCoeffs *self)
{
    self->b0 = 1.0f;
    self->b1 = 0;
    self->a1 = 0;
}

void naive_iir_2nd_df1_states_init(NaiveIir2ndDf1States *self)
{
    self->s0 = 0;
    self->s1 = 0;
    self->s2 = 0;
    self->s3 = 0;
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

void naive_iir_2nd_coeffs_init(NaiveIir2ndCoeffs *self)
{
    self->b0 = 1.0f;
    self->b1 = 0;
    self->b2 = 0;
    self->a1 = 0;
    self->a2 = 0;
}

NaiveResult naive_iir_df1_states_init(NaiveIirDf1States *states, NaiveAllocFunc alloc, void *allocator, NaiveI32 max_num_sos)
{
    int err = NAIVE_OK;

    naive_iir_1st_df1_states_init(&states->fos_states);

    states->max_num_sos = max_num_sos;

    if (max_num_sos > 0) {
        states->sos_states = alloc(allocator, NAIVE_MEM_STATE, NAIVE_ALIGNOF(NaiveIir2ndDf1States), sizeof(NaiveIir2ndDf1States) * max_num_sos);
        if (!err && !states->sos_states)
            err = NAIVE_ERR_NOMEM;
    } else {
        states->sos_states = NULL;
    }

    if (states->sos_states) {
        for (NaiveI32 i = 0; i < max_num_sos; ++i) {
            naive_iir_2nd_df1_states_init(&states->sos_states[i]);
        }
    }

    return err;
}

void naive_iir_df1_process(NaiveIirDf1States *states, NAIVE_CONST NaiveIirCoeffs *coeffs, NaiveF32 *inout, NaiveI32 len, void *scratch)
{
    (void)scratch;

    if (coeffs->has_fos) {
        naive_iir_1st_df1_process(&states->fos_states, &coeffs->fos_coeffs, inout, len);
    }

    for (NaiveI32 i = 0; i < coeffs->num_sos; ++i) {
        naive_iir_2nd_df1_process(&states->sos_states[i], &coeffs->sos_coeffs[i], inout, len);
    }

    naive_gain(inout, len, coeffs->gain);
}

void naive_iir_df1_reset(NaiveIirDf1States *states, NAIVE_CONST NaiveIirCoeffs *coeffs)
{
    naive_iir_1st_df1_reset(&states->fos_states);

    for (NaiveI32 i = 0; i < coeffs->num_sos; ++i) {
        naive_iir_2nd_df1_reset(&states->sos_states[i]);
    }
}

NaiveResult naive_iir_df1_set_coeffs(NaiveIirDf1States *states, NaiveIirCoeffs *coeffs, NAIVE_CONST NaiveIirCoeffs *from)
{
    if (from->num_sos > states->max_num_sos)
        return NAIVE_ERR_INVALID_PARAMETER;

    coeffs->has_fos = from->has_fos;
    coeffs->fos_coeffs = from->fos_coeffs;

    for (NaiveI32 i = 0; i < from->num_sos; ++i) {
        coeffs->sos_coeffs[i] = from->sos_coeffs[i];
    }

    for (NaiveI32 i = coeffs->num_sos; i < from->num_sos; ++i) {
        naive_iir_2nd_df1_reset(&states->sos_states[i]);
    }

    coeffs->num_sos = from->num_sos;
    coeffs->gain = from->gain;

    return NAIVE_OK;
}
