#ifndef __NAIVE_PEQ_STATES_H__
#define __NAIVE_PEQ_STATES_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "naivedsp/inttypes.h"
#include "naivedsp/iir_states.h"

typedef struct {
    NaiveF32              max_bands;
    NaiveIirDf1States     *band_states;
} NaivePeqStates;

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: __NAIVE_PEQ_STATES_H__ */
