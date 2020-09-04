#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "naivedsp/iir.h"
#include "naivedsp/math.h"
#include "naivedsp/test.h"

typedef struct {
    NaiveI32              order;
    NaiveIir1stDf1States  iir_1st_states;
    NaiveIir1stCoeffs     iir_1st_coeffs;
    NaiveIir2ndDf1States  iir_2nd_states;
    NaiveIir2ndCoeffs     iir_2nd_coeffs;
} TestContext;

NaiveErr set_params(void *_context, NAIVE_CONST TomlTable *config, NaiveI32 sample_rate)
{
    TestContext *context = _context;

    context->order = (NaiveI32)toml_table_get_as_integer(config, "order");
    const char *type = toml_table_get_as_string(config, "type")->str;
    NaiveF32 freq = (NaiveF32)toml_table_get_as_float(config, "freq");

    switch (context->order) {
        case 1:
            if (strcmp(type, "lowpass") == 0) {
                naive_iir_1st_coeffs_set_lowpass(&context->iir_1st_coeffs, sample_rate, freq);
            } else if (strcmp(type, "highpass") == 0) {
                naive_iir_1st_coeffs_set_highpass(&context->iir_1st_coeffs, sample_rate, freq);
            } else if (strcmp(type, "allpass") == 0) {
                naive_iir_1st_coeffs_set_allpass(&context->iir_1st_coeffs, sample_rate, freq);
            } else if (strcmp(type, "butterworth-lowpass") == 0) {
                naive_iir_1st_coeffs_set_butterworth_lowpass(&context->iir_1st_coeffs, sample_rate, freq);
            } else if (strcmp(type, "butterworth-highpass") == 0) {
                naive_iir_1st_coeffs_set_butterworth_highpass(&context->iir_1st_coeffs, sample_rate, freq);
            }
            break;
        case 2:
            if (strcmp(type, "lowpass") == 0) {
                naive_iir_2nd_coeffs_set_lowpass(&context->iir_2nd_coeffs, sample_rate, freq, (NaiveF32)toml_table_get_as_float(config, "q"));
            } else if (strcmp(type, "highpass") == 0) {
                naive_iir_2nd_coeffs_set_highpass(&context->iir_2nd_coeffs, sample_rate, freq, (NaiveF32)toml_table_get_as_float(config, "q"));
            } else if (strcmp(type, "bandpass") == 0) {
                naive_iir_2nd_coeffs_set_bandpass(&context->iir_2nd_coeffs, sample_rate, freq, (NaiveF32)toml_table_get_as_float(config, "q"));
            } else if (strcmp(type, "bandstop") == 0) {
                naive_iir_2nd_coeffs_set_bandstop(&context->iir_2nd_coeffs, sample_rate, freq, (NaiveF32)toml_table_get_as_float(config, "q"));
            } else if (strcmp(type, "allpass") == 0) {
                naive_iir_2nd_coeffs_set_allpass(&context->iir_2nd_coeffs, sample_rate, freq, (NaiveF32)toml_table_get_as_float(config, "q"));
            } else if (strcmp(type, "low-shelf") == 0) {
                naive_iir_2nd_coeffs_set_low_shelf(&context->iir_2nd_coeffs, sample_rate, freq, (NaiveF32)toml_table_get_as_float(config, "q"), (NaiveF32)toml_table_get_as_float(config, "gain"));
            } else if (strcmp(type, "high-shelf") == 0) {
                naive_iir_2nd_coeffs_set_high_shelf(&context->iir_2nd_coeffs, sample_rate, freq, (NaiveF32)toml_table_get_as_float(config, "q"), (NaiveF32)toml_table_get_as_float(config, "gain"));
            } else if (strcmp(type, "peak") == 0) {
                naive_iir_2nd_coeffs_set_peak(&context->iir_2nd_coeffs, sample_rate, freq, (NaiveF32)toml_table_get_as_float(config, "q"), (NaiveF32)toml_table_get_as_float(config, "gain"));
            } else if (strcmp(type, "butterworth-lowpass") == 0) {
                naive_iir_2nd_coeffs_set_butterworth_lowpass(&context->iir_2nd_coeffs, sample_rate, freq);
            } else if (strcmp(type, "butterworth-highpass") == 0) {
                naive_iir_2nd_coeffs_set_butterworth_highpass(&context->iir_2nd_coeffs, sample_rate, freq);
            } else if (strcmp(type, "butterworth-bandpass") == 0) {
                naive_iir_2nd_coeffs_set_butterworth_bandpass(&context->iir_2nd_coeffs, sample_rate, freq, (NaiveF32)toml_table_get_as_float(config, "q"));
            } else if (strcmp(type, "butterworth-bandstop") == 0) {
                naive_iir_2nd_coeffs_set_butterworth_bandstop(&context->iir_2nd_coeffs, sample_rate, freq, (NaiveF32)toml_table_get_as_float(config, "q"));
            }
            break;
    }

    return NAIVE_OK;
}

NaiveErr test_setup(void *_context, NAIVE_CONST TomlTable *config, NaiveI32 sample_rate)
{
    TestContext *context = _context;

    context->order = 0;
    naive_iir_1st_coeffs_init(&context->iir_1st_coeffs);
    naive_iir_1st_df1_states_init(&context->iir_1st_states);
    naive_iir_2nd_coeffs_init(&context->iir_2nd_coeffs);
    naive_iir_2nd_df1_states_init(&context->iir_2nd_states);

    return set_params(context, config, sample_rate);
}

void test_teardown(void *_context)
{
    (void)_context;
}

NaiveErr test_process(void *_context, NaiveF32 **in, NaiveF32 **out, NaiveI32 block_size)
{
    TestContext *context = _context;
    memcpy(out[0], in[0], sizeof(NaiveF32) * (NaiveUSize)block_size);
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

    NaiveTest test;
    TestContext context;

    naive_iir_1st_df1_states_init(&context.iir_1st_states);
    naive_iir_1st_coeffs_init(&context.iir_1st_coeffs);
    NaiveErr err = naive_test_init(&test,
                                   &naive_default_alloc,
                                   &allocator,
                                   NAIVE_TEST_SOURCE_DIR "/config.toml",
                                   NAIVE_TEST_INPUTS_DIR,
                                   NAIVE_TEST_BINARY_DIR "/outputs",
                                   NAIVE_TEST_SOURCE_DIR "/refs",
                                   1,
                                   1,
                                   256,
                                   &test_setup,
                                   &test_teardown,
                                   &test_process,
                                   &context);

    NaiveI32 num_failed = 0;
    if (!err) {
        num_failed = naive_test_run(&test);
    }

    naive_default_allocator_finalize(&allocator);

    return (!err && num_failed == 0) ? 0 : ((int)err + NAIVE_ERR_CODES_COUNT + num_failed);
}
