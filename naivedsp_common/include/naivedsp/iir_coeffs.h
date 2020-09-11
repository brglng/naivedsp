#ifndef __NAIVE_IIR_COEFFS_H__
#define __NAIVE_IIR_COEFFS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "naivedsp/err.h"
#include "naivedsp/memory.h"
#include "naivedsp/platdefs.h"
#include "naivedsp/math_types.h"

typedef struct {
    NaiveF32 b0;
    NaiveF32 b1;
    NaiveF32 a1;
} NaiveIir1stCoeffs;

void naive_iir_1st_coeffs_init(NaiveIir1stCoeffs *self);

NAIVE_INLINE void naive_iir_1st_coeffs_reset(NaiveIir1stCoeffs *self) {
    naive_iir_1st_coeffs_init(self);
}

NaiveErr naive_iir_1st_coeffs_set_lowpass(NaiveIir1stCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq);
NaiveErr naive_iir_1st_coeffs_set_highpass(NaiveIir1stCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq);
NaiveErr naive_iir_1st_coeffs_set_allpass(NaiveIir1stCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq);

/** 将一阶模拟滤波器 s 域原型系数经双线性变换为数字低通滤波器 z 域系数，带有频率预畸变
 */
NaiveErr naive_iir_1st_coeffs_set_bilinear_lowpass(NaiveIir1stCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq, NaiveF32 b0, NaiveF32 b1, NaiveF32 a0, NaiveF32 a1);

/** 将一阶模拟滤波器 s 域原型系数经双线性变换为数字高通滤波器 z 域系数，带有频率预畸变
 */
NaiveErr naive_iir_1st_coeffs_set_bilinear_highpass(NaiveIir1stCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq, NaiveF32 b0, NaiveF32 b1, NaiveF32 a0, NaiveF32 a1);

typedef struct {
    NaiveF32 b0;
    NaiveF32 b1;
    NaiveF32 b2;
    NaiveF32 a1;
    NaiveF32 a2;
} NaiveIir2ndCoeffs;

void naive_iir_2nd_coeffs_init(NaiveIir2ndCoeffs *self);

NAIVE_INLINE void naive_iir_2nd_coeffs_reset(NaiveIir2ndCoeffs *self) {
    naive_iir_2nd_coeffs_init(self);
}

NaiveErr naive_iir_2nd_coeffs_set_lowpass(NaiveIir2ndCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq, NaiveF32 q);
NaiveErr naive_iir_2nd_coeffs_set_highpass(NaiveIir2ndCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq, NaiveF32 q);
NaiveErr naive_iir_2nd_coeffs_set_bandpass(NaiveIir2ndCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq, NaiveF32 q);
NaiveErr naive_iir_2nd_coeffs_set_bandstop(NaiveIir2ndCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq, NaiveF32 q);
NaiveErr naive_iir_2nd_coeffs_set_allpass(NaiveIir2ndCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq, NaiveF32 q);
NaiveErr naive_iir_2nd_coeffs_set_low_shelf(NaiveIir2ndCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq, NaiveF32 q, NaiveF32 gain);
NaiveErr naive_iir_2nd_coeffs_set_high_shelf(NaiveIir2ndCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq, NaiveF32 q, NaiveF32 gain);
NaiveErr naive_iir_2nd_coeffs_set_peak(NaiveIir2ndCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq, NaiveF32 q, NaiveF32 gain);

/** 将二阶模拟滤波器 s 域原型系数经双线性变换为数字滤波器 z 域系数，带有频率预畸变
 */
NaiveErr naive_iir_2nd_coeffs_set_bilinear_lowpass(NaiveIir2ndCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq, NaiveF32 b0, NaiveF32 b1, NaiveF32 b2, NaiveF32 a0, NaiveF32 a1, NaiveF32 a2);

/** 将二阶模拟滤波器 s 域原型系数经双线性变换为二阶数字高通滤波器 z 域系数，带有频率预畸变
 */
NaiveErr naive_iir_2nd_coeffs_set_bilinear_highpass(NaiveIir2ndCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq, NaiveF32 b0, NaiveF32 b1, NaiveF32 b2, NaiveF32 a0, NaiveF32 a1, NaiveF32 a2);

/** 将一阶模拟低通滤波器 s 域原型系数经双线性变换为二阶数字带通滤波器 z 域系数，带有频率预畸变
 */
NaiveErr naive_iir_2nd_coeffs_set_bilinear_bandpass(NaiveIir2ndCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq, NaiveF32 q, NaiveF32 b0, NaiveF32 b1, NaiveF32 a0, NaiveF32 a1);

/** 将一阶模拟低通滤波器 s 域原型系数经双线性变换为二阶数字带阻滤波器 z 域系数，带有频率预畸变
 */
NaiveErr naive_iir_2nd_coeffs_set_bilinear_bandstop(NaiveIir2ndCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq, NaiveF32 q, NaiveF32 b0, NaiveF32 b1, NaiveF32 a0, NaiveF32 a1);

NaiveErr naive_iir_1st_coeffs_set_butterworth_lowpass(NaiveIir1stCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq);
NaiveErr naive_iir_1st_coeffs_set_butterworth_highpass(NaiveIir1stCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq);
NaiveErr naive_iir_2nd_coeffs_set_butterworth_lowpass(NaiveIir2ndCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq);
NaiveErr naive_iir_2nd_coeffs_set_butterworth_highpass(NaiveIir2ndCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq);
NaiveErr naive_iir_2nd_coeffs_set_butterworth_bandpass(NaiveIir2ndCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq, NaiveF32 q);
NaiveErr naive_iir_2nd_coeffs_set_butterworth_bandstop(NaiveIir2ndCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq, NaiveF32 q);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: __NAIVE_IIR_COEFFS_H__ */
