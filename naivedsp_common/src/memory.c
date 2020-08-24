#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "naivedsp/memory.h"
#include "naivedsp/assert.h"

#define INIT_NUM_BLOCKS_CAP  32

NaiveErr naive_default_allocator_init(NaiveDefaultAllocator *self)
{
    self->_num_blocks_cap = INIT_NUM_BLOCKS_CAP;
    self->num_blocks = 0;
    self->blocks = malloc(sizeof(void *) * 32);
    if (self->blocks == NULL)
        return NAIVE_ERR_NOMEM;
    return NAIVE_OK;
}

NAIVE_ATTRIBUTE_MALLOC void *naive_default_alloc(void *_self, NaiveMemType type, NaiveUSize alignment, NaiveUSize size)
{
    (void)type;

    NaiveDefaultAllocator *self = _self;

    if (self->num_blocks == self->_num_blocks_cap) {
        self->blocks = realloc(self->blocks, sizeof(void *) * self->num_blocks * 2);
        if (self->blocks == NULL)
            return NULL;
        self->_num_blocks_cap *= 2;
    }

    NaiveUSize request_size = size + 1 + alignment;
    NaiveU8* buf = malloc(request_size);
    if (buf == NULL)
        return NULL;
    NAIVE_ASSERT(buf != NULL);
    NaiveUSize remainder = ((NaiveUSize)buf) % alignment;
    NaiveUSize offset = alignment - remainder;
    NaiveU8* ret = buf + (NaiveU8)offset;

    // store how many extra bytes we allocated in the byte just before the
    // pointer we return
    *(NaiveU8*)(ret - 1) = (NaiveU8)offset;

    self->blocks[self->num_blocks] = buf;
    self->num_blocks++;

    return (void*)ret;
}

void naive_default_allocator_finalize(NaiveDefaultAllocator *self)
{
    for (NaiveUSize i = 0; i < self->num_blocks; ++i) {
        free(self->blocks[i]);
    }
    free(self->blocks);
}
