#include "test_desc.h"

NaiveResult init_test_desc(NaiveTestDesc *test_desc, NaiveAllocFunc *alloc, void *allocator)
{
    int err = 0;
    NaiveTestCaseDesc *case_desc = NULL;

    err = naive_test_desc_init(test_desc, alloc, allocator, 1);

    if (!err) err = naive_test_desc_add_case(test_desc, &case_desc);
    if (!err) err = naive_test_case_desc_init(case_desc, alloc, allocator, 7, "Delay");
    if (!err) err = naive_test_case_desc_set_str(case_desc, NAIVE_TEST_CASE_DESC_KEY_INPUT, "Sweep_-15dBFS_44k_16bit_1ch.pcm");
    if (!err) err = naive_test_case_desc_set_str(case_desc, NAIVE_TEST_CASE_DESC_KEY_OUTPUT, "delay-44k-time-1s-feedback-0.5-mix-0.5");
    if (!err) err = naive_test_case_desc_set_u32(case_desc, NAIVE_TEST_CASE_DESC_KEY_SAMPLE_RATE, 44100);
    if (!err) err = naive_test_case_desc_set_u32(case_desc, NAIVE_TEST_CASE_DESC_KEY_BLOCK_SIZE, 256);
    if (!err) err = naive_test_case_desc_set_f32(case_desc, KEY_DELAY_TIME, 1.0f);
    if (!err) err = naive_test_case_desc_set_f32(case_desc, KEY_FEEDBACK, 0.5f);
    if (!err) err = naive_test_case_desc_set_f32(case_desc, KEY_MIX, 0.5f);

    return err;
}
