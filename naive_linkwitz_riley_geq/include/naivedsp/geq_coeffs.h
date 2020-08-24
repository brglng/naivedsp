#ifndef __NAIVE_GEQ_COEFFS_H__
#define __NAIVE_GEQ_COEFFS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "naivedsp/iir_coeffs.h"

typedef struct {
    NaiveU32          max_num_iirs;
    NaiveU32          num_iirs;           /** (num_geq_bands - 1) * 2 */
    NaiveIirCoeffs    *iir_coeffs;
} NaiveGeqCoeffs;

NaiveResult naive_geq_coeffs_init(NaiveGeqCoeffs *coeffs, NaiveAllocFunc alloc, void *allocator, NaiveU32 max_num_iirs, NaiveU32 max_iir_sos);
void naive_geq_coeffs_reset(NaiveGeqCoeffs *coeffs);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: __NAIVE_GEQ_COEFFS_H__ */
