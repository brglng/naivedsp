#include "naivedsp/math.h"
#include "naivedsp/misc.h"
#include "naivedsp/iir_coeffs.h"

void naive_iir_1st_coeffs_init(NaiveIir1stCoeffs *self)
{
    self->b0 = 1.0f;
    self->b1 = 0;
    self->a1 = 0;
}

NaiveErr naive_iir_1st_coeffs_set_lowpass(NaiveIir1stCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq)
{
    NAIVE_CHECK_SAMPLE_RATE(sample_rate);
    if (freq <= 0 || freq > (NaiveF32)sample_rate / 2) {
        return NAIVE_ERR_INVALID_PARAMETER;
    }
    NaiveF32 k = tanf(NAIVE_PI * freq / (NaiveF32)sample_rate);
    self->b0 = k / (k + 1);
    self->b1 = k / (k + 1);
    self->a1 = (k - 1) / (k + 1);
    return NAIVE_OK;
}

NaiveErr naive_iir_1st_coeffs_set_highpass(NaiveIir1stCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq)
{
    NAIVE_CHECK_SAMPLE_RATE(sample_rate);
    if (freq <= 0 || freq > (NaiveF32)sample_rate / 2) {
        return NAIVE_ERR_INVALID_PARAMETER;
    }
    NaiveF32 k = tanf(NAIVE_PI * freq / (NaiveF32)sample_rate);
    self->b0 = 1 / (k + 1);
    self->b1 = -1 / (k + 1);
    self->a1 = (k - 1) / (k + 1);
    return NAIVE_OK;
}

NaiveErr naive_iir_1st_coeffs_set_allpass(NaiveIir1stCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq)
{
    NAIVE_CHECK_SAMPLE_RATE(sample_rate);
    if (freq <= 0 || freq > (NaiveF32)sample_rate / 2) {
        return NAIVE_ERR_INVALID_PARAMETER;
    }
    NaiveF32 k = tanf(NAIVE_PI * freq / (NaiveF32)sample_rate);
    self->b0 = (k - 1) / (k + 1);
    self->b1 = 1.0f;
    self->a1 = (k - 1) / (k + 1);
    return NAIVE_OK;
}

void naive_iir_2nd_coeffs_init(NaiveIir2ndCoeffs *self)
{
    self->b0 = 1.0f;
    self->b1 = 0;
    self->b2 = 0;
    self->a1 = 0;
    self->a2 = 0;
}

NaiveErr naive_iir_2nd_coeffs_set_lowpass(NaiveIir2ndCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq, NaiveF32 q)
{
    NAIVE_CHECK_SAMPLE_RATE(sample_rate);
    if (freq <= 0 || freq > (NaiveF32)sample_rate / 2) {
        return NAIVE_ERR_INVALID_PARAMETER;
    }
    NaiveF32 k = tanf(NAIVE_PI * freq / (NaiveF32)sample_rate);
    NaiveF32 a0 = k * k * q + k + q;
    self->b0 = k * k * q / a0;
    self->b1 = 2 * k * k * q / a0;
    self->b2 = k * k * q / a0;
    self->a1 = 2 * q * (k * k - 1) / a0;
    self->a2 = (k * k * q - k + q) / a0;
    return NAIVE_OK;
}

NaiveErr naive_iir_2nd_coeffs_set_highpass(NaiveIir2ndCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq, NaiveF32 q)
{
    NAIVE_CHECK_SAMPLE_RATE(sample_rate);
    if (freq <= 0 || freq > (NaiveF32)sample_rate / 2) {
        return NAIVE_ERR_INVALID_PARAMETER;
    }
    NaiveF32 k = tanf(NAIVE_PI * freq / (NaiveF32)sample_rate);
    NaiveF32 a0 = k * k * q + k + q;
    self->b0 = q / a0;
    self->b1 = -2 * q / a0;
    self->b2 = q / a0;
    self->a1 = 2 * q * (k * k - 1) / a0;
    self->a2 = (k * k * q - k + q) / a0;
    return NAIVE_OK;
}

NaiveErr naive_iir_2nd_coeffs_set_bandpass(NaiveIir2ndCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq, NaiveF32 q)
{
    NAIVE_CHECK_SAMPLE_RATE(sample_rate);
    if (freq <= 0 || freq > (NaiveF32)sample_rate / 2) {
        return NAIVE_ERR_INVALID_PARAMETER;
    }
    NaiveF32 k = tanf(NAIVE_PI * freq / (NaiveF32)sample_rate);
    NaiveF32 a0 = k * k * q + k + q;
    self->b0 = k / a0;
    self->b1 = 0;
    self->b2 = -k / a0;
    self->a1 = 2 * q * (k * k - 1) / a0;
    self->a2 = (k * k * q - k + q) / a0;
    return NAIVE_OK;
}

NaiveErr naive_iir_2nd_coeffs_set_bandstop(NaiveIir2ndCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq, NaiveF32 q)
{
    NAIVE_CHECK_SAMPLE_RATE(sample_rate);
    if (freq <= 0 || freq > (NaiveF32)sample_rate / 2) {
        return NAIVE_ERR_INVALID_PARAMETER;
    }
    NaiveF32 k = tanf(NAIVE_PI * freq / (NaiveF32)sample_rate);
    NaiveF32 a0 = k * k * q + k + q;
    self->b0 = q * (1 + k * k) / a0;
    self->b1 = 2 * q * (k * k  - 1) / a0;
    self->b2 = q * (1 + k * k) / a0;
    self->a1 = 2 * q * (k * k - 1) / a0;
    self->a2 = (k * k * q - k + q) / a0;
    return NAIVE_OK;
}

NaiveErr naive_iir_2nd_coeffs_set_allpass(NaiveIir2ndCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq, NaiveF32 q)
{
    NAIVE_CHECK_SAMPLE_RATE(sample_rate);
    if (freq <= 0 || freq > (NaiveF32)sample_rate / 2) {
        return NAIVE_ERR_INVALID_PARAMETER;
    }
    NaiveF32 k = tanf(NAIVE_PI * freq / (NaiveF32)sample_rate);
    NaiveF32 a0 = k * k * q + k + q;
    self->b0 = (k * k * q - k + q) / a0;
    self->b1 = 2 * q * (k * k - 1) / a0;
    self->b2 = 1;
    self->a1 = 2 * q * (k * k - 1) / a0;
    self->a2 = (k * k * q - k + q) / a0;
    return NAIVE_OK;
}

NaiveErr naive_iir_2nd_coeffs_set_low_shelf(NaiveIir2ndCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq, NaiveF32 q, NaiveF32 gain)
{
    NAIVE_CHECK_SAMPLE_RATE(sample_rate);
    if (freq <= 0 || freq > (NaiveF32)sample_rate / 2) {
        return NAIVE_ERR_INVALID_PARAMETER;
    }
    NaiveF32 k = tanf(NAIVE_PI * freq / (NaiveF32)sample_rate);
    if (gain >= 1.0f) {
        NaiveF32 a0 = 1 + k / q + k * k;
        self->b0 = (1 + sqrtf(gain) * k / q + gain * k * k) / a0;
        self->b1 = 2 * (gain * k * k - 1) / a0;
        self->b2 = (1 - sqrtf(gain) * k / q + gain * k * k) / a0;
        self->a1 = 2 * (k * k - 1) / a0;
        self->a2 = (1 - k / q + k * k) / a0;
    } else {
        NaiveF32 a0 = gain + sqrtf(gain) * k / q;
        self->b0 = gain * (1 + k / q + k * k) / a0;
        self->b1 = 2 * gain * (k * k - 1) / a0;
        self->b2 = gain * (1 - k / q + k * k) / a0;
        self->a1 = 2 * (k * k - gain) / a0;
        self->a2 = (gain - sqrtf(gain) * k / q + k * k) / a0;
    }
    return NAIVE_OK;
}

NaiveErr naive_iir_2nd_coeffs_set_high_shelf(NaiveIir2ndCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq, NaiveF32 q, NaiveF32 gain)
{
    NAIVE_CHECK_SAMPLE_RATE(sample_rate);
    if (freq <= 0 || freq > (NaiveF32)sample_rate / 2) {
        return NAIVE_ERR_INVALID_PARAMETER;
    }
    NaiveF32 k = tanf(NAIVE_PI * freq / (NaiveF32)sample_rate);
    if (gain >= 1.0f) {
        NaiveF32 a0 = 1 + k / q + k * k;
        self->b0 = (gain + sqrtf(gain) * k / q + k * k) / a0;
        self->b1 = 2 * (k * k - gain) / a0;
        self->b2 = (gain - sqrtf(gain) * k / q + k * k) / a0;
        self->a1 = 2 * (k * k - 1) / a0;
        self->a2 = (1 - k / q + k * k) / a0;
    } else {
        NaiveF32 a0 = 1 + sqrtf(gain) * k / q + gain * k * k;
        self->b0 = gain * (1 + k / q + k * k) / a0;
        self->b1 = 2 * gain * (k * k - 1) / a0;
        self->b2 = gain * (1 - sqrtf(2) * k + k * k) / a0;
        self->a1 = 2 * (gain * k * k - 1) / a0;
        self->a2 = (1 - sqrtf(2 * gain) * k + gain * k * k) / a0;
    }
    return NAIVE_OK;
}

NaiveErr naive_iir_2nd_coeffs_set_peak(NaiveIir2ndCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq, NaiveF32 q, NaiveF32 gain)
{
    NAIVE_CHECK_SAMPLE_RATE(sample_rate);
    if (freq <= 0 || freq > (NaiveF32)sample_rate / 2) {
        return NAIVE_ERR_INVALID_PARAMETER;
    }
    NaiveF32 k = tanf(NAIVE_PI * freq / (NaiveF32)sample_rate);
    if (gain >= 1.0f) {
        NaiveF32 a0 = 1 + k / q + k * k;
        self->b0 = (1 + gain * k / q + k * k) / a0;
        self->b1 = 2 * (k * k - 1) / a0;
        self->b2 = (1 - gain * k / q + k * k) / a0;
        self->a1 = 2 * (k * k - 1) / a0;
        self->a2 = (1 - k / q + k * k) / a0;
    } else {
        NaiveF32 a0 = 1 + k / (gain * q) + k * k;
        self->b0 = (1 + k / q + k * k) / a0;
        self->b1 = 2 * (k * k - 1) / a0;
        self->b2 = (1 - k / q + k * k) / a0;
        self->a1 = 2 * (k * k - 1) / a0;
        self->a2 = (1 - k / (gain * q) + k * k) / a0;
    }
    return NAIVE_OK;
}

NaiveErr naive_iir_1st_coeffs_set_bilinear_lowpass(NaiveIir1stCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq, NaiveF32 b0, NaiveF32 b1, NaiveF32 a0, NaiveF32 a1)
{
    NAIVE_CHECK_SAMPLE_RATE(sample_rate);
    if (freq <= 0 || freq > (NaiveF32)sample_rate / 2) {
        return NAIVE_ERR_INVALID_PARAMETER;
    }
    NaiveF32 k;
    if (freq * 2 == (NaiveF32)sample_rate) {
        k = 0;
    } else {
        k = 2 * NAIVE_PI * freq / tanf(NAIVE_PI * freq / (NaiveF32)sample_rate);
    }
    NaiveF32 w0 = 2 * NAIVE_PI * freq;
    NaiveF32 z_a0 = k * a0 + a1 * w0;
    self->b0 = (k * b0 + b1 * w0) / z_a0;
    self->b1 = (-k * b0 + b1 * w0) / z_a0;
    self->a1 = (-k * a0 + a1 * w0) / z_a0;
    return NAIVE_OK;
}

NaiveErr naive_iir_1st_coeffs_set_bilinear_highpass(NaiveIir1stCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq, NaiveF32 b0, NaiveF32 b1, NaiveF32 a0, NaiveF32 a1)
{
    NAIVE_CHECK_SAMPLE_RATE(sample_rate);
    if (freq <= 0 || freq > (NaiveF32)sample_rate / 2) {
        return NAIVE_ERR_INVALID_PARAMETER;
    }
    NaiveF32 k;
    if (freq * 2 == (NaiveF32)sample_rate) {
        k = 0;
    } else {
        k = 2 * NAIVE_PI * freq / tanf(NAIVE_PI * freq / (NaiveF32)sample_rate);
    }
    NaiveF32 w0 = 2 * NAIVE_PI * freq;
    NaiveF32 z_a0 = k * a1 + a0 * w0;
    self->b0 = (k * b1 + b0 * w0) / z_a0;
    self->b1 = (-k * b1 + b0 * w0) / z_a0;
    self->a1 = (-k * a1 + a0 * w0) / z_a0;
    return NAIVE_OK;
}

NaiveErr naive_iir_2nd_coeffs_set_bilinear_lowpass(NaiveIir2ndCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq, NaiveF32 b0, NaiveF32 b1, NaiveF32 b2, NaiveF32 a0, NaiveF32 a1, NaiveF32 a2)
{
    NAIVE_CHECK_SAMPLE_RATE(sample_rate);
    if (freq <= 0 || freq > (NaiveF32)sample_rate / 2) {
        return NAIVE_ERR_INVALID_PARAMETER;
    }
    NaiveF32 k;
    if (freq * 2 == (NaiveF32)sample_rate) {
        k = 0;
    } else {
        k = 2 * NAIVE_PI * freq / tanf(NAIVE_PI * freq / (NaiveF32)sample_rate);
    }
    NaiveF32 w0 = 2 * NAIVE_PI * freq;
    NaiveF32 z_a0 = k * k * a0 + k * a1 * w0 + a2 * w0 * w0;
    self->b0 = (k * k * b0 + k * b1 * w0 + b2 * w0 * w0) / z_a0;
    self->b1 = (-2 * k * k * b0 + 2 * b2 * w0 * w0) / z_a0;
    self->b2 = (k * k * b0 - k * b1 * w0 + b2 * w0 * w0) / z_a0;
    self->a1 = (-2 * k * k * a0 + 2 * a2 * w0 * w0) / z_a0;
    self->a2 = (k * k * a0 - k * a1 * w0 + a2 * w0 * w0) / z_a0;
    return NAIVE_OK;
}

NaiveErr naive_iir_2nd_coeffs_set_bilinear_highpass(NaiveIir2ndCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq, NaiveF32 b0, NaiveF32 b1, NaiveF32 b2, NaiveF32 a0, NaiveF32 a1, NaiveF32 a2)
{
    NAIVE_CHECK_SAMPLE_RATE(sample_rate);
    if (freq <= 0 || freq > (NaiveF32)sample_rate / 2) {
        return NAIVE_ERR_INVALID_PARAMETER;
    }
    NaiveF32 k;
    if (freq * 2 == (NaiveF32)sample_rate) {
        k = 0;
    } else {
        k = 2 * NAIVE_PI * freq / tanf(NAIVE_PI * freq / (NaiveF32)sample_rate);
    }
    NaiveF32 w0 = 2 * NAIVE_PI * freq;
    NaiveF32 z_a0 = k * k * a2 + k * a1 * w0 + a0 * w0 * w0;
    self->b0 = (k * k * b2 + k * b1 * w0 + b0 * w0 * w0) / z_a0;
    self->b1 = (-2 * k * k * b2 + 2 * b0 * w0 * w0) / z_a0;
    self->b2 = (k * k * b2 - k * b1 * w0 + b0 * w0 * w0) / z_a0;
    self->a1 = (-2 * k * k * a2 + 2 * a0 * w0 * w0) / z_a0;
    self->a2 = (k * k * a2 - k * a1 * w0 + a0 * w0 * w0) / z_a0;
    return NAIVE_OK;
}

NaiveErr naive_iir_2nd_coeffs_set_bilinear_bandpass(NaiveIir2ndCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq, NaiveF32 q, NaiveF32 b0, NaiveF32 b1, NaiveF32 a0, NaiveF32 a1)
{
    NAIVE_CHECK_SAMPLE_RATE(sample_rate);
    if (freq <= 0 || freq > (NaiveF32)sample_rate / 2) {
        return NAIVE_ERR_INVALID_PARAMETER;
    }
    NaiveF32 k;
    if (freq * 2 == (NaiveF32)sample_rate) {
        k = 0;
    } else {
        k = 2 * NAIVE_PI * freq / tanf(NAIVE_PI * freq / (NaiveF32)sample_rate);
    }
    NaiveF32 w0 = 2 * NAIVE_PI * freq;
    NaiveF32 z_a0 = k * k * q * a0 + k * a1 * w0 + q * a0 * w0 * w0;
    self->b0 = (k * k * q * b0 + k * b1 * w0 + q * b0 * w0 * w0) / z_a0;
    self->b1 = (-2 * k * k * q * b0 + 2 * q * b0 * w0 * w0) / z_a0;
    self->b2 = (k * k * q * b0 - k * b1 * w0 + q * b0 * w0 * w0) / z_a0;
    self->a1 = (-2 * k * k * q * a0 + 2 * q * a0 * w0 * w0) / z_a0;
    self->a2 = (k * k * q * a0 - k * a1 * w0 + q * a0 * w0 * w0) / z_a0;
    return NAIVE_OK;
}

NaiveErr naive_iir_2nd_coeffs_set_bilinear_bandstop(NaiveIir2ndCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq, NaiveF32 q, NaiveF32 b0, NaiveF32 b1, NaiveF32 a0, NaiveF32 a1)
{
    NAIVE_CHECK_SAMPLE_RATE(sample_rate);
    if (freq <= 0 || freq > (NaiveF32)sample_rate / 2) {
        return NAIVE_ERR_INVALID_PARAMETER;
    }
    NaiveF32 k;
    if (freq * 2 == (NaiveF32)sample_rate) {
        k = 0;
    } else {
        k = 2 * NAIVE_PI * freq / tanf(NAIVE_PI * freq / (NaiveF32)sample_rate);
    }
    NaiveF32 w0 = 2 * NAIVE_PI * freq;
    NaiveF32 z_a0 = k * k * q * a1 + k * a0 * w0 + q * a1 * w0 * w0;
    self->b0 = (k * k * q * b1 + k * b0 * w0 + q * b1 * w0 * w0) / z_a0;
    self->b1 = (-2 * k * k * q * b1 + 2 * q * b1 * w0 * w0) / z_a0;
    self->b2 = (k * k * q * b1 - k * b0 * w0 + q * b1 * w0 * w0) / z_a0;
    self->a1 = (-2 * k * k * q * a1 + 2 * q * a1 * w0 * w0) / z_a0;
    self->a2 = (k * k * q * a1 - k * a0 * w0 + q * a1 * w0 * w0) / z_a0;
    return NAIVE_OK;
}

NaiveErr naive_iir_1st_coeffs_set_butterworth_lowpass(NaiveIir1stCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq)
{
    return naive_iir_1st_coeffs_set_bilinear_lowpass(self, sample_rate, freq, 0, 1.0f, 1.0f, 1.0f);
}

NaiveErr naive_iir_1st_coeffs_set_butterworth_highpass(NaiveIir1stCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq)
{
    return naive_iir_1st_coeffs_set_bilinear_highpass(self, sample_rate, freq, 0, 1.0f, 1.0f, 1.0f);
}

NaiveErr naive_iir_2nd_coeffs_set_butterworth_lowpass(NaiveIir2ndCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq)
{

    return naive_iir_2nd_coeffs_set_bilinear_lowpass(self, sample_rate, freq, 0, 0, 1, 1, sqrtf(2.0f), 1);
}

NaiveErr naive_iir_2nd_coeffs_set_butterworth_highpass(NaiveIir2ndCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq)
{
    return naive_iir_2nd_coeffs_set_bilinear_highpass(self, sample_rate, freq, 0, 0, 1, 1, sqrtf(2.0f), 1);
}

NaiveErr naive_iir_2nd_coeffs_set_butterworth_bandpass(NaiveIir2ndCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq, NaiveF32 q)
{
    return naive_iir_2nd_coeffs_set_bilinear_bandpass(self, sample_rate, freq, q, 0, 1, 1, 1);
}

NaiveErr naive_iir_2nd_coeffs_set_butterworth_bandstop(NaiveIir2ndCoeffs *self, NaiveI32 sample_rate, NaiveF32 freq, NaiveF32 q)
{
    return naive_iir_2nd_coeffs_set_bilinear_bandstop(self, sample_rate, freq, q, 0, 1, 1, 1);
}
