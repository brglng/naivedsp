#include <string.h>
#include "naivedsp/memory.h"
#include "naivedsp/delay.h"
#include "naivedsp/delay_buf.h"
#include "naivedsp/math.h"

#define NAIVE_DELAY_SCRATCH_SIZE(block_size_cap, delay_len_cap) (sizeof(NaiveF32) * NAIVE_MIN((NaiveUSize)block_size_cap, (NaiveUSize)delay_len_cap))

NaiveErr naive_delay_init(NaiveDelay *self, void *alloc_context, NaiveAllocFunc *alloc, NaiveI32 block_size_cap, NaiveI32 delay_len_cap)
{
    NaiveErr rc = NAIVE_OK;
    NaiveErr err = NAIVE_OK;

    self->block_size_cap = block_size_cap;
    self->delay_len_cap = delay_len_cap;
    self->delay_len = 0;
    self->feedback_gain = 0.0f;
    self->dry_gain = 1.0f;
    self->wet_gain = 0.0f;

    err = naive_delay_buf_init(&self->in_delay_buf, alloc_context, alloc, delay_len_cap);
    if (err) rc = err;

    err = naive_delay_buf_init(&self->out_delay_buf, alloc_context, alloc, delay_len_cap);
    if (err) rc = err;

    self->scratch = alloc(alloc_context, NAIVE_MEM_SCRATCH, NAIVE_DELAY_SCRATCH_SIZE(block_size_cap, delay_len_cap));

    return rc;
}

void naive_delay_reset(NaiveDelay *self)
{
    naive_delay_buf_reset(&self->in_delay_buf);
    naive_delay_buf_reset(&self->out_delay_buf);
}

NaiveErr naive_delay_process(NaiveDelay *self, NaiveF32 *inout, NaiveI32 block_size)
{
    NaiveI32 delay_len = self->delay_len;
    NaiveF32 feedback_gain = self->feedback_gain;
    NaiveF32 dry_gain = self->dry_gain;
    NaiveF32 wet_gain = self->wet_gain;

    if (delay_len == 0)
        return NAIVE_OK;

    if (block_size > self->block_size_cap)
        return NAIVE_ERR_INVALID_PARAMETER;

    NaiveF32 *xy = inout;

    while (xy < inout + block_size && self->in_delay_buf.len < delay_len)
    {
        NaiveI32 len = NAIVE_MIN(delay_len - self->in_delay_buf.len,
                                 (NaiveI32)(inout + block_size - xy));

        naive_delay_buf_write(&self->in_delay_buf, xy, len);
        naive_delay_buf_write(&self->out_delay_buf, xy, len);

        for (NaiveI32 i = 0; i < len; i++) {
            xy[i] = dry_gain * xy[i];
        }

        xy += len;
    }

    while (xy < inout + block_size && self->in_delay_buf.len >= delay_len)
    {
        NaiveI32 len = NAIVE_MIN(delay_len, (NaiveI32)(inout + block_size - xy));

        /*
         * d[n] = x[n - delay_len] + feedback_gain * y[n - delay_len]
         */
        NaiveF32 *xd = naive_delay_buf_front(&self->in_delay_buf);
        NaiveF32 *yd = naive_delay_buf_front(&self->out_delay_buf);
        NaiveF32 *d = self->scratch;
        
        for (NaiveI32 i = 0; i < len; i++) {
            d[i] = xd[i] + feedback_gain * yd[i];
        }
        naive_delay_buf_drain(&self->in_delay_buf, len);
        naive_delay_buf_drain(&self->out_delay_buf, len);

        naive_delay_buf_write(&self->in_delay_buf, xy, len);
        naive_delay_buf_write(&self->out_delay_buf, d, len);

        for (NaiveI32 i = 0; i < len; i++) {
            xy[i] = dry_gain * xy[i] + wet_gain * d[i];
        }

        xy += len;
    }

    return NAIVE_OK;
}

NaiveErr naive_delay_set_delay_len(NaiveDelay *self, NaiveI32 delay_len)
{
    if (delay_len < 0)
        return NAIVE_ERR_INVALID_PARAMETER;

    if (delay_len > self->delay_len_cap)
        return NAIVE_ERR_INVALID_PARAMETER;

    if (delay_len < self->delay_len) {
        naive_delay_buf_drain(&self->in_delay_buf, self->delay_len - delay_len);
        naive_delay_buf_drain(&self->out_delay_buf, self->delay_len - delay_len);
    }

    self->delay_len = delay_len;

    return NAIVE_OK;
}

NaiveErr naive_delay_set_feedback_gain(NaiveDelay *self, NaiveF32 feedback_gain)
{
    self->feedback_gain = feedback_gain;
    return NAIVE_OK;
}

NaiveErr naive_delay_set_dry_gain(NaiveDelay *self, NaiveF32 dry_gain)
{
    self->dry_gain = dry_gain;
    return NAIVE_OK;
}

NaiveErr naive_delay_set_wet_gain(NaiveDelay *self, NaiveF32 wet_gain)
{
    self->wet_gain = wet_gain;
    return NAIVE_OK;
}

void naive_delay_set_default_params(NaiveDelay *self)
{
    self->delay_len = 0;
    self->feedback_gain = 0.0f;
    self->dry_gain = 1.0f;
    self->wet_gain = 0.0f;
}
