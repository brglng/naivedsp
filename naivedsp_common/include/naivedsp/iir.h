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

typedef struct {
    NaiveI32                num_fos_cap;
    NaiveI32                num_fos;
    NaiveIir1stCoeffs*      coeffs;
    NaiveIir1stDf1States*   states;
} NaiveCascadedIir1stDf1;

NaiveErr naive_cascaded_iir_1st_df1_init(NaiveCascadedIir1stDf1 *self, void *alloc_context, NaiveAllocFunc alloc, NaiveI32 num_fos_cap);
void naive_cascaded_iir_1st_df1_process(NaiveCascadedIir1stDf1 *self, NaiveF32 *inout, NaiveI32 len);
void naive_cascaded_iir_1st_df1_reset(NaiveCascadedIir1stDf1 *self);
void naive_cascaded_iir_1st_df1_set_default_params(NaiveCascadedIir1stDf1 *self);

typedef struct {
    NaiveI32                num_sos_cap;
    NaiveI32                num_sos;
    NaiveIir2ndCoeffs       *coeffs;
    NaiveIir2ndDf1States    *states;
} NaiveCascadedIir2ndDf1;

NaiveErr naive_cascaded_iir_2nd_df1_init(NaiveCascadedIir2ndDf1 *self, void *alloc_context, NaiveAllocFunc alloc, NaiveI32 num_sos_cap);
void naive_cascaded_iir_2nd_df1_process(NaiveCascadedIir2ndDf1 *self, NaiveF32 *inout, NaiveI32 len);
void naive_cascaded_iir_2nd_df1_reset(NaiveCascadedIir2ndDf1 *self);
void naive_cascaded_iir_2nd_df1_set_default_params(NaiveCascadedIir2ndDf1 *self);
NaiveErr naive_cascaded_iir_2nd_df1_set_butterworth_lowpass(NaiveCascadedIir2ndDf1 *self, NaiveI32 order, NaiveI32 sample_rate, NaiveF32 freq);
NaiveErr naive_cascaded_iir_2nd_df1_set_butterworth_highpass(NaiveCascadedIir2ndDf1 *self, NaiveI32 order, NaiveI32 sample_rate, NaiveF32 freq);
NaiveErr naive_cascaded_iir_2nd_df1_set_4th_linkwitz_riley_lowpass(NaiveCascadedIir2ndDf1 *self, NaiveI32 sample_rate, NaiveF32 freq);
NaiveErr naive_cascaded_iir_2nd_df1_set_4th_linkwitz_riley_highpass(NaiveCascadedIir2ndDf1 *self, NaiveI32 sample_rate, NaiveF32 freq);

typedef struct {
    NaiveCascadedIir1stDf1 fos;
    NaiveCascadedIir2ndDf1 sos;
} NaiveCascadedIirDf1;

NaiveErr naive_cascaded_iir_df1_init(NaiveCascadedIirDf1 *self, void *alloc_context, NaiveAllocFunc alloc, NaiveI32 num_fos_cap, NaiveI32 num_sos_cap);
void naive_cascaded_iir_df1_process(NaiveCascadedIirDf1 *self, NaiveF32 *inout, NaiveI32 len);
void naive_cascaded_iir_df1_reset(NaiveCascadedIirDf1 *self);
void naive_cascaded_iir_df1_set_default_params(NaiveCascadedIirDf1 *self);

#ifdef __cplusplus
}
#endif

#endif /* __NAIVE_IIR_H__ */
