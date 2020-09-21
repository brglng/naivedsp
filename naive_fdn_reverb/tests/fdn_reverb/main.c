#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "naivedsp/fdn_reverb.h"
#include "naivedsp/test.h"

typedef struct {
    NaiveDefaultAllocator   allocator;
    NaiveFdnReverb          obj;
    void*                   scratch;
} TestContext;

NaiveErr set_params(void *_context, NAIVE_CONST TomlTable *config, NaiveI32 sample_rate)
{
    TestContext *context = _context;
    NaiveErr err = NAIVE_OK;

    (void)sample_rate;

    NaiveF32 balance = (NaiveF32)toml_table_get_as_float(config, "balance");
    NaiveF32 crosstalk = (NaiveF32)toml_table_get_as_float(config, "crosstalk");
    err = naive_fdn_reverb_set_balance_crosstalk(&context->obj, balance, crosstalk);
    if (!err) {
        err = naive_fdn_reverb_set_left_pre_delay_time(&context->obj, (NaiveF32)toml_table_get_as_float(config, "left-pre-delay-time"));
    }
    if (!err) {
        err = naive_fdn_reverb_set_right_pre_delay_time(&context->obj, (NaiveF32)toml_table_get_as_float(config, "right-pre-delay-time"));
    }

    NaiveF32 low_mid_xover_freq;
    NaiveF32 high_damp_freq;
    NaiveF32 dc_reverb_time;
    NaiveF32 mid_freq_reverb_time;
    if (!err) {
        NaiveI32 num_delays = (NaiveI32)toml_table_get_as_integer(config, "num-delays");
        NaiveF32 min_acoustic_path = (NaiveF32)toml_table_get_as_float(config, "min-acoustic-path");
        NaiveF32 max_acoustic_path = (NaiveF32)toml_table_get_as_float(config, "max-acoustic-path");
        low_mid_xover_freq = (NaiveF32)toml_table_get_as_float(config, "low-mid-xover-freq");
        high_damp_freq = (NaiveF32)toml_table_get_as_float(config, "high-damp-freq");
        dc_reverb_time = (NaiveF32)toml_table_get_as_float(config, "dc-reverb-time");
        mid_freq_reverb_time = (NaiveF32)toml_table_get_as_float(config, "mid-freq-reverb-time");
        err = naive_fdn_reverb_set_diffuse_params(&context->obj,
                                                  num_delays,
                                                  min_acoustic_path,
                                                  max_acoustic_path,
                                                  low_mid_xover_freq,
                                                  high_damp_freq,
                                                  dc_reverb_time,
                                                  mid_freq_reverb_time);
    }
    if (!err) {
        NAIVE_CONST char *early_type = toml_table_get_as_string(config, "early-reflection")->str;
        if (strcmp(early_type, "moorer") == 0) {
            err = naive_fdn_reverb_set_early_reflection_preset(&context->obj,
                                                               NAIVE_EARLY_REFLECTION_TYPE_MOORER,
                                                               low_mid_xover_freq,
                                                               high_damp_freq,
                                                               dc_reverb_time,
                                                               mid_freq_reverb_time);
        } else if (strcmp(early_type, "moorer-hall") == 0) {
            err = naive_fdn_reverb_set_early_reflection_preset(&context->obj,
                                                               NAIVE_EARLY_REFLECTION_TYPE_MOORER_HALL,
                                                               low_mid_xover_freq,
                                                               high_damp_freq,
                                                               dc_reverb_time,
                                                               mid_freq_reverb_time);
        }
    }
    if (!err) {
        err = naive_fdn_reverb_set_left_early_send_time(&context->obj, (NaiveF32)toml_table_get_as_float(config, "left-early-send-time"));
    }
    if (!err) {
        err = naive_fdn_reverb_set_right_early_send_time(&context->obj, (NaiveF32)toml_table_get_as_float(config, "right-early-send-time"));
    }
    if (!err) {
        err = naive_fdn_reverb_set_left_early_send_gain(&context->obj, (NaiveF32)toml_table_get_as_float(config, "left-early-send-gain"));
    }
    if (!err) {
        err = naive_fdn_reverb_set_right_early_send_gain(&context->obj, (NaiveF32)toml_table_get_as_float(config, "right-early-send-gain"));
    }
    if (!err) {
        err = naive_fdn_reverb_set_input_gain(&context->obj, (NaiveF32)toml_table_get_as_float(config, "input-gain"));
    }
    if (!err) {
        err = naive_fdn_reverb_set_output_gain(&context->obj, (NaiveF32)toml_table_get_as_float(config, "output-gain"));
    }
    if (!err) {
        err = naive_fdn_reverb_set_left_dry_gain(&context->obj, (NaiveF32)toml_table_get_as_float(config, "left-dry-gain"));
    }
    if (!err) {
        err = naive_fdn_reverb_set_left_early_gain(&context->obj, (NaiveF32)toml_table_get_as_float(config, "left-early-gain"));
    }
    if (!err) {
        err = naive_fdn_reverb_set_left_late_gain(&context->obj, (NaiveF32)toml_table_get_as_float(config, "left-late-gain"));
    }
    if (!err) {
        err = naive_fdn_reverb_set_right_dry_gain(&context->obj, (NaiveF32)toml_table_get_as_float(config, "right-dry-gain"));
    }
    if (!err) {
        err = naive_fdn_reverb_set_right_early_gain(&context->obj, (NaiveF32)toml_table_get_as_float(config, "right-early-gain"));
    }
    if (!err) {
        err = naive_fdn_reverb_set_right_late_gain(&context->obj, (NaiveF32)toml_table_get_as_float(config, "right-late-gain"));
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
                                sample_rate * 1,
                                64,
                                (NaiveI32)((NaiveF32)sample_rate * 0.1f),
                                16,
                                (NaiveI32)((NaiveF32)sample_rate * 100.0f / 343.0f));
    if (err)
        return err;

    context->scratch = naive_default_alloc(&context->allocator, NAIVE_MEM_SCRATCH, NAIVE_FDN_REVERB_SCRATCH_SIZE(16, 256));
    if (context->scratch == NULL)
        return NAIVE_ERR_NOMEM;

    return set_params(context, config, sample_rate);
}

void test_teardown(void *_context)
{
    TestContext *context = _context;
    naive_default_allocator_finalize(&context->allocator);
    context->scratch = NULL;
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
        naive_gain(in[0], in_len, sqrtf(2.0f) / 2.0f);
        naive_gain(in[1], in_len, sqrtf(2.0f) / 2.0f);
    }
    *out_len = in_len;
    return naive_fdn_reverb_process(&context->obj, in[0], in[1], out[0], out[1], in_len, context->scratch);
}

int main(void)
{
    NaiveErr err = 0;
    NaiveDefaultAllocator allocator;
    NaiveTest test;
    TestContext context;
    NaiveI32 num_failed = 0;

    err = naive_default_allocator_init(&allocator);
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
    if (!err) {
        num_failed = naive_test_run(&test);
    }

    naive_default_allocator_finalize(&allocator);

    return (!err && num_failed == 0) ? 0 : ((int)err + NAIVE_ERR_CODES_COUNT + num_failed);
}
