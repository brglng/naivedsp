#ifndef __NAIVE_DELAY_BUF_H__
#define __NAIVE_DELAY_BUF_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "naivedsp/assert.h"
#include "naivedsp/delay_buf_def.h"
#include "naivedsp/err.h"
#include "naivedsp/memory.h"

typedef struct {
    NaiveF32  *buf;
    NaiveI32  size;
    NaiveI32  pos;
    NaiveI32  len;
} NaiveDelayBuf;

NaiveErr naive_delay_buf_init(NaiveDelayBuf *self, void *allocator, NaiveAllocFunc alloc, NaiveI32 size);

NAIVE_INLINE void naive_delay_buf_reset(NaiveDelayBuf *self) {
    self->pos = 0;
    self->len = 0;
}

NAIVE_INLINE NaiveF32* naive_delay_buf_front(NaiveDelayBuf *self) {
    return &self->buf[self->pos];
}

NAIVE_INLINE NaiveBool naive_delay_buf_is_full(NAIVE_CONST NaiveDelayBuf *self) {
    return self->len == self->size;
}

NAIVE_INLINE NaiveBool naive_delay_buf_is_empty(NAIVE_CONST NaiveDelayBuf *self) {
    return self->len == 0;
}

NAIVE_INLINE void naive_delay_buf_drain(NaiveDelayBuf *self, NaiveI32 len) {
    NAIVE_ASSERT(len <= self->len);
    self->pos += len;
    self->len -= len;
}

void naive_delay_buf_write(NaiveDelayBuf *self, NAIVE_CONST NaiveF32 *buf, NaiveI32 len);
void naive_delay_buf_write_zeros(NaiveDelayBuf *self, NaiveI32 len);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: __NAIVE_DELAY_BUF_H__ */
