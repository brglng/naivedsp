#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "naivedsp/geq.h"
#include "naivedsp/geq_design.h"
#include "naivedsp/iir_design.h"
#include "naivedsp/test.h"
#include "test_desc.h"

typedef struct {
    NaiveGeqStates            states;
    NaiveGeqCoeffs            coeffs;
    NaiveGeqCoeffs            design_coeffs;
    void                    *scratch;
} TestContext;

NaiveResult set_params(void *_context, NAIVE_CONST NaiveTestCaseDesc *case_desc, NaiveU32 sample_rate)
{
    TestContext *context = _context;

    NaiveU32 num_bands = naive_test_case_desc_get_u32(case_desc, KEY_NUM_BANDS);
    NaiveU32 order = naive_test_case_desc_get_u32(case_desc, KEY_ORDER);
    NaiveF32 freqs[] = {
        naive_test_case_desc_get_f32(case_desc, KEY_FREQ0),
        naive_test_case_desc_get_f32(case_desc, KEY_FREQ1),
        naive_test_case_desc_get_f32(case_desc, KEY_FREQ2),
        naive_test_case_desc_get_f32(case_desc, KEY_FREQ3),
    };

    naive_geq_coeffs_reset(&context->design_coeffs);

    int err = naive_geq_coeffs_design_butterworth_bands(&context->design_coeffs, order, sample_rate, num_bands, freqs);
    if (err)
        return err;

    for (NaiveU32 i = 0; i < num_bands; ++i) {
        NaiveF32 gain = naive_test_case_desc_get_f32(case_desc, KEY_BAND0_GAIN + i);
        err = naive_geq_coeffs_set_band_gain(&context->design_coeffs, i, gain);
        if (err)
            return err;
    }

    err = naive_geq_set_coeffs(&context->states, &context->coeffs, &context->design_coeffs);
    if (err)
        return err;

    return NAIVE_OK;
}

NaiveResult test_setup(void *_context, NAIVE_CONST NaiveTestCaseDesc *case_desc, NaiveU32 sample_rate)
{
    TestContext *context = _context;
    naive_geq_reset(&context->states, &context->coeffs);
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
    naive_geq_process(&context->states, &context->coeffs, out[0], block_size, context->scratch);
    return NAIVE_OK;
}

int main(void)
{
    int err = 0;

    NaiveDefaultAllocator allocator;
    naive_default_allocator_init(&allocator);

    TestContext context;
    err = naive_geq_coeffs_init(&context.coeffs, &naive_default_alloc, &allocator, naive_geq_calc_num_iirs(20), naive_iir_calc_num_sos(10));

    if (!err) {
        err = naive_geq_states_init(&context.states, &naive_default_alloc, &allocator, naive_geq_calc_num_iirs(20), naive_iir_calc_num_sos(10));
    }

    if (!err) {
        err = naive_geq_coeffs_init(&context.design_coeffs, &naive_default_alloc, &allocator, naive_geq_calc_num_iirs(20), naive_iir_calc_num_sos(10));
    }

    if (!err) {
        context.scratch = naive_default_alloc(&allocator, NAIVE_MEM_SCRATCH, 8, NAIVE_GEQ_SCRATCH_SIZE(256));
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
