#ifndef __NAIVE_PEQ_COEFFS_H__
#define __NAIVE_PEQ_COEFFS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "naivedsp/inttypes.h"
#include "naivedsp/floattypes.h"
#include "naivedsp/bool.h"
#include "naivedsp/memory.h"
#include "naivedsp/result.h"
#include "naivedsp/iir_coeffs.h"

typedef struct {
    NaiveU32              max_bands;
    NaiveU32              num_bands;
    NaiveIirCoeffs        *band_coeffs;
} NaivePeqCoeffs;

NaiveResult naive_peq_coeffs_init(NaivePeqCoeffs *coeffs, NaiveAllocFunc alloc, void *allocator, NaiveU32 max_bands, NaiveU32 max_band_sos);
void naive_peq_coeffs_reset(NaivePeqCoeffs *coeffs);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: __NAIVE_PEQ_COEFFS_H__ */
