#include <stdlib.h>
#include "naivedsp/iir_coeffs.h"

NaiveResult naive_iir_coeffs_init(NaiveIirCoeffs *coeffs, NaiveAllocFunc alloc, void *allocator, NaiveI32 num_sos_limit)
{
    int err = NAIVE_OK;

    coeffs->has_fos = NAIVE_FALSE;
    coeffs->num_sos_limit = num_sos_limit;
    coeffs->num_sos = 0;

    coeffs->fos_coeffs = alloc(allocator, NAIVE_MEM_PARAM, NAIVE_ALIGNOF(NaiveIir1stCoeffs), sizeof(NaiveIir1stCoeffs));

    if (num_sos_limit > 0) {
        coeffs->sos_coeffs = alloc(allocator, NAIVE_MEM_PARAM, NAIVE_ALIGNOF(NaiveIir2ndCoeffs), sizeof(NaiveIir2ndCoeffs) * num_sos_limit);
        if (!err && !coeffs->sos_coeffs)
            err = NAIVE_ERR_NOMEM;
    } else {
        coeffs->sos_coeffs = NULL;
    }

    if (coeffs->fos_coeffs != NULL) {
        naive_iir_1st_coeffs_init(coeffs->fos_coeffs);
    }

    if (coeffs->sos_coeffs) {
        for (NaiveI32 i = 0; i < num_sos_limit; ++i) {
            naive_iir_2nd_coeffs_init(&coeffs->sos_coeffs[i]);
        }
    }

    coeffs->gain = 1.0f;

    return err;
}

void naive_iir_coeffs_reset(NaiveIirCoeffs *coeffs)
{
    coeffs->has_fos = NAIVE_FALSE;
    naive_iir_1st_coeffs_reset(coeffs->fos_coeffs);
    coeffs->num_sos = 0;
    for (NaiveI32 i = 0; i < coeffs->num_sos_limit; ++i) {
        naive_iir_2nd_coeffs_reset(&coeffs->sos_coeffs[i]);
    }
    coeffs->gain = 1.0f;
}
