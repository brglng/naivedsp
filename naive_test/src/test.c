#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "naivedsp/math.h"
#include "naivedsp/conversion.h"
#include "naivedsp/memory.h"
#include "naivedsp/test.h"

NaiveResult naive_test_init(NaiveTest *self,
                        NaiveAllocFunc alloc,
                        void *allocator,
                        NAIVE_CONST NaiveTestDesc *test_desc,
                        NAIVE_CONST char *inputs_dir,
                        NAIVE_CONST char *outputs_dir,
                        NAIVE_CONST char *refs_dir,
                        NaiveI32 num_in_channels,
                        NaiveI32 num_out_channels,
                        NaiveI32 max_block_size,
                        NaiveTestSetUpFunc *setup,
                        NaiveTestTearDownFunc *teardown,
                        NaiveTestProcessFunc *process,
                        void *context)
{
  self->inputs_dir = inputs_dir;
  self->outputs_dir = outputs_dir;
  self->refs_dir = refs_dir;
  self->test_desc = test_desc;
  self->num_in_channels = num_in_channels;
  self->num_out_channels = num_out_channels;
  self->max_block_size = max_block_size;
  self->setup = setup;
  self->teardown = teardown;
  self->process = process;
  self->context = context;

  self->in_fname = alloc(allocator, NAIVE_MEM_STATE, NAIVE_ALIGNOF(char), sizeof(char) * (NAIVE_TEST_FILENAME_MAX + 1));
  if (!self->in_fname)
    return NAIVE_ERR_NOMEM;

  self->out_fname = alloc(allocator, NAIVE_MEM_STATE, NAIVE_ALIGNOF(char), sizeof(char) * (NAIVE_TEST_FILENAME_MAX + 1));
  if (!self->out_fname)
    return NAIVE_ERR_NOMEM;

  self->ref_fname = alloc(allocator, NAIVE_MEM_STATE, NAIVE_ALIGNOF(char), sizeof(char) * (NAIVE_TEST_FILENAME_MAX + 1));
  if (!self->ref_fname)
    return NAIVE_ERR_NOMEM;

  self->in_i16 = alloc(allocator, NAIVE_MEM_STATE, NAIVE_ALIGNOF(NaiveI16), sizeof(NaiveI16) * num_in_channels * max_block_size);
  if (!self->in_i16)
    return NAIVE_ERR_NOMEM;

  self->out_i16 = alloc(allocator, NAIVE_MEM_STATE, NAIVE_ALIGNOF(NaiveI16), sizeof(NaiveI16) * num_out_channels * max_block_size);
  if (!self->out_i16)
    return NAIVE_ERR_NOMEM;

  self->ref_i16 = alloc(allocator, NAIVE_MEM_STATE, NAIVE_ALIGNOF(NaiveI16), sizeof(NaiveI16) * num_out_channels * max_block_size);
  if (!self->ref_i16)
    return NAIVE_ERR_NOMEM;

  self->in_f32 = alloc(allocator, NAIVE_MEM_STATE, NAIVE_ALIGNOF(NaiveF32 *), sizeof(NaiveF32 *) * num_in_channels);
  if (!self->in_f32)
    return NAIVE_ERR_NOMEM;

  self->out_f32 = alloc(allocator, NAIVE_MEM_STATE, NAIVE_ALIGNOF(NaiveF32 *), sizeof(NaiveF32 *) * num_out_channels);
  if (!self->out_f32)
    return NAIVE_ERR_NOMEM;

  self->ref_f32 = alloc(allocator, NAIVE_MEM_STATE, NAIVE_ALIGNOF(NaiveF32 *), sizeof(NaiveF32 *) * num_out_channels);
  if (!self->ref_f32)
    return NAIVE_ERR_NOMEM;

  self->stats = alloc(allocator, NAIVE_MEM_STATE, NAIVE_ALIGNOF(NaiveTestStats), sizeof(NaiveTestStats) * num_out_channels);

  for (NaiveI32 i = 0; i < num_in_channels; ++i) {
    self->in_f32[i] = alloc(allocator, NAIVE_MEM_STATE, NAIVE_ALIGNOF(NaiveF32), sizeof(NaiveF32) * max_block_size);
    if (!self->in_f32[i])
      return NAIVE_ERR_NOMEM;
  }

  for (NaiveI32 i = 0; i < num_out_channels; ++i) {
    self->out_f32[i] = alloc(allocator, NAIVE_MEM_STATE, NAIVE_ALIGNOF(NaiveF32), sizeof(NaiveF32) * max_block_size);
    if (!self->out_f32[i])
      return NAIVE_ERR_NOMEM;

    self->ref_f32[i] = alloc(allocator, NAIVE_MEM_STATE, NAIVE_ALIGNOF(NaiveF32), sizeof(NaiveF32) * max_block_size);
    if (!self->ref_f32[i])
      return NAIVE_ERR_NOMEM;
  }

  return NAIVE_OK;
}

NaiveResult naive_test_run(NaiveTest *test)
{
  NaiveI32 num_passed = 0;
  NaiveI32 num_failed = 0;
  NaiveResult err = NAIVE_OK;

  for (NaiveI32 i = 0; i < test->test_desc->len && !err; ++i) {
    NAIVE_CONST NaiveTestCaseDesc *case_desc = &test->test_desc->cases[i];
    NAIVE_CONST char *case_in_fname = NULL;
    NAIVE_CONST char *case_out_fname = NULL;
    NaiveBool failed = NAIVE_FALSE;

    printf("test %u [%s] begin\n", i, case_desc->name);

    NaiveI32 block_size = naive_test_case_desc_get_u32(case_desc, NAIVE_TEST_CASE_DESC_KEY_BLOCK_SIZE);
    if (block_size > test->max_block_size) {
      fprintf(stderr, "test %u: block size is too large", i);
      failed = NAIVE_TRUE;
    }

    if (!failed) {
      case_in_fname = naive_test_case_desc_get_str(case_desc, NAIVE_TEST_CASE_DESC_KEY_INPUT);
      case_out_fname = naive_test_case_desc_get_str(case_desc, NAIVE_TEST_CASE_DESC_KEY_OUTPUT);

      if (strlen(test->inputs_dir) + 1 + strlen(case_in_fname) > NAIVE_TEST_FILENAME_MAX) {
        fprintf(stderr, "test %u: input filename is too long", i);
        failed = NAIVE_TRUE;
      }
    }

    if (!failed) {
      strcpy(test->in_fname, test->inputs_dir);
      strcat(test->in_fname, "/");
      strcat(test->in_fname, case_in_fname);

      if (strlen(test->outputs_dir) + 1 + strlen(case_out_fname) > NAIVE_TEST_FILENAME_MAX) {
        fprintf(stderr, "test %u: output filename is too long", i);
        failed = NAIVE_TRUE;
      }
    }

    if (!failed) {
      strcpy(test->out_fname, test->outputs_dir);
      strcat(test->out_fname, "/");
      strcat(test->out_fname, case_out_fname);

      if (strlen(test->refs_dir) + 1 + strlen(case_out_fname) > NAIVE_TEST_FILENAME_MAX) {
        fprintf(stderr, "test %u: output filename is too long", i);
        failed = NAIVE_TRUE;
      }
    }

    FILE *fin = NULL;
    if (!failed) {
      strcpy(test->ref_fname, test->refs_dir);
      strcat(test->ref_fname, "/");
      strcat(test->ref_fname, case_out_fname);

      fin = fopen(test->in_fname, "rb");
      if (fin == NULL) {
        fprintf(stderr, "test %u: could not open %s\n", i, test->in_fname);
        failed = NAIVE_TRUE;
      }
    }

    FILE *fout = NULL;
    if (!failed) {
      fout = fopen(test->out_fname, "wb");
      if (fout == NULL) {
        fprintf(stderr, "test %u: could not open %s\n", i, test->out_fname);
        failed = NAIVE_TRUE;
      }
    }

    FILE *fref = NULL;
    if (!failed) {
      fref = fopen(test->ref_fname, "rb");
    }

    if (!failed) {
      if (test->setup(test->context, case_desc, naive_test_case_desc_get_u32(case_desc, NAIVE_TEST_CASE_DESC_KEY_SAMPLE_RATE)) != NAIVE_OK) {
        fprintf(stderr, "test %u: setup failed\n", i);
        failed = NAIVE_TRUE;
      }
    }

    if (!failed) {
      for (NaiveI32 ich = 0; ich < test->num_in_channels; ich++) {
        memset(test->in_f32[ich], 0, sizeof(NaiveF32) * block_size);
      }

      for (NaiveI32 ich = 0; ich < test->num_out_channels; ich++) {
        memset(test->out_f32[ich], 0, sizeof(NaiveF32) * block_size);
      }

      if (fref != NULL) {
        for (NaiveI32 ich = 0; ich < test->num_out_channels; ich++) {
          memset(test->ref_f32[ich], 0, sizeof(NaiveF32) * block_size);
        }

        for (NaiveI32 ich = 0; ich < test->num_out_channels; ich++) {
          naive_test_stats_init(&test->stats[ich]);
        }
      }
    }

    while (!failed) {
      size_t count = fread(test->in_i16, sizeof(NaiveI16), block_size * test->num_in_channels, fin);

      NaiveI32 real_block_size = count / test->num_in_channels;

      naive_i16_q15_interleaved_to_f32_planar(test->in_f32, test->in_i16, test->num_in_channels, real_block_size);

      int procrc = test->process(test->context, test->in_f32, test->out_f32, real_block_size);
      if (procrc != NAIVE_OK) {
        fprintf(stderr, "test %u: the process function returned %d\n", i, procrc);
        failed = NAIVE_TRUE;
      }

      if (!failed) {
        naive_f32_planar_to_i16_q15_interleaved(test->out_i16, test->out_f32, test->num_out_channels, real_block_size);
        if (fwrite(test->out_i16, sizeof(NaiveI16), count, fout) != count) {
          fprintf(stderr, "test %u: failed to write out_fname file\n", i);
          failed = NAIVE_TRUE;
        }
      }

      if (!failed) {
        if (fref != NULL) {
          size_t count_ref = fread(test->ref_i16, sizeof(NaiveI16), real_block_size * test->num_out_channels, fref);
          if (count_ref != real_block_size * test->num_out_channels) {
            fprintf(stderr, "test %u: ref_fname file is incorrect\n", i);
            failed = NAIVE_TRUE;
          }

          if (!failed) {
            naive_i16_q15_interleaved_to_f32_planar(test->ref_f32, test->ref_i16, test->num_out_channels, real_block_size);
            for (NaiveI32 ich = 0; ich < test->num_out_channels; ich++) {
              naive_test_stats_update(&test->stats[ich], test->out_f32[ich], test->ref_f32[ich], real_block_size);
            }
          }
        }
      }

      if (real_block_size < block_size)
        break;
    }

    if (!failed) {
      if (fref != NULL) {
        for (NaiveI32 ich = 0; ich < test->num_out_channels; ich++) {
          if (NAIVE_DB(test->stats[ich].max_diff) > NAIVE_TEST_FAIL_DB_THRESH) {
            failed = NAIVE_TRUE;
            break;
          }
        }
      }
    }

    if (fref != NULL) {
      for (NaiveI32 ich = 0; ich < test->num_out_channels; ich++) {
        printf("test %u: channel %u: ", i, ich);
        naive_test_stats_print(&test->stats[ich]);
      }
    }

    if (failed) {
      printf("test %u [%s] failed\n", i, case_desc->name);
      num_failed++;
    } else {
      printf("test %u [%s] passed\n", i, case_desc->name);
      num_passed++;
    }

    if (test->teardown) {
      test->teardown(test->context);
    }
    if (fref) fclose(fref);
    if (fout) fclose(fout);
    if (fin) fclose(fin);
  }

  printf("total %u tests, %u passed, %u failed\n", test->test_desc->len, num_passed, num_failed);

  return num_failed;
}

void naive_test_stats_init(NaiveTestStats *stats)
{
  stats->num_samples = 0;
  stats->max_diff = 0;
  stats->max_diff_idx = 0;
}

void naive_test_stats_update(NaiveTestStats *stats, NaiveF32 *out, NaiveF32 *ref_out, NaiveI32 block_size)
{
  for (NaiveI32 i = 0; i < block_size; i++) {
    NaiveF32 diff = NAIVE_ABS(out[i] - ref_out[i]);
    if (diff > stats->max_diff) {
      stats->max_diff = diff;
      stats->max_diff_idx = stats->num_samples + i;
    }
  }

  stats->num_samples += block_size;
}

void naive_test_stats_print(NAIVE_CONST NaiveTestStats *stats)
{
  if (stats->max_diff > 0) {
    printf("total %u samples, max diff: %f dB at sample %u\n", stats->num_samples, NAIVE_DB(stats->max_diff), stats->max_diff_idx);
  } else {
    printf("total %u samples, max diff: 0 db\n", stats->num_samples);
  }
}
