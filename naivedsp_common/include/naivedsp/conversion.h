#ifndef __NAIVE_CONVERSION_H__
#define __NAIVE_CONVERSION_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "naivedsp/math_types.h"

void naive_i16_q15_to_f32(NaiveF32 *out, NaiveI16 *in, NaiveI32 block_size);
void naive_f32_to_i16_q15(NaiveI16 *out, NaiveF32 *in, NaiveI32 block_size);
void naive_i16_q15_interleaved_to_f32_planar(NaiveF32 **out, NaiveI16 *in,
                                           NaiveI32 num_channels,
                                           NaiveI32 block_size);
void naive_f32_planar_to_i16_q15_interleaved(NaiveI16 *out, NaiveF32 **in,
                                           NaiveI32 num_channels,
                                           NaiveI32 block_size);

void naive_stereo_interleaved_to_planar(NaiveF32 *left_out, NaiveF32 *right_out, NaiveF32 *in, NaiveI32 block_size);
void naive_stereo_planar_to_interleaved(NaiveF32 *out, NaiveF32 *left_in, NaiveF32 *right_in, NaiveI32 block_size);

#ifdef __cplusplus
}
#endif

#endif /* __NAIVE_CONVERSION_H__ */
