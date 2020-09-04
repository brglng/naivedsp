#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "naivedsp/delay.h"
#include "naivedsp/test.h"

typedef struct {
    NaiveDelay delay_obj;
} TestContext;

NaiveErr set_params(void *_context, NAIVE_CONST TomlTable *config, NaiveI32 sample_rate)
{
    TestContext *context = _context;
    NaiveErr err = NAIVE_OK;

    err = naive_delay_set_delay_len(&context->delay_obj, (NaiveI32)(toml_table_get_as_float(config, "delay-time") * (NaiveF32)sample_rate));

    if (!err) {
        err = naive_delay_set_feedback_gain(&context->delay_obj, (NaiveF32)toml_table_get_as_float(config, "feedback-gain"));
    }
    if (!err) {
        err = naive_delay_set_dry_gain(&context->delay_obj, (NaiveF32)toml_table_get_as_float(config, "dry-gain"));
    }
    if (!err) {
        err = naive_delay_set_wet_gain(&context->delay_obj, (NaiveF32)toml_table_get_as_float(config, "wet-gain"));
    }

    return err;
}

NaiveErr test_setup(void *_context, NAIVE_CONST TomlTable *config, NaiveI32 sample_rate)
{
    TestContext *context = _context;
    naive_delay_reset(&context->delay_obj);
    naive_delay_set_default_params(&context->delay_obj);
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
    naive_delay_process(&context->delay_obj, out[0], block_size);
    return NAIVE_OK;
}

int main(void)
{
    NaiveErr err = 0;

    NaiveDefaultAllocator allocator;
    naive_default_allocator_init(&allocator);

    TestContext context;
    err = naive_delay_init(&context.delay_obj, &allocator, &naive_default_alloc, 256, 88200);

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
                              1,
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
