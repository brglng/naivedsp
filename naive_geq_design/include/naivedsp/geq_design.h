#ifndef __NAIVE_GEQ_DESIGN_H__
#define __NAIVE_GEQ_DESIGN_H__

#include "naivedsp/geq_coeffs.h"

NAIVE_INLINE NaiveU32 naive_geq_calc_num_iirs(NaiveU32 num_bands) {
    return (num_bands - 1) * 2;
}

NaiveResult naive_geq_coeffs_init_with_design(NaiveGeqCoeffs *coeffs, NaiveAllocFunc alloc, void *allocator, NaiveU32 max_bands, NaiveU32 max_iir_order);
NaiveResult naive_geq_coeffs_design_butterworth_bands(NaiveGeqCoeffs *coeffs, NaiveU32 order, NaiveU32 sample_rate, NaiveU32 num_bands, NAIVE_CONST NaiveF32 *crossover_freqs);
NaiveResult naive_geq_coeffs_set_band_gain(NaiveGeqCoeffs *coeffs, NaiveU32 band_index, NaiveF32 gain);
NaiveResult naive_geq_coeffs_set_band_gains(NaiveGeqCoeffs *coeffs, NAIVE_CONST NaiveF32 *band_gains);

#endif /* end of include guard: __NAIVE_GEQ_DESIGN_H__ */
