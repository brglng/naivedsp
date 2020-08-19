#ifndef __NAIVE_IIR_H__
#define __NAIVE_IIR_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "naivedsp/memory.h"
#include "naivedsp/iir_coeffs.h"
#include "naivedsp/iir_states.h"
#include "naivedsp/typedefs.h"

void naive_iir_1st_df1_states_init(NaiveIir1stDf1States *self);
void naive_iir_1st_df1_process(NaiveIir1stDf1States *states, NAIVE_CONST NaiveIir1stCoeffs *params, NaiveF32 *data, NaiveI32 block_size);
void naive_iir_1st_df1_reset(NaiveIir1stDf1States *self);

void naive_iir_2nd_df1_states_init(NaiveIir2ndDf1States *self);
void naive_iir_2nd_df1_process(NaiveIir2ndDf1States *states, NAIVE_CONST NaiveIir2ndCoeffs *params, NaiveF32 *data, NaiveI32 block_size);
void naive_iir_2nd_df1_reset(NaiveIir2ndDf1States *self);

#if NAIVE_ADSP21489
#define NAIVE_IIR_SCRATCH_SIZE(block_size) (sizeof(NaiveF32) * (block_size))
#else
#define NAIVE_IIR_SCRATCH_SIZE(block_size) 0
#endif

NaiveResult naive_iir_df1_states_init(NaiveIirDf1States *states, NaiveAllocFunc alloc, void *allocator, NaiveI32 num_sos_limit);

void naive_iir_df1_process(NaiveIirDf1States *states, NAIVE_CONST NaiveIirCoeffs *coeffs, NaiveF32 *inout, NaiveI32 len, void *scratch);

void naive_iir_df1_reset(NaiveIirDf1States *states, NAIVE_CONST NaiveIirCoeffs *coeffs);

NaiveResult naive_iir_df1_set_coeffs(NaiveIirDf1States *states, NaiveIirCoeffs *coeffs, NAIVE_CONST NaiveIirCoeffs *from);

#ifdef __cplusplus
}
#endif

#endif /* __NAIVE_IIR_H__ */
