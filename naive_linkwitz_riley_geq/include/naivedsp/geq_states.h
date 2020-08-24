#ifndef __NAIVE_GEQ_STATES_H__
#define __NAIVE_GEQ_STATES_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "naivedsp/iir_states.h"

typedef struct {
    NaiveU32              max_num_iirs;
    NaiveIirDf1States     *iir_states;
} NaiveGeqStates;

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: __NAIVE_GEQ_STATES_H__ */
