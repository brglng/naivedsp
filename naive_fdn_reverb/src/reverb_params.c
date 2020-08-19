#include "naivedsp/reverb_params.h"

NaiveResult naive_reverb_params_init(
    NaiveReverbParams *params,
    NaiveAllocFunc alloc,
    void *allocator,
    NaiveU32 max_num_delays,
    NaiveU32 wet_eq_max_bands,
    NaiveU32 wet_eq_band_max_sos)
{
    int err = NAIVE_OK;

    params->pre_delay_len = 0;

    params->num_delays_cap = max_num_delays;
    params->num_delays = 0;

    params->delay_lens = alloc(allocator, NAIVE_MEM_PARAM, NAIVE_ALIGNOF(NaiveU32), sizeof(NaiveU32) * max_num_delays);
    if (params->delay_lens) {
        for (NaiveU32 i = 0; i < max_num_delays; ++i) {
            params->delay_lens[i] = 0;
        }
    } else if (!err) {
        err = NAIVE_ERR_NOMEM;
    }

    params->in_gains = alloc(allocator, NAIVE_MEM_PARAM, NAIVE_ALIGNOF(NaiveF32), sizeof(NaiveF32) * max_num_delays);
    if (params->in_gains) {
        for (NaiveU32 i = 0; i < max_num_delays; ++i) {
            params->in_gains[i] = 1.0f;
        }
    } else if (!err) {
        err = NAIVE_ERR_NOMEM;
    }

    params->out_gains = alloc(allocator, NAIVE_MEM_PARAM, NAIVE_ALIGNOF(NaiveF32), sizeof(NaiveF32) * max_num_delays);
    if (params->out_gains) {
        for (NaiveU32 i = 0; i < max_num_delays; ++i) {
            params->out_gains[i] = 1.0f;
        }
    } else if (!err) {
        err = NAIVE_ERR_NOMEM;
    }

    params->delay_eq_coeffs = alloc(allocator, NAIVE_MEM_PARAM, NAIVE_ALIGNOF(NaivePeqCoeffs), sizeof(NaivePeqCoeffs) * max_num_delays);
    if (params->delay_eq_coeffs) {
        for (NaiveU32 i = 0; i < max_num_delays; ++i) {
            int rc = naive_peq_coeffs_init(&params->delay_eq_coeffs[i], alloc, allocator, 2, 0);
            if (!err && rc)
                err = rc;
        }
    } else if (!err) {
        err = NAIVE_ERR_NOMEM;
    }

    params->feedback_mat = alloc(allocator, NAIVE_MEM_PARAM, NAIVE_ALIGNOF(NaiveF32), sizeof(NaiveF32) * max_num_delays * max_num_delays);
    if (params->feedback_mat) {
        for (NaiveU32 i = 0; i < max_num_delays * max_num_delays; ++i) {
            params->feedback_mat[i] = 0;
        }
    } else if (!err) {
        err = NAIVE_ERR_NOMEM;
    }

    int rc = naive_peq_coeffs_init(&params->wet_eq_coeffs, alloc, allocator, wet_eq_max_bands, wet_eq_band_max_sos);
    if (!err && rc)
        err = rc;

    params->dry_gain = 1.0f;

    return err;
}

void naive_reverb_params_reset(NaiveReverbParams *params)
{
    params->pre_delay_len = 0;
    params->num_delays = 0;
    for (NaiveU32 i = 0; i < params->num_delays_cap; ++i) {
        params->delay_lens[i] = 0;
    }
    for (NaiveU32 i = 0; i < params->num_delays_cap; ++i) {
        params->in_gains[i] = 1.0f;
    }
    for (NaiveU32 i = 0; i < params->num_delays_cap; ++i) {
        params->out_gains[i] = 1.0f;
    }
    for (NaiveU32 i = 0; i < params->num_delays_cap; ++i) {
        naive_peq_coeffs_reset(&params->delay_eq_coeffs[i]);
    }
    for (NaiveU32 i = 0; i < params->num_delays_cap * params->num_delays_cap; ++i) {
        params->feedback_mat[i] = 0;
    }
    params->dry_gain = 1.0f;
}
