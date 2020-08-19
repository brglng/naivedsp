#include <string.h>
#include "naivedsp/memory.h"
#include "naivedsp/delay.h"
#include "naivedsp/delay_buf.h"
#include "naivedsp/math.h"

NaiveResult naive_delay_states_init(NaiveDelayStates *self, NaiveAllocFunc *alloc, void *allocator, NaiveI32 delay_len_cap)
{
    int rc = NAIVE_OK, err = NAIVE_OK;

    err = naive_delay_buf_init(&self->in_delay_buf, alloc, allocator, delay_len_cap);
    if (err) rc = err;

    err = naive_delay_buf_init(&self->out_delay_buf, alloc, allocator, delay_len_cap);
    if (err) rc = err;

    self->delay_len_cap = delay_len_cap;

    return rc;
}

void naive_delay_reset(NaiveDelayStates *states)
{
    naive_delay_buf_reset(&states->in_delay_buf);
    naive_delay_buf_reset(&states->out_delay_buf);
}

void naive_delay_process(NaiveDelayStates *states, NAIVE_CONST NaiveDelayParams *params, NaiveF32 *inout, NaiveI32 block_size, void *scratch)
{
    NaiveI32 delay_len = params->delay_len;
    NaiveF32 feedback_gain = params->feedback_gain;
    NaiveF32 dry_gain = params->dry_gain;
    NaiveF32 wet_gain = params->wet_gain;

    if (delay_len == 0)
        return;

    NaiveF32 *xy = inout;

    while (xy < inout + block_size && states->in_delay_buf.len < delay_len)
    {
        NaiveI32 len = NAIVE_MIN(
            delay_len - states->in_delay_buf.len,
            (NaiveI32)(inout + block_size - xy));

        naive_delay_buf_write(&states->in_delay_buf, xy, len);
        naive_delay_buf_write(&states->out_delay_buf, xy, len);

        for (NaiveI32 i = 0; i < len; i++) {
            xy[i] = dry_gain * xy[i];
        }

        xy += len;
    }

    while (xy < inout + block_size && states->in_delay_buf.len >= delay_len)
    {
        NaiveI32 len = NAIVE_MIN(delay_len, (inout + block_size - xy));

        /*
         * d[n] = x[n - delay_len] + feedback_gain * y[n - delay_len]
         */
        NaiveF32 *xd = scratch;
        NaiveF32 *yd = naive_delay_buf_front(&states->out_delay_buf);
        NaiveF32 *d = (float *)((NaiveUIntPtr)xd + NAIVE_CEIL_8_BYTES(sizeof(NaiveF32) * len));
        
        memcpy(xd, naive_delay_buf_front(&states->in_delay_buf), sizeof(NaiveF32) * len);

        naive_delay_buf_write(&states->in_delay_buf, xy, sizeof(NaiveF32) * len);

        for (NaiveI32 i = 0; i < len; i++) {
            d[i] = xd[i] + feedback_gain * yd[i];
        }

        naive_delay_buf_write(&states->out_delay_buf, d, sizeof(NaiveF32) * len);

        for (NaiveI32 i = 0; i < len; i++) {
            xy[i] = dry_gain * xy[i] + wet_gain * d[i];
        }

        xy += len;
    }
}

void naive_delay_params_init(NaiveDelayParams *params)
{
    params->delay_len = 0;
    params->feedback_gain = 0.0f;
    params->dry_gain = 1.0f;
    params->wet_gain = 0.0f;
}
