#include <string.h>
#include "naivedsp/assert.h"
#include "naivedsp/memory.h"
#include "naivedsp/math.h"
#include "naivedsp/geq.h"

NaiveResult naive_geq_states_init(NaiveGeqStates *states, NaiveAllocFunc alloc, void *allocator, NaiveU32 max_num_iirs, NaiveU32 max_iir_sos)
{
    int err = NAIVE_OK;

    if (max_num_iirs < 2)
        return NAIVE_ERR_INVALID_PARAMETER;

    states->max_num_iirs = max_num_iirs;
    states->iir_states = alloc(allocator, NAIVE_MEM_STATE, NAIVE_ALIGNOF(NaiveIirDf1States), sizeof(NaiveIirDf1States) * max_num_iirs);
    if (states->iir_states) {
        for (NaiveU32 i = 0; i < max_num_iirs; ++i) {
            int rc = naive_iir_df1_states_init(&states->iir_states[i], alloc, allocator, max_iir_sos);
            if (!err && rc)
                err = rc;
        }
    }

    return err;
}

void naive_geq_process(NaiveGeqStates *states, NAIVE_CONST NaiveGeqCoeffs *coeffs, NaiveF32 *inout, NaiveU32 len, void *scratch)
{
    NAIVE_ASSERT(coeffs->num_iirs >= 2 && ((coeffs->num_iirs >> 1) << 1) == coeffs->num_iirs);

    NaiveF32 *iir_inout = scratch;
    NaiveF32 *iir_scratch = (NaiveF32 *)((NaiveUIntPtr)iir_inout + NAIVE_CEIL_8_BYTES(sizeof(NaiveF32) * len));
    NaiveF32 *output = (NaiveF32 *)((NaiveUIntPtr)iir_scratch + NAIVE_CEIL_8_BYTES(NAIVE_IIR_SCRATCH_SIZE(len)));
    memset(output, 0, sizeof(NaiveF32) * len);

    /* first band lowpass */
    memcpy(iir_inout, inout, sizeof(NaiveF32) * len);
    naive_iir_df1_process(&states->iir_states[0], &coeffs->iir_coeffs[0], iir_inout, len, iir_scratch);
    naive_mix(output, iir_inout, len);

    /* middle bands */
    if (coeffs->num_iirs > 2) {
        for (NaiveU32 i = 1; i < coeffs->num_iirs - 1; i+=2) {
            memcpy(iir_inout, inout, sizeof(NaiveF32) * len);
            naive_iir_df1_process(&states->iir_states[i], &coeffs->iir_coeffs[i], iir_inout, len, iir_scratch);
            naive_iir_df1_process(&states->iir_states[i + 1], &coeffs->iir_coeffs[i + 1], iir_inout, len, iir_scratch);
            naive_mix(output, iir_inout, len);
        }
    }

    /* last band highpass */
    memcpy(iir_inout, inout, sizeof(NaiveF32) * len);
    naive_iir_df1_process(&states->iir_states[coeffs->num_iirs - 1], &coeffs->iir_coeffs[coeffs->num_iirs - 1], iir_inout, len, iir_scratch);
    naive_mix(output, iir_inout, len);

    memcpy(inout, output, sizeof(NaiveF32) * len);
}

void naive_geq_reset(NaiveGeqStates *states, NAIVE_CONST NaiveGeqCoeffs *coeffs)
{
    for (NaiveU32 i = 0; i < coeffs->num_iirs; ++i) {
        naive_iir_df1_reset(&states->iir_states[i], &coeffs->iir_coeffs[i]);
    }
}

NaiveResult naive_geq_set_coeffs(NaiveGeqStates *states, NaiveGeqCoeffs *coeffs, NAIVE_CONST NaiveGeqCoeffs *from)
{
    if (from->num_iirs > states->max_num_iirs)
        return NAIVE_ERR_INVALID_PARAMETER;

    if (((from->num_iirs >> 1) << 1) != from->num_iirs)
        return NAIVE_ERR_INVALID_PARAMETER;

    for (NaiveU32 i = 0; i < from->num_iirs; ++i) {
        int err = naive_iir_df1_set_coeffs(&states->iir_states[i], &coeffs->iir_coeffs[i], &from->iir_coeffs[i]);
        if (err)
            return err;
    }

    for (NaiveU32 i = coeffs->num_iirs; i < from->num_iirs; ++i) {
        naive_iir_df1_reset(&states->iir_states[i], &coeffs->iir_coeffs[i]);
    }

    coeffs->num_iirs = from->num_iirs;

    return NAIVE_OK;
}
