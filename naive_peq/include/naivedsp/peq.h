#ifndef __NAIVE_PEQ_H__
#define __NAIVE_PEQ_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "naivedsp/platdefs.h"
#include "naivedsp/memory.h"
#include "naivedsp/iir.h"
#include "naivedsp/peq_coeffs.h"
#include "naivedsp/peq_states.h"

#define NAIVE_PEQ_SCRATCH_SIZE(block_size) NAIVE_IIR_SCRATCH_SIZE(block_size)

NaiveResult naive_peq_states_init(NaivePeqStates *states, NaiveAllocFunc alloc, void *allocator, NaiveI32 num_bands_cap, NaiveU32 band_num_sos_cap);

void naive_peq_process(NaivePeqStates *states, NAIVE_CONST NaivePeqCoeffs *coeffs, NaiveF32 *inout, NaiveI32 len, void *scratch);

void naive_peq_reset(NaivePeqStates *self, NAIVE_CONST NaivePeqCoeffs *coeffs);

NaiveResult naive_peq_set_coeffs(NaivePeqStates *states, NaivePeqCoeffs *coeffs, NAIVE_CONST NaivePeqCoeffs *from);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: __NAIVE_PEQ_H__ */
