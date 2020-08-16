#include "naivedsp/iir_coeffs.h"

NaiveResult naive_iir_coeffs_init(NaiveIirCoeffs *coeffs, NaiveAllocFunc alloc, void *allocator, NaiveI32 max_num_sos)
{
    int err = NAIVE_OK;

    coeffs->has_fos = NAIVE_FALSE;
    naive_iir_1st_coeffs_init(&coeffs->fos_coeffs);

    coeffs->max_num_sos = max_num_sos;
    coeffs->num_sos = 0;

    if (max_num_sos > 0) {
        coeffs->sos_coeffs = alloc(allocator, NAIVE_MEM_COEFF, NAIVE_ALIGNOF(NaiveIir2ndCoeffs), sizeof(NaiveIir2ndCoeffs) * max_num_sos);
        if (!err && !coeffs->sos_coeffs)
            err = NAIVE_ERR_NOMEM;
    } else {
        coeffs->sos_coeffs = NULL;
    }

    if (coeffs->sos_coeffs) {
        for (NaiveI32 i = 0; i < max_num_sos; ++i) {
            naive_iir_2nd_coeffs_init(&coeffs->sos_coeffs[i]);
        }
    }

    coeffs->gain = 1.0f;

    return err;
}

void naive_iir_coeffs_reset(NaiveIirCoeffs *coeffs)
{
    coeffs->has_fos = NAIVE_FALSE;
    naive_iir_1st_coeffs_reset(&coeffs->fos_coeffs);
    coeffs->num_sos = 0;
    for (NaiveI32 i = 0; i < coeffs->max_num_sos; ++i) {
        naive_iir_2nd_coeffs_init(&coeffs->sos_coeffs[i]);
    }
    coeffs->gain = 1.0f;
}
