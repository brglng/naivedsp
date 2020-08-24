#ifndef __NAIVE_PEQ_DESIGN_H__
#define __NAIVE_PEQ_DESIGN_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "naivedsp/memory.h"
#include "naivedsp/result.h"
#include "naivedsp/peq_coeffs.h"

NaiveResult naive_peq_coeffs_add_butterworth_lowpass(NaivePeqCoeffs *coeffs, NaiveU32 order, NaiveU32 sample_rate, NaiveF32 freq);
NaiveResult naive_peq_coeffs_add_butterworth_highpass(NaivePeqCoeffs *coeffs, NaiveU32 order, NaiveU32 sample_rate, NaiveF32 freq);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: __NAIVE_PEQ_DESIGN_H__ */
