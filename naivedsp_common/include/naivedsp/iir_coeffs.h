#ifndef __NAIVE_IIR_COEFFS_H__
#define __NAIVE_IIR_COEFFS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "naivedsp/memory.h"
#include "naivedsp/platdefs.h"
#include "naivedsp/typedefs.h"

typedef struct {
    NaiveF32 b0;
    NaiveF32 b1;
    NaiveF32 a1;
} NaiveIir1stCoeffs;

void naive_iir_1st_coeffs_init(NaiveIir1stCoeffs *self);

NAIVE_INLINE void naive_iir_1st_coeffs_reset(NaiveIir1stCoeffs *self) {
    naive_iir_1st_coeffs_init(self);
}

typedef struct {
#if NAIVE_ADSP21489
    NaiveF32 a2;
    NaiveF32 a1;
    NaiveF32 b2;
    NaiveF32 b1;
    NaiveF32 b0;
#else
    NaiveF32 b0;
    NaiveF32 b1;
    NaiveF32 b2;
    NaiveF32 a1;
    NaiveF32 a2;
#endif
} NaiveIir2ndCoeffs;

void naive_iir_2nd_coeffs_init(NaiveIir2ndCoeffs *self);

NAIVE_INLINE void naive_iir_2nd_coeffs_reset(NaiveIir2ndCoeffs *self) {
    naive_iir_2nd_coeffs_init(self);
}

typedef struct {
    NaiveBool             has_fos;
    NaiveIir1stCoeffs     fos_coeffs;

    NaiveI32              max_num_sos;
    NaiveI32              num_sos;
    NaiveIir2ndCoeffs     *sos_coeffs;

    NaiveF32              gain;
} NaiveIirCoeffs;

NaiveResult naive_iir_coeffs_init(NaiveIirCoeffs *coeffs, NaiveAllocFunc alloc, void *allocator, NaiveI32 max_num_sos);
void naive_iir_coeffs_reset(NaiveIirCoeffs *coeffs);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: __NAIVE_IIR_COEFFS_H__ */
