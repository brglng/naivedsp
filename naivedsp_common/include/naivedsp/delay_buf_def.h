#ifndef __NAIVE_DELAY_BUF_DEF_H__
#define __NAIVE_DELAY_BUF_DEF_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "naivedsp/typedefs.h"

typedef struct {
    NaiveF32  *buf;
    NaiveI32  size;
    NaiveI32  pos;
    NaiveI32  len;
} NaiveDelayBuf;

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: __NAIVE_DELAY_BUF_DEF_H__ */
