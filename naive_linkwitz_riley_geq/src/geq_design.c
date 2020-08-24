#include <math.h>
#include "naivedsp/iir_design.h"
#include "naivedsp/geq_design.h"

NaiveResult naive_geq_coeffs_init_with_design(NaiveGeqCoeffs *coeffs, NaiveAllocFunc alloc, void *allocator, NaiveU32 max_bands, NaiveU32 max_peq_order)
{
    return naive_geq_coeffs_init(coeffs, alloc, allocator, naive_geq_calc_num_iirs(max_bands), naive_iir_calc_num_sos(max_peq_order));
}

NaiveResult naive_geq_coeffs_design_butterworth_bands(NaiveGeqCoeffs *coeffs, NaiveU32 order, NaiveU32 sample_rate, NaiveU32 num_bands, NAIVE_CONST NaiveF32 *freqs)
{
    NaiveU32 num_iirs = naive_geq_calc_num_iirs(num_bands);

    if (num_iirs > coeffs->max_num_iirs)
        return NAIVE_ERR_INVALID_PARAMETER;

    if (num_bands < 2)
        return NAIVE_ERR_INVALID_PARAMETER;

    int err = NAIVE_OK;

    // first band, lowpass
    err = naive_iir_coeffs_set_butterworth_lowpass(&coeffs->iir_coeffs[0], order, sample_rate, freqs[0]);
    if (err)
        return err;

    for (NaiveU32 i = 1; i < num_bands - 1; ++i) {
        // middle bands, cascaded highpass and lowpass
        err = naive_iir_coeffs_set_butterworth_highpass(&coeffs->iir_coeffs[2 * i - 1], order, sample_rate, freqs[i - 1]);
        if (err)
            return err;

        err = naive_iir_coeffs_set_butterworth_lowpass(&coeffs->iir_coeffs[2 * i], order, sample_rate, freqs[i]);
        if (err)
            return err;
    }

    // last band, highpass
    err = naive_iir_coeffs_set_butterworth_highpass(&coeffs->iir_coeffs[2 * (num_bands - 1) - 1], order, sample_rate, freqs[num_bands - 2]);
    if (err)
        return err;

    coeffs->num_iirs = num_iirs;

    return NAIVE_OK;
}

NaiveResult naive_geq_coeffs_set_band_gain(NaiveGeqCoeffs *coeffs, NaiveU32 band_index, NaiveF32 gain)
{
    if (band_index * 2 > coeffs->num_iirs)
        return NAIVE_ERR_INVALID_PARAMETER;

    if (band_index == 0) {
        coeffs->iir_coeffs[0].gain = gain;
    } else if (band_index * 2 == coeffs->num_iirs) {
        coeffs->iir_coeffs[coeffs->num_iirs - 1].gain = gain;
    } else {
        NaiveU32 peq_index_highpass = band_index * 2 - 1;
        NaiveU32 peq_index_lowpass = peq_index_highpass + 1;
        coeffs->iir_coeffs[peq_index_highpass].gain = gain;
        coeffs->iir_coeffs[peq_index_lowpass].gain = gain;
    }

    return NAIVE_OK;
}

NaiveResult naive_geq_coeffs_set_band_gains(NaiveGeqCoeffs *coeffs, NAIVE_CONST NaiveF32 *band_gains)
{
    int err = NAIVE_OK;

    NaiveU32 num_bands = coeffs->num_iirs / 2 + 1;

    for (NaiveU32 i = 0; i < num_bands; ++i) {
        err = naive_geq_coeffs_set_band_gain(coeffs, i, band_gains[i]);
        if (err)
            return err;
    }

    return NAIVE_OK;
}
