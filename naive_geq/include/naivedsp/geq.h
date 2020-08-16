#ifndef __NAIVE_GEQ_H__
#define __NAIVE_GEQ_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "naivedsp/memory.h"
#include "naivedsp/result.h"
#include "naivedsp/iir.h"
#include "naivedsp/geq_coeffs.h"
#include "naivedsp/geq_states.h"

#define NAIVE_GEQ_SCRATCH_SIZE(block_size) (2 * NAIVE_CEIL_8_BYTES(sizeof(NaiveF32) * (block_size)) + \
                                          NAIVE_CEIL_8_BYTES(NAIVE_IIR_SCRATCH_SIZE(block_size)))

NaiveResult naive_geq_states_init(NaiveGeqStates *states, NaiveAllocFunc alloc, void *allocator, NaiveU32 max_num_iirs, NaiveU32 max_iir_sos);

void naive_geq_process(NaiveGeqStates *states, NAIVE_CONST NaiveGeqCoeffs *coeffs, NaiveF32 *inout, NaiveU32 len, void *scratch);

void naive_geq_reset(NaiveGeqStates *states, NAIVE_CONST NaiveGeqCoeffs *coeffs);

NaiveResult naive_geq_set_coeffs(NaiveGeqStates *states, NaiveGeqCoeffs *coeffs, NAIVE_CONST NaiveGeqCoeffs *from);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: __NAIVE_GEQ_H__ */
