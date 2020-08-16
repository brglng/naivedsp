#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "naivedsp/stereo-delay.h"

#define BLOCK_SIZE      256
#define SAMPLE_RATE     44100
#define MAX_DELAY_LEN   (10 * SAMPLE_RATE)
#define LEFT_DELAY_LEN  (0.2 * SAMPLE_RATE)
#define RIGHT_DELAY_LEN (0.7 * SAMPLE_RATE)

int main(int argc, char *argv[])
{
  if (argc < 3) {
    return -1;
  }

  FILE *fin = fopen(argv[1], "rb");
  FILE *fout = fopen(argv[2], "wb");

  NaiveDefaultAllocator allocator;
  naive_default_allocator_init(&allocator);

  NaiveStereoDelay delay;
  void *scratch = malloc(naive_stereo_delay_scratch_size(BLOCK_SIZE, LEFT_DELAY_LEN, RIGHT_DELAY_LEN));

  naive_stereo_delay_init(&delay, MAX_DELAY_LEN, BLOCK_SIZE, &allocator);
  naive_stereo_delay_set_left_delay_length(&delay, LEFT_DELAY_LEN);
  naive_stereo_delay_set_left_feedback_gain(&delay, 0.4f);
  naive_stereo_delay_set_left_mix_level(&delay, 0.5f);
  naive_stereo_delay_set_left_crossfeed_gain(&delay, 0.3f);
  naive_stereo_delay_set_right_delay_length(&delay, RIGHT_DELAY_LEN);
  naive_stereo_delay_set_right_feedback_gain(&delay, 0.4f);
  naive_stereo_delay_set_right_mix_level(&delay, 0.5f);
  naive_stereo_delay_set_right_crossfeed_gain(&delay, 0.3f);

  NaiveI16 *inout_i16 = malloc(sizeof(NaiveI16) * BLOCK_SIZE * 2);
  float *inout_float = malloc(sizeof(float) * BLOCK_SIZE * 2);

  float *left = malloc(sizeof(float) * BLOCK_SIZE + 8);
  float *right = malloc(sizeof(float) * BLOCK_SIZE + 8);

  size_t count;
  do {
    count = fread(inout_i16, sizeof(NaiveI16), BLOCK_SIZE * 2, fin);
    naive_i16_q15_to_float(inout_float, inout_i16, count);
    naive_interleaved_to_planar(left, right, inout_float, count / 2);

    naive_stereo_delay_process(&delay, left, right, count / 2, scratch);

    naive_planar_to_interleaved(inout_float, left, right, count / 2);
    naive_float_to_i16_q15(inout_i16, inout_float, count);
    fwrite(inout_i16, sizeof(NaiveI16), count, fout);
  } while (count == BLOCK_SIZE * 2);

  naive_stereo_delay_finalize(&delay);
  free(right);
  free(left);
  free(inout_float);
  free(inout_i16);
  free(scratch);
  fclose(fin);
  fclose(fout);

  return 0;
}
