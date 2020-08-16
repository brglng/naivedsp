#ifndef __NAIVE_IIR_TEST_DESC_H__
#define __NAIVE_IIR_TEST_DESC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "naivedsp/test_desc.h"

enum {
    KEY_ORDER = NAIVE_TEST_CASE_DESC_KEY_USER_BEGIN,
    KEY_TYPE,
    KEY_FREQ,
    KEY_Q,
    KEY_GAIN,
};

enum {
    TYPE_LOWPASS,
    TYPE_HIGHPASS,
    TYPE_BANDPASS,
    TYPE_BANDSTOP,
    TYPE_ALLPASS,
    TYPE_LOW_SHELF,
    TYPE_HIGH_SHELF,
    TYPE_PEAK,
    TYPE_BUTTERWORTH_LOWPASS,
    TYPE_BUTTERWORTH_HIGHPASS,
    TYPE_BUTTERWORTH_BANDPASS,
    TYPE_BUTTERWORTH_BANDSTOP,
};

NaiveResult init_test_desc(NaiveTestDesc *test_desc, NaiveAllocFunc *alloc, void *allocator);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: __NAIVE_IIR_TEST_DESC_H__ */
