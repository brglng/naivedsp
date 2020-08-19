#ifndef __NAIVE_IIR_DESIGN_H__
#define __NAIVE_IIR_DESIGN_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "naivedsp/iir_coeffs.h"
#include "naivedsp/typedefs.h"

NaiveResult naive_iir_1st_coeffs_set_lowpass(NaiveIir1stCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq);
NaiveResult naive_iir_1st_coeffs_set_highpass(NaiveIir1stCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq);
NaiveResult naive_iir_1st_coeffs_set_allpass(NaiveIir1stCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq);

NaiveResult naive_iir_2nd_coeffs_set_lowpass(NaiveIir2ndCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq, NaiveF32 q);
NaiveResult naive_iir_2nd_coeffs_set_highpass(NaiveIir2ndCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq, NaiveF32 q);
NaiveResult naive_iir_2nd_coeffs_set_bandpass(NaiveIir2ndCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq, NaiveF32 q);
NaiveResult naive_iir_2nd_coeffs_set_bandstop(NaiveIir2ndCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq, NaiveF32 q);
NaiveResult naive_iir_2nd_coeffs_set_allpass(NaiveIir2ndCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq, NaiveF32 q);
NaiveResult naive_iir_2nd_coeffs_set_low_shelf(NaiveIir2ndCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq, NaiveF32 q, NaiveF32 gain);
NaiveResult naive_iir_2nd_coeffs_set_high_shelf(NaiveIir2ndCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq, NaiveF32 q, NaiveF32 gain);
NaiveResult naive_iir_2nd_coeffs_set_peak(NaiveIir2ndCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq, NaiveF32 q, NaiveF32 gain);

/** 将一阶模拟滤波器 s 域原型系数经双线性变换为数字低通滤波器 z 域系数，带有频率预畸变
 */
NaiveResult naive_iir_1st_coeffs_set_bilinear_lowpass(NaiveIir1stCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq, NaiveF32 b0, NaiveF32 b1, NaiveF32 a0, NaiveF32 a1);

/** 将一阶模拟滤波器 s 域原型系数经双线性变换为数字高通滤波器 z 域系数，带有频率预畸变
 */
NaiveResult naive_iir_1st_coeffs_set_bilinear_highpass(NaiveIir1stCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq, NaiveF32 b0, NaiveF32 b1, NaiveF32 a0, NaiveF32 a1);

/** 将二阶模拟滤波器 s 域原型系数经双线性变换为数字滤波器 z 域系数，带有频率预畸变
 */
NaiveResult naive_iir_2nd_coeffs_set_bilinear_lowpass(NaiveIir2ndCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq, NaiveF32 b0, NaiveF32 b1, NaiveF32 b2, NaiveF32 a0, NaiveF32 a1, NaiveF32 a2);

/** 将二阶模拟滤波器 s 域原型系数经双线性变换为二阶数字高通滤波器 z 域系数，带有频率预畸变
 */
NaiveResult naive_iir_2nd_coeffs_set_bilinear_highpass(NaiveIir2ndCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq, NaiveF32 b0, NaiveF32 b1, NaiveF32 b2, NaiveF32 a0, NaiveF32 a1, NaiveF32 a2);

/** 将一阶模拟低通滤波器 s 域原型系数经双线性变换为二阶数字带通滤波器 z 域系数，带有频率预畸变
 */
NaiveResult naive_iir_2nd_coeffs_set_bilinear_bandpass(NaiveIir2ndCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq, NaiveF32 q, NaiveF32 b0, NaiveF32 b1, NaiveF32 a0, NaiveF32 a1);

/** 将一阶模拟低通滤波器 s 域原型系数经双线性变换为二阶数字带阻滤波器 z 域系数，带有频率预畸变
 */
NaiveResult naive_iir_2nd_coeffs_set_bilinear_bandstop(NaiveIir2ndCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq, NaiveF32 q, NaiveF32 b0, NaiveF32 b1, NaiveF32 a0, NaiveF32 a1);

NaiveResult naive_iir_1st_coeffs_set_butterworth_lowpass(NaiveIir1stCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq);
NaiveResult naive_iir_1st_coeffs_set_butterworth_highpass(NaiveIir1stCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq);
NaiveResult naive_iir_2nd_coeffs_set_butterworth_lowpass(NaiveIir2ndCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq);
NaiveResult naive_iir_2nd_coeffs_set_butterworth_highpass(NaiveIir2ndCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq);
NaiveResult naive_iir_2nd_coeffs_set_butterworth_bandpass(NaiveIir2ndCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq, NaiveF32 q);
NaiveResult naive_iir_2nd_coeffs_set_butterworth_bandstop(NaiveIir2ndCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq, NaiveF32 q);

NAIVE_INLINE NaiveI32 naive_iir_calc_num_sos(NaiveI32 order) {
    return order / 2;
}

NAIVE_INLINE NaiveBool naive_iir_calc_has_fos(NaiveI32 order) {
    return (order - (order / 2) * 2) == 1 ? NAIVE_TRUE : NAIVE_FALSE;
}

NaiveResult naive_iir_coeffs_set_butterworth_lowpass(NaiveIirCoeffs *coeffs, NaiveI32 order, NaiveI32 sample_rate, NaiveF32 freq);
NaiveResult naive_iir_coeffs_set_butterworth_highpass(NaiveIirCoeffs *coeffs, NaiveI32 order, NaiveI32 sample_rate, NaiveF32 freq);
NaiveResult naive_iir_coeffs_set_4th_linkwitz_riley_lowpass(NaiveIirCoeffs *coeffs, NaiveI32 sample_rate, NaiveF32 freq);
NaiveResult naive_iir_coeffs_set_4th_linkwitz_riley_highpass(NaiveIirCoeffs *coeffs, NaiveI32 sample_rate, NaiveF32 freq);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: __NAIVE_IIR_DESIGN_H__ */
