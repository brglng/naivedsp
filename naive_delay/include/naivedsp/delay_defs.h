#ifndef __NAIVE_DELAY_DEFS_H__
#define __NAIVE_DELAY_DEFS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "naivedsp/delay_buf_def.h"
#include "naivedsp/typedefs.h"

typedef struct {
    NaiveI32      max_delay_len;
    NaiveI32      delay_len;
    NaiveF32      feedback_gain;
    NaiveF32      mix_level;
    NaiveDelayBuf in_delay_buf;
    NaiveDelayBuf out_delay_buf;
} NaiveDelay;

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: __NAIVE_DELAY_PARAMS_H__ */
