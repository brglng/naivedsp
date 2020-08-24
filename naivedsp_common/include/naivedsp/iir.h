#ifndef __NAIVE_IIR_H__
#define __NAIVE_IIR_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "naivedsp/memory.h"
#include "naivedsp/iir_coeffs.h"
#include "naivedsp/iir_states.h"
#include "naivedsp/math_types.h"

void naive_iir_1st_df1_process(NaiveIir1stDf1States *states, NAIVE_CONST NaiveIir1stCoeffs *params, NaiveF32 *data, NaiveI32 block_size);
void naive_iir_1st_df1_reset(NaiveIir1stDf1States *self);

void naive_iir_2nd_df1_process(NaiveIir2ndDf1States *states, NAIVE_CONST NaiveIir2ndCoeffs *params, NaiveF32 *data, NaiveI32 block_size);
void naive_iir_2nd_df1_reset(NaiveIir2ndDf1States *self);

NAIVE_INLINE NaiveI32 naive_iir_calc_num_sos(NaiveI32 order) {
    return order / 2;
}

NAIVE_INLINE NaiveBool naive_iir_calc_has_fos(NaiveI32 order) {
    return (order - (order / 2) * 2) == 1 ? NAIVE_TRUE : NAIVE_FALSE;
}

typedef struct {
    NaiveI32                num_sos_cap;
    NaiveBool               has_fos;
    NaiveI32                num_sos;
    NaiveIir1stCoeffs       *fos_coeffs;
    NaiveIir2ndCoeffs       *sos_coeffs;
    NaiveIir1stDf1States    *fos_states;
    NaiveIir2ndDf1States    *sos_states;
    NaiveF32                gain;
} NaiveIirDf1;

NaiveErr naive_iir_df1_init(NaiveIirDf1 *self, NaiveAllocFunc alloc, void *alloc_context, NaiveI32 num_sos_limit);

void naive_iir_df1_process(NaiveIirDf1 *self, NaiveF32 *inout, NaiveI32 len);

void naive_iir_df1_reset(NaiveIirDf1 *self);

void naive_iir_df1_set_default_params(NaiveIirDf1 *self);
NaiveErr naive_iir_df1_set_butterworth_lowpass(NaiveIirDf1 *self, NaiveI32 order, NaiveI32 sample_rate, NaiveF32 freq);
NaiveErr naive_iir_df1_set_butterworth_highpass(NaiveIirDf1 *self, NaiveI32 order, NaiveI32 sample_rate, NaiveF32 freq);
NaiveErr naive_iir_df1_set_4th_linkwitz_riley_lowpass(NaiveIirDf1 *self, NaiveI32 sample_rate, NaiveF32 freq);
NaiveErr naive_iir_df1_set_4th_linkwitz_riley_highpass(NaiveIirDf1 *self, NaiveI32 sample_rate, NaiveF32 freq);

#ifdef __cplusplus
}
#endif

#endif /* __NAIVE_IIR_H__ */
