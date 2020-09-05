#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "naivedsp/stereo_delay.h"
#include "naivedsp/test.h"

typedef struct {
    NaiveStereoDelay obj;
} TestContext;

NaiveErr set_params(void *_context, NAIVE_CONST TomlTable *config, NaiveI32 sample_rate)
{
    TestContext *context = _context;
    NaiveErr err = NAIVE_OK;

    err = naive_stereo_delay_set_left_delay_len(&context->obj, (NaiveI32)(toml_table_get_as_float(config, "left-delay-time") * (NaiveF32)sample_rate));

    if (!err) {
        err = naive_stereo_delay_set_left_feedback_gain(&context->obj, (NaiveF32)toml_table_get_as_float(config, "left-feedback-gain"));
    }
    if (!err) {
        err = naive_stereo_delay_set_left_crossfeed_gain(&context->obj, (NaiveF32)toml_table_get_as_float(config, "left-crossfeed-gain"));
    }
    if (!err) {
        err = naive_stereo_delay_set_left_dry_gain(&context->obj, (NaiveF32)toml_table_get_as_float(config, "left-dry-gain"));
    }
    if (!err) {
        err = naive_stereo_delay_set_left_wet_gain(&context->obj, (NaiveF32)toml_table_get_as_float(config, "left-wet-gain"));
    }
    if (!err) {
        err = naive_stereo_delay_set_right_delay_len(&context->obj, (NaiveI32)(toml_table_get_as_float(config, "right-delay-time") * (NaiveF32)sample_rate));
    }
    if (!err) {
        err = naive_stereo_delay_set_right_feedback_gain(&context->obj, (NaiveF32)toml_table_get_as_float(config, "right-feedback-gain"));
    }
    if (!err) {
        err = naive_stereo_delay_set_right_crossfeed_gain(&context->obj, (NaiveF32)toml_table_get_as_float(config, "right-crossfeed-gain"));
    }
    if (!err) {
        err = naive_stereo_delay_set_right_dry_gain(&context->obj, (NaiveF32)toml_table_get_as_float(config, "right-dry-gain"));
    }
    if (!err) {
        err = naive_stereo_delay_set_right_wet_gain(&context->obj, (NaiveF32)toml_table_get_as_float(config, "right-wet-gain"));
    }

    return err;
}

NaiveErr test_setup(void *_context, NAIVE_CONST TomlTable *config, NaiveI32 sample_rate)
{
    TestContext *context = _context;
    naive_stereo_delay_reset(&context->obj);
    naive_stereo_delay_set_default_params(&context->obj);
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
    memcpy(out[1], in[0], sizeof(NaiveF32) * (NaiveUSize)block_size);
    return naive_stereo_delay_process(&context->obj, out[0], out[1], block_size);
}

int main(void)
{
    NaiveErr err = 0;

    NaiveDefaultAllocator allocator;
    naive_default_allocator_init(&allocator);

    TestContext context;
    err = naive_stereo_delay_init(&context.obj, &allocator, &naive_default_alloc, 256, 88200);

    NaiveTest test;

    if (!err) {
        err = naive_test_init(&test,
                              &naive_default_alloc,
                              &allocator,
                              NAIVE_TEST_SOURCE_DIR "/config.toml",
                              NAIVE_TEST_INPUTS_DIR,
                              NAIVE_TEST_BINARY_DIR "/outputs",
                              NAIVE_TEST_SOURCE_DIR "/refs",
                              1,
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
