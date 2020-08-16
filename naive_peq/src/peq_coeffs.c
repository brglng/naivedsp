#include "naivedsp/peq_coeffs.h"

NaiveResult naive_peq_coeffs_init(NaivePeqCoeffs *coeffs, NaiveAllocFunc alloc, void *allocator, NaiveU32 max_bands, NaiveU32 max_band_sos)
{
    int err = NAIVE_OK;

    coeffs->max_bands = max_bands;
    coeffs->num_bands = 0;
    coeffs->band_coeffs = alloc(allocator, NAIVE_MEM_PARAM, NAIVE_ALIGNOF(NaiveIirCoeffs), sizeof(NaiveIirCoeffs) * max_bands);
    if (!err && !coeffs->band_coeffs)
        err = NAIVE_ERR_NOMEM;

    if (coeffs->band_coeffs) {
        for (NaiveU32 i = 0; i < max_bands; ++i) {
            int rc = naive_iir_coeffs_init(&coeffs->band_coeffs[i], alloc, allocator, max_band_sos);
            if (!err && rc)
                err = rc;
        }
    }

    return err;
}

void naive_peq_coeffs_reset(NaivePeqCoeffs *coeffs)
{
    for (NaiveU32 i = 0; i < coeffs->max_bands; ++i) {
        naive_iir_coeffs_reset(&coeffs->band_coeffs[i]);
    }
    coeffs->num_bands = 0;
}
