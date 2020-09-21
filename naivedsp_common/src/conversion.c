#include "naivedsp/conversion.h"
#include "naivedsp/math.h"

void naive_i16_q15_to_f32(NaiveF32 *out, NaiveI16 *in, NaiveI32 block_size)
{
    for (NaiveI32 i = 0; i < block_size; i++) {
        out[i] = NAIVE_SATURATE((NaiveF32)in[i] / (1 << 15));
    }
}

void naive_f32_to_i16_q15(NaiveI16 *out, NaiveF32 *in, NaiveI32 block_size)
{
    for (NaiveI32 i = 0; i < block_size; i++) {
        out[i] = (NaiveI16)roundf(NAIVE_SATURATE(in[i]) * (1 << 15));
    }
}

void naive_stereo_interleaved_to_planar(NaiveF32 *left_out, NaiveF32 *right_out, NaiveF32 *in, NaiveI32 block_size)
{
    for (NaiveI32 i = 0; i < block_size; i++) {
        left_out[i]  = in[2 * i];
        right_out[i] = in[2 * i + 1];
    }
}

void naive_stereo_planar_to_interleaved(NaiveF32 *out, NaiveF32 *left_in, NaiveF32 *right_in, NaiveI32 block_size)
{
    for (NaiveI32 i = 0; i < block_size; i++) {
        out[2 * i]     = left_in[i];
        out[2 * i + 1] = right_in[i];
    }
}

void naive_i16_q15_interleaved_to_f32_planar(NaiveF32 **out, NaiveI16 *in,
                                             NaiveI32 num_channels,
                                             NaiveI32 block_size)
{
    for (NaiveI32 j = 0; j < block_size; j++) {
        for (NaiveI32 i = 0; i < num_channels; i++) {
            out[i][j] = NAIVE_SATURATE((NaiveF32)in[num_channels * j + i] / (1 << 15));
        }
    }
}

void naive_f32_planar_to_i16_q15_interleaved(NaiveI16 *out, NaiveF32 **in,
                                             NaiveI32 num_channels,
                                             NaiveI32 block_size)
{
    for (NaiveI32 i = 0; i < num_channels; i++) {
        for (NaiveI32 j = 0; j < block_size; j++) {
            out[num_channels * j + i] = (NaiveI16)roundf(NAIVE_SATURATE(in[i][j]) * (1 << 15));
        }
    }
}
