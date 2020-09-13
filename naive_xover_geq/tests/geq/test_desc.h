#ifndef __NAIVE_GEQ_TEST_DESC_H__
#define __NAIVE_GEQ_TEST_DESC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "naivedsp/test_desc.h"

enum {
    KEY_NUM_BANDS = NAIVE_TEST_CASE_DESC_KEY_USER_BEGIN,
    KEY_ORDER,
    KEY_FREQ0,
    KEY_FREQ1,
    KEY_FREQ2,
    KEY_FREQ3,
    KEY_FREQ4,
    KEY_FREQ5,
    KEY_FREQ6,
    KEY_FREQ7,
    KEY_FREQ8,
    KEY_FREQ9,
    KEY_FREQ10,
    KEY_FREQ11,
    KEY_FREQ12,
    KEY_FREQ13,
    KEY_FREQ14,
    KEY_FREQ15,
    KEY_FREQ16,
    KEY_FREQ17,
    KEY_FREQ18,
    KEY_BAND0_GAIN,
    KEY_BAND1_GAIN,
    KEY_BAND2_GAIN,
    KEY_BAND3_GAIN,
    KEY_BAND4_GAIN,
    KEY_BAND5_GAIN,
    KEY_BAND6_GAIN,
    KEY_BAND7_GAIN,
    KEY_BAND8_GAIN,
    KEY_BAND9_GAIN,
    KEY_BAND10_GAIN,
    KEY_BAND11_GAIN,
    KEY_BAND12_GAIN,
    KEY_BAND13_GAIN,
    KEY_BAND14_GAIN,
    KEY_BAND15_GAIN,
    KEY_BAND16_GAIN,
    KEY_BAND17_GAIN,
    KEY_BAND18_GAIN,
    KEY_BAND19_GAIN,
};

NaiveResult init_test_desc(NaiveTestDesc *test_desc, NaiveAllocFunc *alloc, void *allocator);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: __NAIVE_GEQ_TEST_DESC_H__ */
