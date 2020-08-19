#ifndef __NAIVE_DELAY_TEST_DESC_H__
#define __NAIVE_DELAY_TEST_DESC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "naivedsp/test_desc.h"

enum {
    KEY_DELAY_TIME = NAIVE_TEST_CASE_DESC_KEY_USER_BEGIN,
    KEY_FEEDBACK,
    KEY_DRY_GAIN,
    KEY_WET_GAIN
};

NaiveResult init_test_desc(NaiveTestDesc *test_desc, NaiveAllocFunc *alloc, void *allocator);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: __NAIVE_DELAY_TEST_DESC_H__ */
