#include <stdio.h>
#include "naivedsp/math.h"
#include "naivedsp/iir_design.h"
#include "naivedsp/peq.h"
#include "naivedsp/peq_design.h"

NaiveResult naive_peq_coeffs_add_butterworth_lowpass(NaivePeqCoeffs *coeffs, NaiveU32 order, NaiveU32 sample_rate, NaiveF32 freq)
{
    if (coeffs->num_bands == coeffs->max_bands)
        return NAIVE_ERR_INVALID_PARAMETER;

    int err = naive_iir_coeffs_set_butterworth_lowpass(&coeffs->band_coeffs[coeffs->num_bands], order, sample_rate, freq);

    if (!err) {
        coeffs->num_bands++;
    }

    return err;
}

NaiveResult naive_peq_coeffs_add_butterworth_highpass(NaivePeqCoeffs *coeffs, NaiveU32 order, NaiveU32 sample_rate, NaiveF32 freq)
{
    if (coeffs->num_bands == coeffs->max_bands)
        return NAIVE_ERR_INVALID_PARAMETER;

    int err = naive_iir_coeffs_set_butterworth_highpass(&coeffs->band_coeffs[coeffs->num_bands], order, sample_rate, freq);

    if (!err) {
        coeffs->num_bands++;
    }

    return err;
}

NaiveResult naive_peq_coeffs_add_4th_linkwitz_riley_lowpass(NaivePeqCoeffs *coeffs, NaiveU32 sample_rate, NaiveF32 freq)
{
    if (coeffs->num_bands == coeffs->max_bands)
        return NAIVE_ERR_INVALID_PARAMETER;

    int err = naive_iir_coeffs_set_4th_linkwitz_riley_lowpass(&coeffs->band_coeffs[coeffs->num_bands], sample_rate, freq);

    if (!err) {
        coeffs->num_bands++;
    }

    return err;
}

NaiveResult naive_peq_coeffs_add_4th_linkwitz_riley_highpass(NaivePeqCoeffs *coeffs, NaiveU32 sample_rate, NaiveF32 freq)
{
    if (coeffs->num_bands == coeffs->max_bands)
        return NAIVE_ERR_INVALID_PARAMETER;

    int err = naive_iir_coeffs_set_4th_linkwitz_riley_highpass(&coeffs->band_coeffs[coeffs->num_bands], sample_rate, freq);

    if (!err) {
        coeffs->num_bands++;
    }

    return err;
}
