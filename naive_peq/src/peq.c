#include "naivedsp/memory.h"
#include "naivedsp/math.h"
#include "naivedsp/iir.h"
#include "naivedsp/peq.h"

NaiveErr naive_peq_init(NaivePeq *self, void *alloc_context, NaiveAllocFunc alloc, NaiveI32 num_bands_cap, NaiveI32 band_num_sos_cap)
{
    NaiveErr err = NAIVE_OK;

    self->num_bands_cap = num_bands_cap;
    self->bands = NAIVE_NEW(alloc_context, alloc, NAIVE_MEM_STATE, NaiveIirDf1, (NaiveUSize)num_bands_cap);
    if (!err && !self->bands)
        err = NAIVE_ERR_NOMEM;

    if (self->bands) {
        for (NaiveI32 i = 0; i < num_bands_cap; ++i) {
            NaiveErr rc = naive_iir_df1_init(&self->bands[i], alloc_context, alloc, band_num_sos_cap);
            if (!err && rc)
                err = rc;
        }
    }

    return err;
}

void naive_peq_process(NaivePeq *self, NaiveF32 *inout, NaiveI32 len)
{
    for (NaiveI32 i = 0; i < self->num_bands; ++i) {
        naive_iir_df1_process(&self->bands[i], inout, len);
    }
}

void naive_peq_reset(NaivePeq *self)
{
    for (NaiveI32 i = 0; i < self->num_bands; ++i) {
        naive_iir_df1_reset(&self->bands[i]);
    }
}

void naive_peq_set_default_params(NaivePeq *self)
{
    for (NaiveI32 i = 0; i < self->num_bands; ++i) {
        naive_iir_df1_set_default_params(&self->bands[i]);
    }
}
