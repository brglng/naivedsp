#ifndef __NAIVE_DELAY_STATES_H__
#define __NAIVE_DELAY_STATES_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "naivedsp/delay_buf_def.h"
#include "naivedsp/typedefs.h"

typedef struct {
    NaiveI32        delay_len_cap;
    NaiveDelayBuf   in_delay_buf;
    NaiveDelayBuf   out_delay_buf;
} NaiveDelayStates;

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: __NAIVE_DELAY_STATES_H__ */
