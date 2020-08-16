#ifndef __NAIVE_TYPEDEFS_H__
#define __NAIVE_TYPEDEFS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "naivedsp/platdefs.h"

// NaiveI64 and NaiveU64
#if NAIVE_APPLE || NAIVE_MSVC
typedef long long           NaiveI64;
typedef unsigned long long  NaiveU64;
#else
#if NAIVE_X86_64
typedef long                NaiveI64;
typedef unsigned long       NaiveU64;
#else
typedef long long           NaiveI64;
typedef unsigned long long  NaiveU64;
#endif
#endif

typedef long                NaiveISize;
typedef unsigned long       NaiveUSize;
typedef long                NaiveIntPtr;
typedef unsigned long       NaiveUIntPtr;

#if !NAIVE_ADSP21489
typedef signed char         NaiveI8;
typedef unsigned char       NaiveU8;
typedef short               NaiveI16;
typedef unsigned short      NaiveU16;
#endif

typedef int                 NaiveI32;
typedef unsigned int        NaiveU32;

#if !NAIVE_ADSP21489
typedef double              NaiveF64;
#endif

typedef float               NaiveF32;

typedef enum {
    NAIVE_FALSE,
    NAIVE_TRUE,
} NaiveBool;

typedef enum {
    NAIVE_OK,
    NAIVE_ERR,
    NAIVE_ERR_NOMEM,
    NAIVE_ERR_INVALID_PARAMETER,
} NaiveResult;

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: __NAIVE_TYPEDEFS_H__ */
