#ifndef __NAIVE_IIR_STATES_H__
#define __NAIVE_IIR_STATES_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "naivedsp/typedefs.h"

typedef struct {
    NaiveF32  s0;
    NaiveF32  s1;
} NaiveIir1stDf1States;

typedef struct {
    NaiveF32  s0;
    NaiveF32  s1;
    NaiveF32  s2;
    NaiveF32  s3;
} NaiveIir2ndDf1States;

typedef struct {
    NaiveIir1stDf1States  fos_states;

    NaiveI32              max_num_sos;
    NaiveIir2ndDf1States  *sos_states;
} NaiveIirDf1States;

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: __NAIVE_IIR_STATES_H__ */
