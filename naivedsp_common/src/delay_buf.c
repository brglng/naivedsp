#include <stdlib.h>
#include <string.h>
#include "naivedsp/delay_buf.h"

NaiveErr naive_delay_buf_init(NaiveDelayBuf *self, void *allocator, NaiveAllocFunc alloc, NaiveI32 size)
{
    NAIVE_ASSERT(size > 0);
    self->buf = alloc(allocator, NAIVE_MEM_STATE, sizeof(NaiveF32) * (NaiveUSize)size);
    self->size = size;
    self->pos = 0;
    self->len = 0;
    if (self->buf != NULL) {
        return NAIVE_OK;
    } else {
        return NAIVE_ERR_NOMEM;
    }
}

void naive_delay_buf_write(NaiveDelayBuf *self, NAIVE_CONST NaiveF32 *buf, NaiveI32 len)
{
    NAIVE_ASSERT(len <= self->size - self->len);
    if (len <= self->size - (self->pos + self->len)) {
        memcpy(&self->buf[self->pos + self->len], buf, sizeof(NaiveF32) * (NaiveUSize)len);
    } else {
        memmove(self->buf, &self->buf[self->pos], sizeof(NaiveF32) * (NaiveUSize)self->len);
        memcpy(&self->buf[self->len], buf, sizeof(NaiveF32) * (NaiveUSize)len);
        self->pos = 0;
    }
    self->len += len;
}

void naive_delay_buf_write_zeros(NaiveDelayBuf *self, NaiveI32 len)
{
    NAIVE_ASSERT(len <= self->size - self->len);
    if (len <= self->size - (self->pos + self->len)) {
        memset(&self->buf[self->pos + self->len], 0, sizeof(NaiveF32) * (NaiveUSize)len);
    } else {
        memmove(self->buf, &self->buf[self->pos], sizeof(NaiveF32) * (NaiveUSize)self->len);
        memset(&self->buf[self->len], 0, sizeof(NaiveF32) * (NaiveUSize)len);
        self->pos = 0;
    }
    self->len += len;
}
