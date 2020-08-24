#ifndef __NAIVE_IIR_STATES_H__
#define __NAIVE_IIR_STATES_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "naivedsp/math_types.h"

typedef struct {
    NaiveF32  s0;
    NaiveF32  s1;
} NaiveIir1stDf1States;

void naive_iir_1st_df1_states_init(NaiveIir1stDf1States *self);

typedef struct {
    NaiveF32  s0;
    NaiveF32  s1;
    NaiveF32  s2;
    NaiveF32  s3;
} NaiveIir2ndDf1States;

void naive_iir_2nd_df1_states_init(NaiveIir2ndDf1States *self);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: __NAIVE_IIR_STATES_H__ */
