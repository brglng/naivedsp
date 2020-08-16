#include <string.h>
#include "naivedsp/memory.h"
#include "naivedsp/delay.h"
#include "naivedsp/delay_buf.h"
#include "naivedsp/math.h"

NaiveResult naive_delay_init(NaiveDelay *self, NaiveAllocFunc *alloc, void *allocator, NaiveI32 max_delay_len)
{
    int rc = NAIVE_OK, err = NAIVE_OK;

    err = naive_fifo_init(&self->in_delay_buf, alloc, allocator, NAIVE_ALIGNOF(NaiveF32), sizeof(NaiveF32) * max_delay_len);
    if (err) rc = err;

    err = naive_fifo_init(&self->out_delay_buf, alloc, allocator, NAIVE_ALIGNOF(NaiveF32), sizeof(NaiveF32) * max_delay_len);
    if (err) rc = err;

    self->max_delay_len = max_delay_len;

    return rc;
}

void naive_delay_reset(NaiveDelayStates *states)
{
    naive_fifo_reset(&states->in_delay_buf);
    naive_fifo_reset(&states->out_delay_buf);
}

void naive_delay_process(NaiveDelayStates *states, NAIVE_CONST NaiveDelayParams *params, NaiveF32 *inout, NaiveU32 block_size, void *scratch)
{
    NaiveU32 delay_len = params->delay_len;
    NaiveF32 feedback_gain = params->feedback_gain;
    NaiveF32 mix_level = params->mix_level;

    if (delay_len == 0)
        return;

    NaiveF32 *xy = inout;

    /*
    * 如果当前延时缓冲区中数据不足延时长度，则将当前输入数据写入缓冲区，
    */
    while (xy < inout + block_size && naive_fifo_size(&states->in_delay_buf) < sizeof(NaiveF32) * delay_len)
    {
        NaiveU32 len = NAIVE_MIN(
            delay_len - naive_fifo_size(&states->in_delay_buf) / sizeof(NaiveF32),
            (NaiveU32)(inout + block_size - xy));

        naive_fifo_write(&states->in_delay_buf, xy, sizeof(NaiveF32) * len);
        naive_fifo_write(&states->out_delay_buf, xy, sizeof(NaiveF32) * len);

        /* 干湿比例 */
        for (NaiveU32 i = 0; i < len; i++) {
            xy[i] = (1 - mix_level) * xy[i];
        }

        xy += len;
    }

    /*
    * 如果存够了延时数据，进行差分方程运算
    */
    while (xy < inout + block_size && naive_fifo_size(&states->in_delay_buf) >= sizeof(NaiveF32) * delay_len)
    {
        NaiveU32 len = NAIVE_MIN(delay_len, (inout + block_size - xy));

        /*
        * 差分方程：
        * d[n] = x[n - delay_len] + feedback_gain * y[n - delay_len]
        */
        NaiveF32 *x_delayed = scratch;
        NaiveF32 *y_delayed = (float *)((NaiveIntPtr)scratch + NAIVE_CEIL_8_BYTES(sizeof(NaiveF32) * len));
        NaiveF32 *d = (float *)((NaiveIntPtr)y_delayed + NAIVE_CEIL_8_BYTES(sizeof(NaiveF32) * len));
        
        naive_fifo_read(&states->in_delay_buf, x_delayed, sizeof(NaiveF32) * len);
        naive_fifo_read(&states->out_delay_buf, y_delayed, sizeof(NaiveF32) * len);

        naive_fifo_write(&states->in_delay_buf, xy, sizeof(NaiveF32) * len);

        for (NaiveU32 i = 0; i < len; i++) {
            d[i] = x_delayed[i] + feedback_gain * y_delayed[i];
        }

        naive_fifo_write(&states->out_delay_buf, d, sizeof(NaiveF32) * len);

        /* 干湿比例调节 */
        for (NaiveU32 i = 0; i < len; i++) {
            xy[i] = (1 - mix_level) * xy[i] + mix_level * d[i];
        }

        xy += len;
    }
}

void naive_delay_params_init(NaiveDelayParams *self)
{
    self->delay_len = 0;
    self->feedback_gain = 0.0f;
    self->mix_level = 0.0f;
}
