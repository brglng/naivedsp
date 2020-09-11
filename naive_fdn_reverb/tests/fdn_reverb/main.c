#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "naivedsp/fdn_reverb.h"
#include "naivedsp/test.h"

typedef struct {
    NaiveDefaultAllocator   allocator;
    NaiveFdnReverb          obj;
} TestContext;

NaiveErr set_params(void *_context, NAIVE_CONST TomlTable *config, NaiveI32 sample_rate)
{
    TestContext *context = _context;
    NaiveErr err = NAIVE_OK;

    (void)sample_rate;

    err = naive_fdn_reverb_set_pre_delay_time(&context->obj, (NaiveF32)toml_table_get_as_float(config, "pre-delay-time"));
    if (!err) {
        NaiveI32 num_delays = (NaiveI32)toml_table_get_as_integer(config, "num-delays");
        NaiveF32 min_acoustic_path = (NaiveF32)toml_table_get_as_float(config, "min-acoustic-path");
        NaiveF32 max_acoustic_path = (NaiveF32)toml_table_get_as_float(config, "max-acoustic-path");
        NaiveF32 low_mix_xover_freq = (NaiveF32)toml_table_get_as_float(config, "low-mid-xover-freq");
        NaiveF32 high_damp_freq = (NaiveF32)toml_table_get_as_float(config, "high-damp-freq");
        NaiveF32 dc_reverb_time = (NaiveF32)toml_table_get_as_float(config, "dc-reverb-time");
        NaiveF32 mid_freq_reverb_time = (NaiveF32)toml_table_get_as_float(config, "mid-freq-reverb-time");
        err = naive_fdn_reverb_set_room_params(&context->obj,
                                               num_delays,
                                               min_acoustic_path,
                                               max_acoustic_path,
                                               low_mix_xover_freq,
                                               high_damp_freq,
                                               dc_reverb_time,
                                               mid_freq_reverb_time);
    }
    if (!err) {
        err = naive_fdn_reverb_set_input_gain(&context->obj, (NaiveF32)toml_table_get_as_float(config, "input-gain"));
    }
    if (!err) {
        err = naive_fdn_reverb_set_output_gain(&context->obj, (NaiveF32)toml_table_get_as_float(config, "output-gain"));
    }
    if (!err) {
        err = naive_fdn_reverb_set_dry_gain(&context->obj, (NaiveF32)toml_table_get_as_float(config, "dry-gain"));
    }
    if (!err) {
        err = naive_fdn_reverb_set_wet_gain(&context->obj, (NaiveF32)toml_table_get_as_float(config, "wet-gain"));
    }

    return err;
}

NaiveErr test_setup(void *_context, NAIVE_CONST TomlTable *config, NaiveI32 sample_rate)
{
    TestContext *context = _context;
    NaiveErr err;

    err = naive_default_allocator_init(&context->allocator);
    if (err)
        return err;

    err = naive_fdn_reverb_init(&context->obj,
                                &context->allocator,
                                naive_default_alloc,
                                sample_rate,
                                256,
                                sample_rate * 1,
                                16,
                                (NaiveI32)((NaiveF32)sample_rate * 100.0f / 343.0f));
    if (err)
        return err;

    return set_params(context, config, sample_rate);
}

void test_teardown(void *_context)
{
    TestContext *context = _context;
    naive_default_allocator_finalize(&context->allocator);
}

NaiveErr test_process(void *_context,
                      NaiveF32 **in,
                      NaiveI32 num_in_channels,
                      NaiveI32 in_len,
                      NaiveF32 **out,
                      NaiveI32 *out_len)
{
    TestContext *context = _context;
    if (num_in_channels == 1) {
        memcpy(in[1], in[0], sizeof(NaiveF32) * (NaiveUSize)in_len);
    }
    *out_len = in_len;
    return naive_fdn_reverb_process(&context->obj, in[0], in[1], out[0], out[1], in_len);
}

int main(void)
{
    NaiveErr err = 0;

    NaiveDefaultAllocator allocator;
    naive_default_allocator_init(&allocator);

    TestContext context;
    NaiveTest test;

    if (!err) {
        err = naive_test_init(&test,
                              &naive_default_alloc,
                              &allocator,
                              NAIVE_TEST_SOURCE_DIR "/config.toml",
                              NAIVE_TEST_INPUTS_DIR,
                              NAIVE_TEST_BINARY_DIR "/outputs",
                              NAIVE_TEST_SOURCE_DIR "/refs",
                              2,
                              2,
                              256,
                              &test_setup,
                              &test_teardown,
                              &test_process,
                              &context);
    }

    NaiveI32 num_failed = 0;
    if (!err) {
        num_failed = naive_test_run(&test);
    }

    naive_default_allocator_finalize(&allocator);

    return (!err && num_failed == 0) ? 0 : ((int)err + NAIVE_ERR_CODES_COUNT + num_failed);
}
