#include "naivedsp/conversion.h"
#include "opt.h"

#if !NAIVE_I16_Q15_TO_FLOAT_OPTIMIZED
void naive_i16_q15_to_f32(NaiveF32* out, NaiveI16* in, NaiveI32 block_size)
{
    for (NaiveI32 i = 0; i < block_size; i++) {
        out[i] = (NaiveF32)in[i] / (1 << 15);
    }
}
#endif

#if !NAIVE_FLOAT_TO_I16_Q15_OPTIMIZED
void naive_f32_to_i16_q15(NaiveI16* out, NaiveF32* in, NaiveI32 block_size)
{
    for (NaiveI32 i = 0; i < block_size; i++) {
        out[i] = (NaiveI16)(in[i] * (1 << 15));
    }
}
#endif

void naive_interleaved_to_planar(NaiveF32* left_out, NaiveF32* right_out, NaiveF32* in, NaiveI32 block_size)
{
    for (NaiveI32 i = 0; i < block_size; i++) {
        left_out[i]  = in[2 * i];
        right_out[i] = in[2 * i + 1];
    }
}

void naive_planar_to_interleaved(NaiveF32* out, NaiveF32* left_in, NaiveF32* right_in, NaiveI32 block_size)
{
    for (NaiveI32 i = 0; i < block_size; i++) {
        out[2 * i]     = left_in[i];
        out[2 * i + 1] = right_in[i];
    }
}

#if !NAIVE_I16_Q15_INTERLEAVED_TO_FLOAT_PLANAR_OPTIMIZED
void naive_i16_q15_interleaved_to_f32_planar(NaiveF32** out, NaiveI16* in,
                                           NaiveI32 num_channels,
                                           NaiveI32 block_size)
{
    for (NaiveI32 j = 0; j < block_size; j++) {
        for (NaiveI32 i = 0; i < num_channels; i++) {
            out[i][j] = (NaiveF32)in[num_channels * j + i] / (1 << 15);
        }
    }
}
#endif

#if !NAIVE_FLOAT_PLANAR_TO_I16_Q15_INTERLEAVED_OPTIMIZED
void naive_f32_planar_to_i16_q15_interleaved(NaiveI16* out, NaiveF32** in,
                                           NaiveI32 num_channels,
                                           NaiveI32 block_size)
{
    for (NaiveI32 i = 0; i < num_channels; i++) {
        for (NaiveI32 j = 0; j < block_size; j++) {
            out[num_channels * j + i] = (NaiveI16)(in[i][j] * (1 << 15));
        }
    }
}
#endif
