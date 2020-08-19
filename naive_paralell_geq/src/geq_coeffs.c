#include "naivedsp/geq_coeffs.h"

NaiveResult naive_geq_coeffs_init(NaiveGeqCoeffs *coeffs, NaiveAllocFunc alloc, void *allocator, NaiveU32 max_num_iirs, NaiveU32 max_iir_sos)
{
    int err = NAIVE_OK;

    coeffs->max_num_iirs = max_num_iirs;
    coeffs->num_iirs = 0;
    coeffs->iir_coeffs = alloc(allocator, NAIVE_MEM_PARAM, NAIVE_ALIGNOF(NaiveIirCoeffs), sizeof(NaiveIirCoeffs) * max_num_iirs);
    if (coeffs->iir_coeffs) {
        for (NaiveU32 i = 0; i < max_num_iirs; ++i) {
            int rc = naive_iir_coeffs_init(&coeffs->iir_coeffs[i], alloc, allocator, max_iir_sos);
            if (!err && rc)
                err = rc;
        }
    }

    return err;
}

void naive_geq_coeffs_reset(NaiveGeqCoeffs *coeffs)
{
    coeffs->num_iirs = 0;
    for (NaiveU32 i = 0; i < coeffs->max_num_iirs; ++i) {
        naive_iir_coeffs_reset(&coeffs->iir_coeffs[i]);
    }
}
