#include "naivedsp/math.h"

void naive_gain(NaiveF32 *inout, NaiveI32 block_size, NaiveF32 gain)
{
    for (NaiveI32 i = 0; i < block_size; i++) {
        inout[i] *= gain;
    }
}

void naive_mix(NaiveF32 *output, NaiveF32 *input, NaiveI32 len)
{
    for (NaiveI32 i = 0; i < len; ++i) {
        output[i] += input[i];
    }
}

void naive_mix_with_gain(NaiveF32 *output, NaiveF32 *input, NaiveF32 gain, NaiveI32 len)
{
    for (NaiveI32 i = 0; i < len; ++i) {
        output[i] += gain * input[i];
    }
}
