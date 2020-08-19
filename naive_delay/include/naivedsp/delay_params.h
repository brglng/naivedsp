#ifndef __NAIVE_DELAY_PARAMS_H__
#define __NAIVE_DELAY_PARAMS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "naivedsp/typedefs.h"

typedef struct {
    NaiveI32    delay_len;
    NaiveF32    feedback_gain;
    NaiveF32    dry_gain;
    NaiveF32    wet_gain;
} NaiveDelayParams;

void naive_delay_params_init(NaiveDelayParams *self);

NAIVE_INLINE void twl_delay_params_reset(NaiveDelayParams *self) {
    naive_delay_params_init(self);
}

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: __NAIVE_DELAY_PARAMS_H__ */
