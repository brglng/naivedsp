#include "naivedsp/memory.h"
#include "naivedsp/math.h"
#include "naivedsp/iir.h"
#include "naivedsp/peq.h"

NaiveResult naive_peq_states_init(NaivePeqStates *states, NaiveAllocFunc alloc, void *allocator, NaiveU32 max_bands, NaiveU32 max_band_sos)
{
    int err = NAIVE_OK;

    states->max_bands = max_bands;
    states->band_states = alloc(allocator, NAIVE_MEM_STATE, NAIVE_ALIGNOF(NaiveIirDf1States), sizeof(NaiveIirDf1States) * max_bands);
    if (!err && !states->band_states)
        err = NAIVE_ERR_NOMEM;

    if (states->band_states) {
        for (NaiveU32 i = 0; i < max_bands; ++i) {
            int rc = naive_iir_df1_states_init(&states->band_states[i], alloc, allocator, max_band_sos);
            if (!err && rc)
                err = rc;
        }
    }

    return err;
}

void naive_peq_process(NaivePeqStates *states, NAIVE_CONST NaivePeqCoeffs *coeffs, NaiveF32 *inout, NaiveU32 len, void *scratch)
{
    for (NaiveU32 i = 0; i < coeffs->num_bands; ++i) {
        naive_iir_df1_process(&states->band_states[i], &coeffs->band_coeffs[i], inout, len, scratch);
    }
}

void naive_peq_reset(NaivePeqStates *states, NAIVE_CONST NaivePeqCoeffs *coeffs)
{
    for (NaiveU32 i = 0; i < coeffs->num_bands; ++i) {
        naive_iir_df1_reset(&states->band_states[i], &coeffs->band_coeffs[i]);
    }
}

NaiveResult naive_peq_set_coeffs(NaivePeqStates *states, NaivePeqCoeffs *coeffs, NAIVE_CONST NaivePeqCoeffs *from)
{
    if (from->num_bands > states->max_bands)
        return NAIVE_ERR_INVALID_PARAMETER;

    int err = NAIVE_OK;

    for (NaiveU32 i = 0; i < from->num_bands; ++i) {
        err = naive_iir_df1_set_coeffs(&states->band_states[i], &coeffs->band_coeffs[i], &from->band_coeffs[i]);
        if (err)
            return err;
    }

    for (NaiveU32 i = coeffs->num_bands; i < from->num_bands; ++i) {
        naive_iir_df1_reset(&states->band_states[i], &coeffs->band_coeffs[i]);
    }

    coeffs->num_bands = from->num_bands;

    return NAIVE_OK;
}
