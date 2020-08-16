#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "naivedsp/delay.h"
#include "naivedsp/test.h"
#include "test_desc.h"

typedef struct {
    NaiveDelayStates          states;
    NaiveDelayParams          params;
    void                    *scratch;
} TestContext;

NaiveResult set_params(void *_context, NAIVE_CONST NaiveTestCaseDesc *case_desc, NaiveU32 sample_rate)
{
    TestContext *context = _context;
    NaiveResult err = NAIVE_OK;

    context->params.delay_len = (NaiveU32)(naive_test_case_desc_get_f32(case_desc, KEY_DELAY_TIME) * sample_rate);
    context->params.feedback_gain = naive_test_case_desc_get_f32(case_desc, KEY_FEEDBACK);
    context->params.mix_level = naive_test_case_desc_get_f32(case_desc, KEY_MIX);

    return err;
}

NaiveResult test_setup(void *_context, NAIVE_CONST NaiveTestCaseDesc *case_desc, NaiveU32 sample_rate)
{
    TestContext *context = _context;
    naive_delay_reset(&context->states);
    return set_params(context, case_desc, sample_rate);
}

void test_teardown(void *_context)
{
    (void)_context;
}

NaiveResult test_process(void *_context, NaiveF32 **in, NaiveF32 **out, NaiveU32 block_size)
{
    TestContext *context = _context;
    memcpy(out[0], in[0], sizeof(NaiveF32) * block_size);
    naive_delay_process(&context->states, &context->params, out[0], block_size, context->scratch);
    return NAIVE_OK;
}

int main(void)
{
    int err = 0;

    NaiveDefaultAllocator allocator;
    naive_default_allocator_init(&allocator);

    TestContext context;
    naive_delay_params_init(&context.params);
    err = naive_delay_states_init(&context.states, &naive_default_alloc, &allocator, 88200);

    if (!err) {
        context.scratch = naive_default_alloc(&allocator, NAIVE_MEM_SCRATCH, 8, NAIVE_DELAY_SCRATCH_SIZE(256, 88200));
        if (!context.scratch)
            err = NAIVE_ERR_NOMEM;
    }

    NaiveTestDesc test_desc;
    if (!err) {
        err = init_test_desc(&test_desc, &naive_default_alloc, &allocator);
    }

    NaiveTest test;

    if (!err) {
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
