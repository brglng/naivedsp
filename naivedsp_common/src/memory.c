#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "naivedsp/memory.h"
#include "naivedsp/assert.h"

#define INIT_NUM_BLOCKS_CAP  32

void naive_default_allocator_init(NaiveDefaultAllocator *self)
{
    self->_num_blocks_cap = INIT_NUM_BLOCKS_CAP;
    self->num_blocks = 0;
    self->blocks = malloc(sizeof(void *) * 32);
    NAIVE_ASSERT(self->blocks != NULL);
}

NAIVE_ATTRIBUTE_MALLOC void *naive_default_alloc(void *_self, NaiveMemType type, NaiveUSize alignment, NaiveUSize size)
{
    (void)type;

    NaiveDefaultAllocator *self = _self;

    if (self->num_blocks == self->_num_blocks_cap) {
        self->blocks = realloc(self->blocks, sizeof(void *) * self->num_blocks * 2);
        NAIVE_ASSERT(self->blocks != NULL);
        self->_num_blocks_cap *= 2;
    }

    NaiveUSize request_size = size + 1 + alignment;
    char* buf = malloc(request_size);
    NAIVE_ASSERT(buf != NULL);
    NaiveUSize remainder = ((NaiveUSize)buf) % alignment;
    NaiveUSize offset = alignment - remainder;
    char* ret = buf + (unsigned char)offset;

    // store how many extra bytes we allocated in the byte just before the
    // pointer we return
    *(unsigned char *)(ret - 1) = offset;

    self->blocks[self->num_blocks] = buf;
    self->num_blocks++;

    return (void *)ret;
}

void naive_default_allocator_finalize(NaiveDefaultAllocator *self)
{
    for (NaiveUSize i = 0; i < self->num_blocks; ++i) {
        free(self->blocks[i]);
    }
    free(self->blocks);
}
