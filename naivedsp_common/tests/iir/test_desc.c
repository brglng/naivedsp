#include "test_desc.h"

NaiveResult init_test_desc(NaiveTestDesc *test_desc, NaiveAllocFunc *alloc, void *allocator)
{
    int err = 0;
    NaiveTestCaseDesc *case_desc = NULL;

    err = naive_test_desc_init(test_desc, alloc, allocator, 19);

    if (!err) err = naive_test_desc_add_case(test_desc, &case_desc);
    if (!err) err = naive_test_case_desc_init(case_desc, alloc, allocator, 7, "IIR 1st Order Lowpass");
    if (!err) err = naive_test_case_desc_set_str(case_desc, NAIVE_TEST_CASE_DESC_KEY_INPUT, "Sweep_-15dBFS_44k_16bit_1ch.pcm");
    if (!err) err = naive_test_case_desc_set_str(case_desc, NAIVE_TEST_CASE_DESC_KEY_OUTPUT, "iir-1st-lowpass-44k-freq-10000.pcm");
    if (!err) err = naive_test_case_desc_set_i32(case_desc, NAIVE_TEST_CASE_DESC_KEY_SAMPLE_RATE, 44100);
    if (!err) err = naive_test_case_desc_set_i32(case_desc, NAIVE_TEST_CASE_DESC_KEY_BLOCK_SIZE, 256);
    if (!err) err = naive_test_case_desc_set_i32(case_desc, KEY_ORDER, 1);
    if (!err) err = naive_test_case_desc_set_int(case_desc, KEY_TYPE, TYPE_LOWPASS);
    if (!err) err = naive_test_case_desc_set_f32(case_desc, KEY_FREQ, 10000.0f);

    if (!err) err = naive_test_desc_add_case(test_desc, &case_desc);
    if (!err) err = naive_test_case_desc_init(case_desc, alloc, allocator, 7, "IIR 1st Order Highpass");
    if (!err) err = naive_test_case_desc_set_str(case_desc, NAIVE_TEST_CASE_DESC_KEY_INPUT, "Sweep_-15dBFS_44k_16bit_1ch.pcm");
    if (!err) err = naive_test_case_desc_set_str(case_desc, NAIVE_TEST_CASE_DESC_KEY_OUTPUT, "iir-1st-highpass-44k-freq-100.pcm");
    if (!err) err = naive_test_case_desc_set_i32(case_desc, NAIVE_TEST_CASE_DESC_KEY_SAMPLE_RATE, 44100);
    if (!err) err = naive_test_case_desc_set_i32(case_desc, NAIVE_TEST_CASE_DESC_KEY_BLOCK_SIZE, 256);
    if (!err) err = naive_test_case_desc_set_i32(case_desc, KEY_ORDER, 1);
    if (!err) err = naive_test_case_desc_set_int(case_desc, KEY_TYPE, TYPE_HIGHPASS);
    if (!err) err = naive_test_case_desc_set_f32(case_desc, KEY_FREQ, 100.0f);

    if (!err) err = naive_test_desc_add_case(test_desc, &case_desc);
    if (!err) err = naive_test_case_desc_init(case_desc, alloc, allocator, 7, "IIR 1st Order Allpass");
    if (!err) err = naive_test_case_desc_set_str(case_desc, NAIVE_TEST_CASE_DESC_KEY_INPUT, "Sweep_-15dBFS_44k_16bit_1ch.pcm");
    if (!err) err = naive_test_case_desc_set_str(case_desc, NAIVE_TEST_CASE_DESC_KEY_OUTPUT, "iir-1st-allpass-44k-freq-500.pcm");
    if (!err) err = naive_test_case_desc_set_i32(case_desc, NAIVE_TEST_CASE_DESC_KEY_SAMPLE_RATE, 44100);
    if (!err) err = naive_test_case_desc_set_i32(case_desc, NAIVE_TEST_CASE_DESC_KEY_BLOCK_SIZE, 256);
    if (!err) err = naive_test_case_desc_set_i32(case_desc, KEY_ORDER, 1);
    if (!err) err = naive_test_case_desc_set_int(case_desc, KEY_TYPE, TYPE_ALLPASS);
    if (!err) err = naive_test_case_desc_set_f32(case_desc, KEY_FREQ, 100.0f);

    if (!err) err = naive_test_desc_add_case(test_desc, &case_desc);
    if (!err) err = naive_test_case_desc_init(case_desc, alloc, allocator, 8, "IIR 2nd Order Lowpass");
    if (!err) err = naive_test_case_desc_set_str(case_desc, NAIVE_TEST_CASE_DESC_KEY_INPUT, "Sweep_-15dBFS_44k_16bit_1ch.pcm");
    if (!err) err = naive_test_case_desc_set_str(case_desc, NAIVE_TEST_CASE_DESC_KEY_OUTPUT, "iir-2nd-lowpass-44k-freq-10000-q-0.7.pcm");
    if (!err) err = naive_test_case_desc_set_i32(case_desc, NAIVE_TEST_CASE_DESC_KEY_SAMPLE_RATE, 44100);
    if (!err) err = naive_test_case_desc_set_i32(case_desc, NAIVE_TEST_CASE_DESC_KEY_BLOCK_SIZE, 256);
    if (!err) err = naive_test_case_desc_set_i32(case_desc, KEY_ORDER, 2);
    if (!err) err = naive_test_case_desc_set_int(case_desc, KEY_TYPE, TYPE_LOWPASS);
    if (!err) err = naive_test_case_desc_set_f32(case_desc, KEY_FREQ, 10000.0f);
    if (!err) err = naive_test_case_desc_set_f32(case_desc, KEY_Q, 0.7f);

    if (!err) err = naive_test_desc_add_case(test_desc, &case_desc);
    if (!err) err = naive_test_case_desc_init(case_desc, alloc, allocator, 8, "IIR 2nd Order Highpass");
    if (!err) err = naive_test_case_desc_set_str(case_desc, NAIVE_TEST_CASE_DESC_KEY_INPUT, "Sweep_-15dBFS_44k_16bit_1ch.pcm");
    if (!err) err = naive_test_case_desc_set_str(case_desc, NAIVE_TEST_CASE_DESC_KEY_OUTPUT, "iir-2nd-highpass-44k-freq-100-q-0.7.pcm");
    if (!err) err = naive_test_case_desc_set_i32(case_desc, NAIVE_TEST_CASE_DESC_KEY_SAMPLE_RATE, 44100);
    if (!err) err = naive_test_case_desc_set_i32(case_desc, NAIVE_TEST_CASE_DESC_KEY_BLOCK_SIZE, 256);
    if (!err) err = naive_test_case_desc_set_i32(case_desc, KEY_ORDER, 2);
    if (!err) err = naive_test_case_desc_set_int(case_desc, KEY_TYPE, TYPE_HIGHPASS);
    if (!err) err = naive_test_case_desc_set_f32(case_desc, KEY_FREQ, 100.0f);
    if (!err) err = naive_test_case_desc_set_f32(case_desc, KEY_Q, 0.7f);

    if (!err) err = naive_test_desc_add_case(test_desc, &case_desc);
    if (!err) err = naive_test_case_desc_init(case_desc, alloc, allocator, 8, "IIR 2nd Order Bandpass");
    if (!err) err = naive_test_case_desc_set_str(case_desc, NAIVE_TEST_CASE_DESC_KEY_INPUT, "Sweep_-15dBFS_44k_16bit_1ch.pcm");
    if (!err) err = naive_test_case_desc_set_str(case_desc, NAIVE_TEST_CASE_DESC_KEY_OUTPUT, "iir-2nd-bandpass-44k-freq-500-q-0.7.pcm");
    if (!err) err = naive_test_case_desc_set_i32(case_desc, NAIVE_TEST_CASE_DESC_KEY_SAMPLE_RATE, 44100);
    if (!err) err = naive_test_case_desc_set_i32(case_desc, NAIVE_TEST_CASE_DESC_KEY_BLOCK_SIZE, 256);
    if (!err) err = naive_test_case_desc_set_i32(case_desc, KEY_ORDER, 2);
    if (!err) err = naive_test_case_desc_set_int(case_desc, KEY_TYPE, TYPE_BANDPASS);
    if (!err) err = naive_test_case_desc_set_f32(case_desc, KEY_FREQ, 500.0f);
    if (!err) err = naive_test_case_desc_set_f32(case_desc, KEY_Q, 0.7f);

    if (!err) err = naive_test_desc_add_case(test_desc, &case_desc);
    if (!err) err = naive_test_case_desc_init(case_desc, alloc, allocator, 8, "IIR 2nd Order Bandstop");
    if (!err) err = naive_test_case_desc_set_str(case_desc, NAIVE_TEST_CASE_DESC_KEY_INPUT, "Sweep_-15dBFS_44k_16bit_1ch.pcm");
    if (!err) err = naive_test_case_desc_set_str(case_desc, NAIVE_TEST_CASE_DESC_KEY_OUTPUT, "iir-2nd-bandstop-44k-freq-500-q-0.7.pcm");
    if (!err) err = naive_test_case_desc_set_i32(case_desc, NAIVE_TEST_CASE_DESC_KEY_SAMPLE_RATE, 44100);
    if (!err) err = naive_test_case_desc_set_i32(case_desc, NAIVE_TEST_CASE_DESC_KEY_BLOCK_SIZE, 256);
    if (!err) err = naive_test_case_desc_set_i32(case_desc, KEY_ORDER, 2);
    if (!err) err = naive_test_case_desc_set_int(case_desc, KEY_TYPE, TYPE_BANDSTOP);
    if (!err) err = naive_test_case_desc_set_f32(case_desc, KEY_FREQ, 500.0f);
    if (!err) err = naive_test_case_desc_set_f32(case_desc, KEY_Q, 0.7f);

    if (!err) err = naive_test_desc_add_case(test_desc, &case_desc);
    if (!err) err = naive_test_case_desc_init(case_desc, alloc, allocator, 8, "IIR 2nd Order Allpass");
    if (!err) err = naive_test_case_desc_set_str(case_desc, NAIVE_TEST_CASE_DESC_KEY_INPUT, "Sweep_-15dBFS_44k_16bit_1ch.pcm");
    if (!err) err = naive_test_case_desc_set_str(case_desc, NAIVE_TEST_CASE_DESC_KEY_OUTPUT, "iir-2nd-allpass-44k-freq-500-q-0.7.pcm");
    if (!err) err = naive_test_case_desc_set_i32(case_desc, NAIVE_TEST_CASE_DESC_KEY_SAMPLE_RATE, 44100);
    if (!err) err = naive_test_case_desc_set_i32(case_desc, NAIVE_TEST_CASE_DESC_KEY_BLOCK_SIZE, 256);
    if (!err) err = naive_test_case_desc_set_i32(case_desc, KEY_ORDER, 2);
    if (!err) err = naive_test_case_desc_set_int(case_desc, KEY_TYPE, TYPE_ALLPASS);
    if (!err) err = naive_test_case_desc_set_f32(case_desc, KEY_FREQ, 500.0f);
    if (!err) err = naive_test_case_desc_set_f32(case_desc, KEY_Q, 0.7f);

    if (!err) err = naive_test_desc_add_case(test_desc, &case_desc);
    if (!err) err = naive_test_case_desc_init(case_desc, alloc, allocator, 9, "IIR 2nd Order Low Shelf Boost");
    if (!err) err = naive_test_case_desc_set_str(case_desc, NAIVE_TEST_CASE_DESC_KEY_INPUT, "Sweep_-15dBFS_44k_16bit_1ch.pcm");
    if (!err) err = naive_test_case_desc_set_str(case_desc, NAIVE_TEST_CASE_DESC_KEY_OUTPUT, "iir-2nd-low-shelf-44k-freq-200-q-0.7-gain-2.0.pcm");
    if (!err) err = naive_test_case_desc_set_i32(case_desc, NAIVE_TEST_CASE_DESC_KEY_SAMPLE_RATE, 44100);
    if (!err) err = naive_test_case_desc_set_i32(case_desc, NAIVE_TEST_CASE_DESC_KEY_BLOCK_SIZE, 256);
    if (!err) err = naive_test_case_desc_set_i32(case_desc, KEY_ORDER, 2);
    if (!err) err = naive_test_case_desc_set_int(case_desc, KEY_TYPE, TYPE_LOW_SHELF);
    if (!err) err = naive_test_case_desc_set_f32(case_desc, KEY_FREQ, 200.0f);
    if (!err) err = naive_test_case_desc_set_f32(case_desc, KEY_Q, 0.7f);
    if (!err) err = naive_test_case_desc_set_f32(case_desc, KEY_GAIN, 2.0f);

    if (!err) err = naive_test_desc_add_case(test_desc, &case_desc);
    if (!err) err = naive_test_case_desc_init(case_desc, alloc, allocator, 9, "IIR 2nd Order Low Shelf Cut");
    if (!err) err = naive_test_case_desc_set_str(case_desc, NAIVE_TEST_CASE_DESC_KEY_INPUT, "Sweep_-15dBFS_44k_16bit_1ch.pcm");
    if (!err) err = naive_test_case_desc_set_str(case_desc, NAIVE_TEST_CASE_DESC_KEY_OUTPUT, "iir-2nd-low-shelf-44k-freq-200-q-0.7-gain-0.5.pcm");
    if (!err) err = naive_test_case_desc_set_i32(case_desc, NAIVE_TEST_CASE_DESC_KEY_SAMPLE_RATE, 44100);
    if (!err) err = naive_test_case_desc_set_i32(case_desc, NAIVE_TEST_CASE_DESC_KEY_BLOCK_SIZE, 256);
    if (!err) err = naive_test_case_desc_set_i32(case_desc, KEY_ORDER, 2);
    if (!err) err = naive_test_case_desc_set_int(case_desc, KEY_TYPE, TYPE_LOW_SHELF);
    if (!err) err = naive_test_case_desc_set_f32(case_desc, KEY_FREQ, 200.0f);
    if (!err) err = naive_test_case_desc_set_f32(case_desc, KEY_Q, 0.7f);
    if (!err) err = naive_test_case_desc_set_f32(case_desc, KEY_GAIN, 0.5f);

    if (!err) err = naive_test_desc_add_case(test_desc, &case_desc);
    if (!err) err = naive_test_case_desc_init(case_desc, alloc, allocator, 9, "IIR 2nd Order High Shelf Boost");
    if (!err) err = naive_test_case_desc_set_str(case_desc, NAIVE_TEST_CASE_DESC_KEY_INPUT, "Sweep_-15dBFS_44k_16bit_1ch.pcm");
    if (!err) err = naive_test_case_desc_set_str(case_desc, NAIVE_TEST_CASE_DESC_KEY_OUTPUT, "iir-2nd-high-shelf-44k-freq-5000-q-0.7-gain-2.0.pcm");
    if (!err) err = naive_test_case_desc_set_i32(case_desc, NAIVE_TEST_CASE_DESC_KEY_SAMPLE_RATE, 44100);
    if (!err) err = naive_test_case_desc_set_i32(case_desc, NAIVE_TEST_CASE_DESC_KEY_BLOCK_SIZE, 256);
    if (!err) err = naive_test_case_desc_set_i32(case_desc, KEY_ORDER, 2);
    if (!err) err = naive_test_case_desc_set_int(case_desc, KEY_TYPE, TYPE_HIGH_SHELF);
    if (!err) err = naive_test_case_desc_set_f32(case_desc, KEY_FREQ, 5000.0f);
    if (!err) err = naive_test_case_desc_set_f32(case_desc, KEY_Q, 0.7f);
    if (!err) err = naive_test_case_desc_set_f32(case_desc, KEY_GAIN, 2.0f);

    if (!err) err = naive_test_desc_add_case(test_desc, &case_desc);
    if (!err) err = naive_test_case_desc_init(case_desc, alloc, allocator, 9, "IIR 2nd Order High Shelf Cut");
    if (!err) err = naive_test_case_desc_set_str(case_desc, NAIVE_TEST_CASE_DESC_KEY_INPUT, "Sweep_-15dBFS_44k_16bit_1ch.pcm");
    if (!err) err = naive_test_case_desc_set_str(case_desc, NAIVE_TEST_CASE_DESC_KEY_OUTPUT, "iir-2nd-high-shelf-44k-freq-5000-q-0.7-gain-0.5.pcm");
    if (!err) err = naive_test_case_desc_set_i32(case_desc, NAIVE_TEST_CASE_DESC_KEY_SAMPLE_RATE, 44100);
    if (!err) err = naive_test_case_desc_set_i32(case_desc, NAIVE_TEST_CASE_DESC_KEY_BLOCK_SIZE, 256);
    if (!err) err = naive_test_case_desc_set_i32(case_desc, KEY_ORDER, 2);
    if (!err) err = naive_test_case_desc_set_int(case_desc, KEY_TYPE, TYPE_HIGH_SHELF);
    if (!err) err = naive_test_case_desc_set_f32(case_desc, KEY_FREQ, 5000.0f);
    if (!err) err = naive_test_case_desc_set_f32(case_desc, KEY_Q, 0.7f);
    if (!err) err = naive_test_case_desc_set_f32(case_desc, KEY_GAIN, 0.5f);

    if (!err) err = naive_test_desc_add_case(test_desc, &case_desc);
    if (!err) err = naive_test_case_desc_init(case_desc, alloc, allocator, 9, "IIR 2nd Order Peak");
    if (!err) err = naive_test_case_desc_set_str(case_desc, NAIVE_TEST_CASE_DESC_KEY_INPUT, "Sweep_-15dBFS_44k_16bit_1ch.pcm");
    if (!err) err = naive_test_case_desc_set_str(case_desc, NAIVE_TEST_CASE_DESC_KEY_OUTPUT, "iir-2nd-peak-44k-freq-500-q-0.7-gain-2.0.pcm");
    if (!err) err = naive_test_case_desc_set_i32(case_desc, NAIVE_TEST_CASE_DESC_KEY_SAMPLE_RATE, 44100);
    if (!err) err = naive_test_case_desc_set_i32(case_desc, NAIVE_TEST_CASE_DESC_KEY_BLOCK_SIZE, 256);
    if (!err) err = naive_test_case_desc_set_i32(case_desc, KEY_ORDER, 2);
    if (!err) err = naive_test_case_desc_set_int(case_desc, KEY_TYPE, TYPE_PEAK);
    if (!err) err = naive_test_case_desc_set_f32(case_desc, KEY_FREQ, 500.0f);
    if (!err) err = naive_test_case_desc_set_f32(case_desc, KEY_Q, 0.7f);
    if (!err) err = naive_test_case_desc_set_f32(case_desc, KEY_GAIN, 2.0f);

    if (!err) err = naive_test_desc_add_case(test_desc, &case_desc);
    if (!err) err = naive_test_case_desc_init(case_desc, alloc, allocator, 7, "IIR 1st Order Butterworth Lowpass");
    if (!err) err = naive_test_case_desc_set_str(case_desc, NAIVE_TEST_CASE_DESC_KEY_INPUT, "Sweep_-15dBFS_44k_16bit_1ch.pcm");
    if (!err) err = naive_test_case_desc_set_str(case_desc, NAIVE_TEST_CASE_DESC_KEY_OUTPUT, "iir-1st-butterworth-lowpass-44k-freq-10000.pcm");
    if (!err) err = naive_test_case_desc_set_i32(case_desc, NAIVE_TEST_CASE_DESC_KEY_SAMPLE_RATE, 44100);
    if (!err) err = naive_test_case_desc_set_i32(case_desc, NAIVE_TEST_CASE_DESC_KEY_BLOCK_SIZE, 256);
    if (!err) err = naive_test_case_desc_set_i32(case_desc, KEY_ORDER, 1);
    if (!err) err = naive_test_case_desc_set_int(case_desc, KEY_TYPE, TYPE_BUTTERWORTH_LOWPASS);
    if (!err) err = naive_test_case_desc_set_f32(case_desc, KEY_FREQ, 10000.0f);

    if (!err) err = naive_test_desc_add_case(test_desc, &case_desc);
    if (!err) err = naive_test_case_desc_init(case_desc, alloc, allocator, 7, "IIR 1st Order Butterworth Highpass");
    if (!err) err = naive_test_case_desc_set_str(case_desc, NAIVE_TEST_CASE_DESC_KEY_INPUT, "Sweep_-15dBFS_44k_16bit_1ch.pcm");
    if (!err) err = naive_test_case_desc_set_str(case_desc, NAIVE_TEST_CASE_DESC_KEY_OUTPUT, "iir-1st-butterworth-highpass-44k-freq-100.pcm");
    if (!err) err = naive_test_case_desc_set_i32(case_desc, NAIVE_TEST_CASE_DESC_KEY_SAMPLE_RATE, 44100);
    if (!err) err = naive_test_case_desc_set_i32(case_desc, NAIVE_TEST_CASE_DESC_KEY_BLOCK_SIZE, 256);
    if (!err) err = naive_test_case_desc_set_i32(case_desc, KEY_ORDER, 1);
    if (!err) err = naive_test_case_desc_set_int(case_desc, KEY_TYPE, TYPE_BUTTERWORTH_HIGHPASS);
    if (!err) err = naive_test_case_desc_set_f32(case_desc, KEY_FREQ, 100.0f);

    if (!err) err = naive_test_desc_add_case(test_desc, &case_desc);
    if (!err) err = naive_test_case_desc_init(case_desc, alloc, allocator, 7, "IIR 2nd Order Butterworth Lowpass");
    if (!err) err = naive_test_case_desc_set_str(case_desc, NAIVE_TEST_CASE_DESC_KEY_INPUT, "Sweep_-15dBFS_44k_16bit_1ch.pcm");
    if (!err) err = naive_test_case_desc_set_str(case_desc, NAIVE_TEST_CASE_DESC_KEY_OUTPUT, "iir-2nd-butterworth-lowpass-44k-freq-10000.pcm");
    if (!err) err = naive_test_case_desc_set_i32(case_desc, NAIVE_TEST_CASE_DESC_KEY_SAMPLE_RATE, 44100);
    if (!err) err = naive_test_case_desc_set_i32(case_desc, NAIVE_TEST_CASE_DESC_KEY_BLOCK_SIZE, 256);
    if (!err) err = naive_test_case_desc_set_i32(case_desc, KEY_ORDER, 2);
    if (!err) err = naive_test_case_desc_set_int(case_desc, KEY_TYPE, TYPE_BUTTERWORTH_LOWPASS);
    if (!err) err = naive_test_case_desc_set_f32(case_desc, KEY_FREQ, 10000.0f);

    if (!err) err = naive_test_desc_add_case(test_desc, &case_desc);
    if (!err) err = naive_test_case_desc_init(case_desc, alloc, allocator, 7, "IIR 2nd Order Butterworth Highpass");
    if (!err) err = naive_test_case_desc_set_str(case_desc, NAIVE_TEST_CASE_DESC_KEY_INPUT, "Sweep_-15dBFS_44k_16bit_1ch.pcm");
    if (!err) err = naive_test_case_desc_set_str(case_desc, NAIVE_TEST_CASE_DESC_KEY_OUTPUT, "iir-2nd-butterworth-highpass-44k-freq-100.pcm");
    if (!err) err = naive_test_case_desc_set_i32(case_desc, NAIVE_TEST_CASE_DESC_KEY_SAMPLE_RATE, 44100);
    if (!err) err = naive_test_case_desc_set_i32(case_desc, NAIVE_TEST_CASE_DESC_KEY_BLOCK_SIZE, 256);
    if (!err) err = naive_test_case_desc_set_i32(case_desc, KEY_ORDER, 2);
    if (!err) err = naive_test_case_desc_set_int(case_desc, KEY_TYPE, TYPE_BUTTERWORTH_HIGHPASS);
    if (!err) err = naive_test_case_desc_set_f32(case_desc, KEY_FREQ, 100.0f);

    if (!err) err = naive_test_desc_add_case(test_desc, &case_desc);
    if (!err) err = naive_test_case_desc_init(case_desc, alloc, allocator, 8, "IIR 2nd Order Butterworth Bandpass");
    if (!err) err = naive_test_case_desc_set_str(case_desc, NAIVE_TEST_CASE_DESC_KEY_INPUT, "Sweep_-15dBFS_44k_16bit_1ch.pcm");
    if (!err) err = naive_test_case_desc_set_str(case_desc, NAIVE_TEST_CASE_DESC_KEY_OUTPUT, "iir-2nd-butterworth-bandpass-44k-freq-500-q-0.7.pcm");
    if (!err) err = naive_test_case_desc_set_i32(case_desc, NAIVE_TEST_CASE_DESC_KEY_SAMPLE_RATE, 44100);
    if (!err) err = naive_test_case_desc_set_i32(case_desc, NAIVE_TEST_CASE_DESC_KEY_BLOCK_SIZE, 256);
    if (!err) err = naive_test_case_desc_set_i32(case_desc, KEY_ORDER, 2);
    if (!err) err = naive_test_case_desc_set_int(case_desc, KEY_TYPE, TYPE_BUTTERWORTH_BANDPASS);
    if (!err) err = naive_test_case_desc_set_f32(case_desc, KEY_FREQ, 500.0f);
    if (!err) err = naive_test_case_desc_set_f32(case_desc, KEY_Q, 0.7f);

    if (!err) err = naive_test_desc_add_case(test_desc, &case_desc);
    if (!err) err = naive_test_case_desc_init(case_desc, alloc, allocator, 8, "IIR 2nd Order Butterworth Bandstop");
    if (!err) err = naive_test_case_desc_set_str(case_desc, NAIVE_TEST_CASE_DESC_KEY_INPUT, "Sweep_-15dBFS_44k_16bit_1ch.pcm");
    if (!err) err = naive_test_case_desc_set_str(case_desc, NAIVE_TEST_CASE_DESC_KEY_OUTPUT, "iir-2nd-butterworth-bandstop-44k-freq-500-q-0.7.pcm");
    if (!err) err = naive_test_case_desc_set_i32(case_desc, NAIVE_TEST_CASE_DESC_KEY_SAMPLE_RATE, 44100);
    if (!err) err = naive_test_case_desc_set_i32(case_desc, NAIVE_TEST_CASE_DESC_KEY_BLOCK_SIZE, 256);
    if (!err) err = naive_test_case_desc_set_i32(case_desc, KEY_ORDER, 2);
    if (!err) err = naive_test_case_desc_set_int(case_desc, KEY_TYPE, TYPE_BUTTERWORTH_BANDSTOP);
    if (!err) err = naive_test_case_desc_set_f32(case_desc, KEY_FREQ, 500.0f);
    if (!err) err = naive_test_case_desc_set_f32(case_desc, KEY_Q, 0.7f);

    return err;
}
