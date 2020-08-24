#ifndef __NAIVE_ASSERT_H__
#define __NAIVE_ASSERT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "naivedsp/platdefs.h"

#ifdef NDEBUG

#define _NAIVE_ASSERT0(condition) ((void)(condition))
#define _NAIVE_ASSERT1(condition, msg, ...) ((void)(condition))

#else

#define _NAIVE_ASSERT0(condition)                                       \
    do {                                                                \
        int c = condition;                                              \
        if (!c) {                                                       \
            fprintf(stderr, "assertion `%s' failed\n", #condition);     \
            abort();                                                    \
        }                                                               \
        assert(c);                                                      \
    } while (0)

#define _NAIVE_ASSERT1(condition, msg, ...)                                             \
    do {                                                                                \
      int c = condition;                                                                \
      if (!c) {                                                                         \
          fprintf(stderr, "assertion `%s' failed: \n" msg, #condition, ##__VA_ARGS__);  \
          abort();                                                                      \
      }                                                                                 \
      assert(c);                                                                        \
    } while (0)

#endif

#define _NAIVE_ASSERT(condition, _0, _1, _2, _3, _4, _5, _6,    \
                      _7, _8, _9, _10, _11, _12, _13, _14, _15, \
                      _16, _17, _18, _19, _20, _21, _22, _23,   \
                      _24, _25, _26, _27, _28, _29, _30, _31,   \
                      _32, _33, _34, _35, _36, _37, _38, _39,   \
                      _40, _41, _42, _43, _44, _45, _46, _47,   \
                      _48, _49, _50, _51, _52, _53, _54, _55,   \
                      _56, _57, _58, _59, _60, _61, _62, _63,   \
                      _64, name, ...) name

#define NAIVE_ASSERT(condition, ...)    \
  _NAIVE_ASSERT(condition,              \
                _0,                     \
                ##__VA_ARGS__,          \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT1,         \
                _NAIVE_ASSERT0)(        \
                    condition,          \
                    ##__VA_ARGS__)


#ifdef __cplusplus
}
#endif

#endif /* end of include guard: __NAIVE_ASSERT_H__ */
