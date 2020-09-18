#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "naivedsp/math.h"
#include "naivedsp/memory.h"
#include "naivedsp/assert.h"

#define INIT_NUM_BLOCKS_CAP  256

NaiveErr naive_default_allocator_init(NaiveDefaultAllocator *self)
{
    self->_num_blocks_cap = INIT_NUM_BLOCKS_CAP;
    self->num_blocks = 0;
    self->blocks = malloc(sizeof(void *) * INIT_NUM_BLOCKS_CAP);
    if (self->blocks == NULL)
        return NAIVE_ERR_NOMEM;
    return NAIVE_OK;
}

NAIVE_ATTRIBUTE_MALLOC void *naive_default_alloc(void *_self, NaiveMemType type, NaiveUSize size)
{
    NaiveDefaultAllocator *self = _self;

    (void)type;

    if (self->num_blocks == self->_num_blocks_cap) {
        self->blocks = realloc(self->blocks, sizeof(void *) * self->num_blocks * 2);
        NAIVE_ASSERT(self->blocks != NULL);
        self->_num_blocks_cap *= 2;
    }

    void *p = malloc(size);
    NAIVE_ASSERT(p != NULL);

    self->blocks[self->num_blocks] = p;
    self->num_blocks++;

    return p;

    return NULL;
}

void naive_default_allocator_finalize(NaiveDefaultAllocator *self)
{
    for (NaiveUSize i = 0; i < self->num_blocks; ++i) {
        free(self->blocks[i]);
    }
    self->num_blocks = 0;
    free(self->blocks);
    self->blocks = NULL;
}
