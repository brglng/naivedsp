#include "test_desc.h"

NaiveResult init_test_desc(NaiveTestDesc *test_desc, NaiveAllocFunc *alloc, void *allocator)
{
    int err = 0;
    NaiveTestCaseDesc *case_desc = NULL;

    err = naive_test_desc_init(test_desc, alloc, allocator, 1);

    if (!err) err = naive_test_desc_add_case(test_desc, &case_desc);
    if (!err) err = naive_test_case_desc_init(case_desc, alloc, allocator, 15, "5 Band 3rd Order Butterworth GEQ with Lowest Band Frequency of 30Hz");
    if (!err) err = naive_test_case_desc_set_str(case_desc, NAIVE_TEST_CASE_DESC_KEY_INPUT, "Sweep_-15dBFS_44k_16bit_1ch.pcm");
    if (!err) err = naive_test_case_desc_set_str(case_desc, NAIVE_TEST_CASE_DESC_KEY_OUTPUT, "geq-44k-5-bands-order-3-freqs-30-150-750-3750-gains-0.5-1.0-0.25-2.0-1.4.pcm");
    if (!err) err = naive_test_case_desc_set_u32(case_desc, NAIVE_TEST_CASE_DESC_KEY_SAMPLE_RATE, 44100);
    if (!err) err = naive_test_case_desc_set_u32(case_desc, NAIVE_TEST_CASE_DESC_KEY_BLOCK_SIZE, 256);
    if (!err) err = naive_test_case_desc_set_u32(case_desc, KEY_NUM_BANDS, 5);
    if (!err) err = naive_test_case_desc_set_u32(case_desc, KEY_ORDER, 3);
    if (!err) err = naive_test_case_desc_set_f32(case_desc, KEY_FREQ0, 30.0f);
    if (!err) err = naive_test_case_desc_set_f32(case_desc, KEY_FREQ1, 150.0f);
    if (!err) err = naive_test_case_desc_set_f32(case_desc, KEY_FREQ2, 750.0f);
    if (!err) err = naive_test_case_desc_set_f32(case_desc, KEY_FREQ3, 3750.0f);
    if (!err) err = naive_test_case_desc_set_f32(case_desc, KEY_BAND0_GAIN, 0.5f);
    if (!err) err = naive_test_case_desc_set_f32(case_desc, KEY_BAND1_GAIN, 1.0f);
    if (!err) err = naive_test_case_desc_set_f32(case_desc, KEY_BAND2_GAIN, 0.25f);
    if (!err) err = naive_test_case_desc_set_f32(case_desc, KEY_BAND3_GAIN, 2.0f);
    if (!err) err = naive_test_case_desc_set_f32(case_desc, KEY_BAND4_GAIN, 1.4f);

    return err;
}
