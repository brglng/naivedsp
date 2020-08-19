#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "naivedsp/math.h"
#include "naivedsp/memory.h"
#include "naivedsp/reverb.h"
#include "naivedsp/fifo.h"
#include "naivedsp/peq.h"

NaiveResult naive_reverb_states_init(
    NaiveReverbStates *states,
    NaiveAllocFunc alloc,
    void *allocator,
    NaiveU32 pre_delay_len_cap,
    NaiveU32 num_delays_cap,
    NaiveU32 delay_len_cap,
    NaiveU32 wet_eq_max_bands,
    NaiveU32 wet_eq_band_max_sos)
{
    int err = NAIVE_OK;

    int rc = naive_fifo_init(&states->left_pre_delay_buf, alloc, allocator, NAIVE_ALIGNOF(NaiveF32), sizeof(NaiveF32) * pre_delay_len_cap);
    if (!err && rc)
        err = rc;

    rc = naive_fifo_init(&states->right_pre_delay_buf, alloc, allocator, NAIVE_ALIGNOF(NaiveF32), sizeof(NaiveF32) * pre_delay_len_cap);
    if (!err && rc)
        err = rc;

    states->num_delays_cap = num_delays_cap;

    rc = naive_fifo_init(&states->left_in_delay_buf, alloc, allocator, NAIVE_ALIGNOF(NaiveF32), sizeof(NaiveF32) * delay_len_cap);
    if (!err && rc)
        err = rc;

    rc = naive_fifo_init(&states->right_in_delay_buf, alloc, allocator, NAIVE_ALIGNOF(NaiveF32), sizeof(NaiveF32) * delay_len_cap);
    if (!err && rc)
        err = rc;

    states->delay_bufs = alloc(allocator, NAIVE_MEM_STATE, NAIVE_ALIGNOF(NaiveFifo), sizeof(NaiveFifo) * num_delays_cap);
    if (states->delay_bufs) {
        for (NaiveU32 i = 0; i < num_delays_cap; ++i) {
            rc = naive_fifo_init(&states->delay_bufs[i], alloc, allocator, NAIVE_ALIGNOF(NaiveF32), sizeof(NaiveF32) * delay_len_cap);
            if (!err && rc)
                err = rc;
        }
    } else if (!err) {
        err = NAIVE_ERR_NOMEM;
    }

    states->max_delay_len = 0;
    states->min_delay_len = 0;

    states->delay_eq_states = alloc(allocator, NAIVE_MEM_STATE, NAIVE_ALIGNOF(NaivePeqStates), sizeof(NaivePeqStates) * num_delays_cap);
    if (states->delay_eq_states) {
        for (NaiveU32 i = 0; i < num_delays_cap; ++i) {
            rc = naive_peq_states_init(&states->delay_eq_states[i], alloc, allocator, 2, 0);
            if (!err && rc)
                err = rc;
        }
    } else if (!err) {
        err = NAIVE_ERR_NOMEM;
    }

    rc = naive_peq_states_init(&states->left_wet_eq_states, alloc, allocator, wet_eq_max_bands, wet_eq_band_max_sos);
    if (!err && rc)
        err = rc;

    rc = naive_peq_states_init(&states->right_wet_eq_states, alloc, allocator, wet_eq_max_bands, wet_eq_band_max_sos);
    if (!err && rc)
        err = rc;

    return err;
}

void naive_reverb_process(NaiveReverbStates *states, NAIVE_CONST NaiveReverbParams *params, NaiveF32 *left, NaiveF32 *right, NaiveU32 block_size, void *scratch)
{
    NaiveU32 max_delay_len = states->max_delay_len;
    NaiveU32 min_delay_len = states->min_delay_len;
    NaiveU32 N = params->num_delays;
    NaiveF32 *left_predelayed = scratch;
    NaiveF32 *right_predelayed = (NaiveF32 *)((NaiveUIntPtr)left_predelayed + NAIVE_CEIL_8_BYTES(sizeof(NaiveF32) * block_size));
    NaiveF32 *xld = (NaiveF32 *)((NaiveUIntPtr)right_predelayed + NAIVE_CEIL_8_BYTES(sizeof(NaiveF32) * block_size));
    NaiveF32 *xrd = (NaiveF32 *)((NaiveUIntPtr)xld + NAIVE_CEIL_8_BYTES(sizeof(NaiveF32) * max_delay_len));
    NaiveF32 *sd = (NaiveF32 *)((NaiveUIntPtr)xrd + NAIVE_CEIL_8_BYTES(sizeof(NaiveF32) * max_delay_len));
    void *peq_scratch = (void *)((NaiveUIntPtr)sd + NAIVE_CEIL_8_BYTES(sizeof(NaiveF32) * NAIVE_MIN(min_delay_len, block_size)) * N);
    NaiveF32 *s = (NaiveF32 *)((NaiveUIntPtr)peq_scratch + NAIVE_CEIL_8_BYTES(NAIVE_PEQ_SCRATCH_SIZE(NAIVE_MIN(min_delay_len, block_size))));
    void *wet_eq_scratch = (void *)((NaiveUIntPtr)s + NAIVE_CEIL_8_BYTES(sizeof(NaiveF32) * block_size) * N);
    NaiveF32 *A = params->feedback_mat;
    NaiveF32 *b = params->in_gains;
    NaiveF32 *yl = left_predelayed;
    NaiveF32 *yr = right_predelayed;
    NaiveF32 *c = params->out_gains;
    NaiveF32 d = params->dry_gain;

    NaiveU32 n = 0;
    while (n < block_size) {
        NaiveU32 len = NAIVE_MIN(params->pre_delay_len, block_size - n);
        if (len == 0) {
        	break;
        }

        naive_fifo_read(&states->left_pre_delay_buf, &left_predelayed[n], sizeof(NaiveF32) * len);
        naive_fifo_write(&states->left_pre_delay_buf, &left[n], sizeof(NaiveF32) * len);
        naive_fifo_read(&states->right_pre_delay_buf, &right_predelayed[n], sizeof(NaiveF32) * len);
        naive_fifo_write(&states->right_pre_delay_buf, &right[n], sizeof(NaiveF32) * len);
        n += len;
    }

    n = 0;
    while (n < block_size) {
        NaiveU32 len1 = NAIVE_MIN(min_delay_len, block_size - n);

        if (len1 == 0) {
            break;
        }

        /* 每次读 max_delay_len 个点，但只递增 fifo 中的读指针 cur_len 点，保证 xd 中为最近的 max_delay_len 点数据 */
        naive_fifo_peek(&states->left_in_delay_buf, xld, sizeof(NaiveF32) * max_delay_len);
        naive_fifo_drain(&states->left_in_delay_buf, sizeof(NaiveF32) * len1);
        naive_fifo_write(&states->left_in_delay_buf, &left_predelayed[n], sizeof(NaiveF32) * len1);
        naive_fifo_peek(&states->right_in_delay_buf, xrd, sizeof(NaiveF32) * max_delay_len);
        naive_fifo_drain(&states->right_in_delay_buf, sizeof(NaiveF32) * len1);
        naive_fifo_write(&states->right_in_delay_buf, &right_predelayed[n], sizeof(NaiveF32) * len1);

        for (NaiveU32 i = 0; i < N; ++i) {
            NaiveF32 *sdi = (NaiveF32 *)((NaiveUIntPtr)sd + NAIVE_CEIL_8_BYTES(sizeof(NaiveF32) * NAIVE_MIN(min_delay_len, block_size)) * i);
            naive_fifo_read(&states->delay_bufs[i], sdi, sizeof(NaiveF32) * len1);
        }

        for (NaiveU32 i = 0; i < N; i += 2) {
            NaiveF32 *si = (NaiveF32 *)((NaiveUIntPtr)s + NAIVE_CEIL_8_BYTES(sizeof(NaiveF32) * block_size) * i);
            NaiveF32 *xldi = xld + max_delay_len - params->delay_lens[i];

            NaiveU32 len2 = NAIVE_MIN(len1, params->delay_lens[i]);

            for (NaiveU32 nn = 0; nn < len2; ++nn) {
                si[n + nn] = b[i] * xldi[nn];
            }
        }

        for (NaiveU32 i = 1; i < N; i += 2) {
            NaiveF32 *si = (NaiveF32 *)((NaiveUIntPtr)s + NAIVE_CEIL_8_BYTES(sizeof(NaiveF32) * block_size) * i);
            NaiveF32 *xrdi = xrd + max_delay_len - params->delay_lens[i];

            NaiveU32 len2 = NAIVE_MIN(len1, params->delay_lens[i]);

            for (NaiveU32 nn = 0; nn < len2; ++nn) {
                si[n + nn] = b[i] * xrdi[nn];
            }
        }

        for (NaiveU32 i = 0; i < N; ++i) {
            NaiveF32 *si = (NaiveF32 *)((NaiveUIntPtr)s + NAIVE_CEIL_8_BYTES(sizeof(NaiveF32) * block_size) * i);
            NaiveU32 len2 = NAIVE_MIN(len1, params->delay_lens[i]);

            for (NaiveU32 j = 0; j < N; ++j) {
                NaiveF32 *sdj = (NaiveF32 *)((NaiveUIntPtr)sd + NAIVE_CEIL_8_BYTES(sizeof(NaiveF32) * NAIVE_MIN(min_delay_len, block_size)) * j);
                NaiveU32 len3 = NAIVE_MIN(len2, params->delay_lens[j]);
                for (NaiveU32 nn = 0; nn < len3; ++nn) {
                    si[n + nn] += A[N * j + i] * sdj[nn];
                }
            }

            naive_peq_process(&states->delay_eq_states[i], &params->delay_eq_coeffs[i], &si[n], len2, peq_scratch);
            naive_fifo_write(&states->delay_bufs[i], &si[n], sizeof(NaiveF32) * len2);
        }

        n += len1;
    }

    if (min_delay_len > 0) {
        for (NaiveU32 i = 0; i < N; i += 2) {
            NaiveF32 *si = (NaiveF32 *)((NaiveUIntPtr)s + NAIVE_CEIL_8_BYTES(sizeof(NaiveF32) * block_size) * i);
            for (NaiveU32 nn = 0; nn < block_size; ++nn) {
                yl[nn] += c[i] * si[nn];
            }
        }

        for (NaiveU32 i = 1; i < N; i += 2) {
            NaiveF32 *si = (NaiveF32 *)((NaiveUIntPtr)s + NAIVE_CEIL_8_BYTES(sizeof(NaiveF32) * block_size) * i);
            for (NaiveU32 nn = 0; nn < block_size; ++nn) {
                yr[nn] += c[i] * si[nn];
            }
        }
    }

    // naive_peq_process(&states->left_wet_eq_states, &params->wet_eq_coeffs, yl, block_size, wet_eq_scratch);
    // naive_peq_process(&states->right_wet_eq_states, &params->wet_eq_coeffs, yr, block_size, wet_eq_scratch);

    for (NaiveU32 nn = 0; nn < block_size; ++nn) {
        left[nn] = yl[nn] + d * left[nn];
        right[nn] = yr[nn] + d * right[nn];
    }
}

NaiveResult naive_reverb_set_params(
    NaiveReverbStates *states,
    NaiveReverbParams *params,
    NAIVE_CONST NaiveReverbParams *from)
{
    if (from->pre_delay_len > states->left_pre_delay_buf.capacity / sizeof(NaiveF32))
        return NAIVE_ERR_INVALID_PARAMETER;

    if (from->pre_delay_len > params->pre_delay_len) {
        naive_fifo_write_zeros(&states->left_pre_delay_buf, sizeof(NaiveF32) * (from->pre_delay_len - params->pre_delay_len));
        naive_fifo_write_zeros(&states->right_pre_delay_buf, sizeof(NaiveF32) * (from->pre_delay_len - params->pre_delay_len));
    } else if (from->pre_delay_len < params->pre_delay_len) {
        naive_fifo_drain(&states->left_pre_delay_buf, sizeof(NaiveF32) * (params->pre_delay_len - from->pre_delay_len));
        naive_fifo_drain(&states->right_pre_delay_buf, sizeof(NaiveF32) * (params->pre_delay_len - from->pre_delay_len));
    }
    params->pre_delay_len = from->pre_delay_len;

    if (from->num_delays > states->num_delays_cap)
        return NAIVE_ERR_INVALID_PARAMETER;

    NaiveU32 delay_len_cap = states->left_in_delay_buf.capacity / sizeof(NaiveF32);

    NaiveU32 max_delay_len = 0;
    NaiveU32 min_delay_len = delay_len_cap;

    for (NaiveU32 i = 0; i < from->num_delays; ++i) {
        if (from->delay_lens[i] > delay_len_cap)
            return NAIVE_ERR_INVALID_PARAMETER;

        max_delay_len = NAIVE_MAX(max_delay_len, from->delay_lens[i]);
        min_delay_len = NAIVE_MIN(min_delay_len, from->delay_lens[i]);
    }

    if (max_delay_len > states->max_delay_len) {
        naive_fifo_write_zeros(&states->left_in_delay_buf, sizeof(NaiveF32) * (max_delay_len - states->max_delay_len));
        naive_fifo_write_zeros(&states->right_in_delay_buf, sizeof(NaiveF32) * (max_delay_len - states->max_delay_len));
    } else if (max_delay_len < states->max_delay_len) {
        naive_fifo_drain(&states->left_in_delay_buf, sizeof(NaiveF32) * (states->max_delay_len - max_delay_len));
        naive_fifo_drain(&states->right_in_delay_buf, sizeof(NaiveF32) * (states->max_delay_len - max_delay_len));
    }

    states->max_delay_len = max_delay_len;
    states->min_delay_len = min_delay_len;

    if (from->num_delays > params->num_delays) {
        for (NaiveU32 i = params->num_delays; i < from->num_delays; ++i) {
            naive_fifo_reset(&states->delay_bufs[i]);
        }
    }

    for (NaiveU32 i = 0; i < from->num_delays; ++i) {
        if (from->delay_lens[i] > params->delay_lens[i]) {
            naive_fifo_write_zeros(&states->delay_bufs[i], sizeof(NaiveF32) * (from->delay_lens[i] - params->delay_lens[i]));
        } else if (from->delay_lens[i] < params->delay_lens[i]) {
            naive_fifo_drain(&states->delay_bufs[i], sizeof(NaiveF32) * (params->delay_lens[i] - from->delay_lens[i]));
        }
        params->delay_lens[i] = from->delay_lens[i];
    }

    memcpy(params->in_gains, from->in_gains, sizeof(NaiveF32) * from->num_delays);
    memcpy(params->out_gains, from->out_gains, sizeof(NaiveF32) * from->num_delays);
    memcpy(params->feedback_mat, from->feedback_mat, sizeof(NaiveF32) * from->num_delays * from->num_delays);

    if (from->num_delays > params->num_delays) {
        for (NaiveU32 i = params->num_delays; i < from->num_delays; ++i) {
            naive_peq_reset(&states->delay_eq_states[i], &params->delay_eq_coeffs[i]);
        }
    }

    int err = 0;

    for (NaiveU32 i = 0; i < from->num_delays; ++i) {
        err = naive_peq_set_coeffs(&states->delay_eq_states[i], &params->delay_eq_coeffs[i], &from->delay_eq_coeffs[i]);
        if (err)
            return err;
    }

    err = naive_peq_set_coeffs(&states->left_wet_eq_states, &params->wet_eq_coeffs, &from->wet_eq_coeffs);
    if (err)
        return err;

    err = naive_peq_set_coeffs(&states->right_wet_eq_states, &params->wet_eq_coeffs, &from->wet_eq_coeffs);
    if (err)
        return err;

    params->num_delays = from->num_delays;

    params->dry_gain = from->dry_gain;

    return NAIVE_OK;
}
