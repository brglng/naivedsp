#ifndef __NAIVE_PEQ_COEFFS_H__
#define __NAIVE_PEQ_COEFFS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "naivedsp/memory.h"
#include "naivedsp/typedefs.h"
#include "naivedsp/iir_coeffs.h"

typedef struct {
    NaiveI32              num_bands_cap;
    NaiveI32              num_bands;
    NaiveIirCoeffs        *band_coeffs;
} NaivePeqCoeffs;

NaiveResult naive_peq_coeffs_init(NaivePeqCoeffs *coeffs, NaiveAllocFunc alloc, void *allocator, NaiveU32 num_bands_cap, NaiveU32 band_sos_cap);
void naive_peq_coeffs_reset(NaivePeqCoeffs *coeffs);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: __NAIVE_PEQ_COEFFS_H__ */
