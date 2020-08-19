#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "naivedsp/iir.h"
#include "naivedsp/iir_design.h"
#include "naivedsp/math.h"
#include "naivedsp/test.h"
#include "test_desc.h"

typedef struct {
    NaiveI32              order;
    NaiveIir1stDf1States  iir_1st_states;
    NaiveIir1stCoeffs     iir_1st_coeffs;
    NaiveIir2ndDf1States  iir_2nd_states;
    NaiveIir2ndCoeffs     iir_2nd_coeffs;
} TestContext;

NaiveResult set_params(void *_context, NAIVE_CONST NaiveTestCaseDesc *case_desc, NaiveI32 sample_rate)
{
    TestContext *context = _context;

    context->order = naive_test_case_desc_get_i32(case_desc, KEY_ORDER);
    int type = naive_test_case_desc_get_int(case_desc, KEY_TYPE);
    NaiveF32 freq = naive_test_case_desc_get_f32(case_desc, KEY_FREQ);

    switch (context->order) {
    case 1:
        switch (type) {
        case TYPE_LOWPASS:
            naive_iir_1st_coeffs_set_lowpass(&context->iir_1st_coeffs, sample_rate, freq);
            break;
        case TYPE_HIGHPASS:
            naive_iir_1st_coeffs_set_highpass(&context->iir_1st_coeffs, sample_rate, freq);
            break;
        case TYPE_ALLPASS:
            naive_iir_1st_coeffs_set_allpass(&context->iir_1st_coeffs, sample_rate, freq);
            break;
        case TYPE_BUTTERWORTH_LOWPASS:
            naive_iir_1st_coeffs_set_butterworth_lowpass(&context->iir_1st_coeffs, sample_rate, freq);
            break;
        case TYPE_BUTTERWORTH_HIGHPASS:
            naive_iir_1st_coeffs_set_butterworth_highpass(&context->iir_1st_coeffs, sample_rate, freq);
            break;
        }
        break;
    case 2:
        switch (type) {
        case TYPE_LOWPASS:
            naive_iir_2nd_coeffs_set_lowpass(&context->iir_2nd_coeffs, sample_rate, freq, naive_test_case_desc_get_f32(case_desc, KEY_Q));
            break;
        case TYPE_HIGHPASS:
            naive_iir_2nd_coeffs_set_highpass(&context->iir_2nd_coeffs, sample_rate, freq, naive_test_case_desc_get_f32(case_desc, KEY_Q));
            break;
        case TYPE_BANDPASS:
            naive_iir_2nd_coeffs_set_bandpass(&context->iir_2nd_coeffs, sample_rate, freq, naive_test_case_desc_get_f32(case_desc, KEY_Q));
            break;
        case TYPE_BANDSTOP:
            naive_iir_2nd_coeffs_set_bandstop(&context->iir_2nd_coeffs, sample_rate, freq, naive_test_case_desc_get_f32(case_desc, KEY_Q));
            break;
        case TYPE_ALLPASS:
            naive_iir_2nd_coeffs_set_allpass(&context->iir_2nd_coeffs, sample_rate, freq, naive_test_case_desc_get_f32(case_desc, KEY_Q));
            break;
        case TYPE_LOW_SHELF:
            naive_iir_2nd_coeffs_set_low_shelf(&context->iir_2nd_coeffs, sample_rate, freq, naive_test_case_desc_get_f32(case_desc, KEY_Q), naive_test_case_desc_get_f32(case_desc, KEY_GAIN));
            break;
        case TYPE_HIGH_SHELF:
            naive_iir_2nd_coeffs_set_high_shelf(&context->iir_2nd_coeffs, sample_rate, freq, naive_test_case_desc_get_f32(case_desc, KEY_Q), naive_test_case_desc_get_f32(case_desc, KEY_GAIN));
            break;
        case TYPE_PEAK:
            naive_iir_2nd_coeffs_set_peak(&context->iir_2nd_coeffs, sample_rate, freq, naive_test_case_desc_get_f32(case_desc, KEY_Q), naive_test_case_desc_get_f32(case_desc, KEY_GAIN));
            break;
        case TYPE_BUTTERWORTH_LOWPASS:
            naive_iir_2nd_coeffs_set_butterworth_lowpass(&context->iir_2nd_coeffs, sample_rate, freq);
            break;
        case TYPE_BUTTERWORTH_HIGHPASS:
            naive_iir_2nd_coeffs_set_butterworth_highpass(&context->iir_2nd_coeffs, sample_rate, freq);
            break;
        case TYPE_BUTTERWORTH_BANDPASS:
            naive_iir_2nd_coeffs_set_butterworth_bandpass(&context->iir_2nd_coeffs, sample_rate, freq, naive_test_case_desc_get_f32(case_desc, KEY_Q));
            break;
        case TYPE_BUTTERWORTH_BANDSTOP:
            naive_iir_2nd_coeffs_set_butterworth_bandstop(&context->iir_2nd_coeffs, sample_rate, freq, naive_test_case_desc_get_f32(case_desc, KEY_Q));
            break;
        }
        break;
    }

    return NAIVE_OK;
}

NaiveResult test_setup(void *_context, NAIVE_CONST NaiveTestCaseDesc *case_desc, NaiveI32 sample_rate)
{
    TestContext *context = _context;

    context->order = 0;
    naive_iir_1st_coeffs_init(&context->iir_1st_coeffs);
    naive_iir_1st_df1_states_init(&context->iir_1st_states);
    naive_iir_2nd_coeffs_init(&context->iir_2nd_coeffs);
    naive_iir_2nd_df1_states_init(&context->iir_2nd_states);

    return set_params(context, case_desc, sample_rate);
}

void test_teardown(void *_context)
{
    (void)_context;
}

NaiveResult test_process(void *_context, NaiveF32 **in, NaiveF32 **out, NaiveI32 block_size)
{
    TestContext *context = _context;
    memcpy(out[0], in[0], sizeof(NaiveF32) * block_size);
    switch (context->order) {
    case 1:
        naive_iir_1st_df1_process(&context->iir_1st_states, &context->iir_1st_coeffs, out[0], block_size);
        break;
    case 2:
        naive_iir_2nd_df1_process(&context->iir_2nd_states, &context->iir_2nd_coeffs, out[0], block_size);
        break;
    }
    return NAIVE_OK;
}

int main(void)
{
    NaiveDefaultAllocator allocator;
    naive_default_allocator_init(&allocator);

    NaiveTestDesc test_desc;
    int err = init_test_desc(&test_desc, &naive_default_alloc, &allocator);

    NaiveTest test;
    TestContext context;

    if (!err) {
        naive_iir_1st_df1_states_init(&context.iir_1st_states);
        naive_iir_1st_coeffs_init(&context.iir_1st_coeffs);
        err = naive_test_init(&test,
                            &naive_default_alloc,
                            &allocator,
                            &test_desc,
                            NAIVE_TEST_INPUTS_DIR,
                            NAIVE_TEST_WORKING_DIR "/test_out",
                            NAIVE_TEST_REFS_DIR,
                            1,
                            1,
                            256,
                            &test_setup,
                            &test_teardown,
                            &test_process,
                            &context);
    }

    if (!err) {
        err = naive_test_run(&test);
    }

    naive_default_allocator_finalize(&allocator);

    return err;
}
