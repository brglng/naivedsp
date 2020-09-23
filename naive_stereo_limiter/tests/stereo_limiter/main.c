#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "naivedsp/math.h"
#include "naivedsp/stereo_limiter.h"
#include "naivedsp/test.h"

typedef struct {
    NaiveStereoLimiter      stereo_limiter_obj;
    NaiveDefaultAllocator   allocator;
} TestContext;

NaiveErr set_params(void *_context, NAIVE_CONST TomlTable *config, NaiveI32 sample_rate)
{
    TestContext *context = _context;
    NaiveErr err = NAIVE_OK;

    (void)sample_rate;

    err = naive_stereo_limiter_set_input_gain(&context->stereo_limiter_obj, (NaiveF32)toml_table_get_as_float(config, "input-gain"));

    if (!err) {
        err = naive_stereo_limiter_set_output_gain(&context->stereo_limiter_obj, (NaiveF32)toml_table_get_as_float(config, "output-gain"));
    }
    if (!err) {
        NaiveF32 threshold_db = (NaiveF32)toml_table_get_as_float(config, "threshold");
        err = naive_stereo_limiter_set_threshold(&context->stereo_limiter_obj, NAIVE_FROM_DB(threshold_db));
    }
    if (!err) {
        err = naive_stereo_limiter_set_attack_time(&context->stereo_limiter_obj, (NaiveF32)toml_table_get_as_float(config, "attack-time"));
    }
    if (!err) {
        err = naive_stereo_limiter_set_release_time(&context->stereo_limiter_obj, (NaiveF32)toml_table_get_as_float(config, "release-time"));
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

    err = naive_stereo_limiter_init(&context->stereo_limiter_obj, &context->allocator, &naive_default_alloc, sample_rate, 100);
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
        naive_gain(in[0], in_len, sqrtf(2.0f) / 2.0f);
        memcpy(in[1], in[0], sizeof(NaiveF32) * (NaiveUSize)in_len);
    }

    *out_len = in_len;

    return naive_stereo_limiter_process(&context->stereo_limiter_obj, in[0], in[1], out[0], out[1], in_len);
}

int main(void)
{
    NaiveErr err = 0;
    NaiveDefaultAllocator allocator;
    NaiveI32 num_failed = 0;
    NaiveTest test;
    TestContext context;

    err = naive_default_allocator_init(&allocator);
    if (!err) {
        err = naive_stereo_limiter_init(&context.stereo_limiter_obj, &allocator, &naive_default_alloc, 100);
    }
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
