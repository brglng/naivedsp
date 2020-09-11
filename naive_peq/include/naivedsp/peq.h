#ifndef __NAIVE_PEQ_H__
#define __NAIVE_PEQ_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include "naivedsp/platdefs.h"
#include "naivedsp/memory.h"
#include "naivedsp/iir.h"

typedef struct {
    NaiveI32                num_bands_cap;
    NaiveI32                num_bands;
    NaiveCascadedIirDf1     *bands;
} NaivePeq;

NaiveErr naive_peq_init(NaivePeq *self, void *alloc_context, NaiveAllocFunc alloc, NaiveI32 num_bands_cap, NaiveI32 band_num_fos_cap, NaiveI32 band_num_sos_cap);

void naive_peq_process(NaivePeq *self, NaiveF32 *inout, NaiveI32 len);

void naive_peq_reset(NaivePeq *self);

void naive_peq_set_default_params(NaivePeq *self);

NAIVE_INLINE NaiveCascadedIirDf1* naive_peq_get_band(NaivePeq *self, NaiveI32 index)
{
    if (index < 0 || index > self->num_bands)
        return NULL;
    return &self->bands[index];
}

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: __NAIVE_PEQ_H__ */
