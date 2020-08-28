#ifndef __NAIVE_ERR_H__
#define __NAIVE_ERR_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    NAIVE_OK,
    NAIVE_ERR,
    NAIVE_ERR_NOMEM,
    NAIVE_ERR_INVALID_PARAMETER,
    NAIVE_ERR_CODES_COUNT,
} NaiveErr;

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: __NAIVE_ERR_H__ */
