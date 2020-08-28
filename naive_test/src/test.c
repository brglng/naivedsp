#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "naivedsp/math.h"
#include "naivedsp/conversion.h"
#include "naivedsp/memory.h"
#include "naivedsp/test.h"

NaiveErr naive_test_init(NaiveTest *self,
                         NaiveAllocFunc alloc,
                         void *alloc_context,
                         NAIVE_CONST char *config_file,
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
    self->config_file = config_file;
    self->inputs_dir = inputs_dir;
    self->outputs_dir = outputs_dir;
    self->refs_dir = refs_dir;
    self->config = NULL;
    self->num_in_channels = num_in_channels;
    self->num_out_channels = num_out_channels;
    self->max_block_size = max_block_size;
    self->setup = setup;
    self->teardown = teardown;
    self->process = process;
    self->context = context;

    self->config = toml_load_filename(config_file);
    if (self->config == NULL) {
        fprintf(stderr, "Failed to read config file %s: %s [libtoml error %d]\n", config_file, toml_err()->message, toml_err()->code);
        return NAIVE_ERR;
    }

    self->in_fname = alloc(alloc_context, NAIVE_MEM_STATE, NAIVE_ALIGNOF(char), sizeof(char) * (NAIVE_TEST_FILENAME_MAX + 1));
    if (!self->in_fname)
        return NAIVE_ERR_NOMEM;

    self->out_fname = alloc(alloc_context, NAIVE_MEM_STATE, NAIVE_ALIGNOF(char), sizeof(char) * (NAIVE_TEST_FILENAME_MAX + 1));
    if (!self->out_fname)
        return NAIVE_ERR_NOMEM;

    self->ref_fname = alloc(alloc_context, NAIVE_MEM_STATE, NAIVE_ALIGNOF(char), sizeof(char) * (NAIVE_TEST_FILENAME_MAX + 1));
    if (!self->ref_fname)
        return NAIVE_ERR_NOMEM;

    self->in_i16 = alloc(alloc_context, NAIVE_MEM_STATE, NAIVE_ALIGNOF(NaiveI16), sizeof(NaiveI16) * (NaiveUSize)num_in_channels * (NaiveUSize)max_block_size);
    if (!self->in_i16)
        return NAIVE_ERR_NOMEM;

    self->out_i16 = alloc(alloc_context, NAIVE_MEM_STATE, NAIVE_ALIGNOF(NaiveI16), sizeof(NaiveI16) * (NaiveUSize)num_out_channels * (NaiveUSize)max_block_size);
    if (!self->out_i16)
        return NAIVE_ERR_NOMEM;

    self->ref_i16 = alloc(alloc_context, NAIVE_MEM_STATE, NAIVE_ALIGNOF(NaiveI16), sizeof(NaiveI16) * (NaiveUSize)num_out_channels * (NaiveUSize)max_block_size);
    if (!self->ref_i16)
        return NAIVE_ERR_NOMEM;

    self->in_f32 = alloc(alloc_context, NAIVE_MEM_STATE, NAIVE_ALIGNOF(NaiveF32 *), sizeof(NaiveF32*) * (NaiveUSize)num_in_channels);
    if (!self->in_f32)
        return NAIVE_ERR_NOMEM;

    self->out_f32 = alloc(alloc_context, NAIVE_MEM_STATE, NAIVE_ALIGNOF(NaiveF32 *), sizeof(NaiveF32*) * (NaiveUSize)num_out_channels);
    if (!self->out_f32)
        return NAIVE_ERR_NOMEM;

    self->ref_f32 = alloc(alloc_context, NAIVE_MEM_STATE, NAIVE_ALIGNOF(NaiveF32 *), sizeof(NaiveF32*) * (NaiveUSize)num_out_channels);
    if (!self->ref_f32)
        return NAIVE_ERR_NOMEM;

    self->stats = alloc(alloc_context, NAIVE_MEM_STATE, NAIVE_ALIGNOF(NaiveTestStats), sizeof(NaiveTestStats) * (NaiveUSize)num_out_channels);

    for (NaiveI32 i = 0; i < num_in_channels; ++i) {
        self->in_f32[i] = alloc(alloc_context, NAIVE_MEM_STATE, NAIVE_ALIGNOF(NaiveF32), sizeof(NaiveF32) * (NaiveUSize)max_block_size);
        if (!self->in_f32[i])
            return NAIVE_ERR_NOMEM;
    }

    for (NaiveI32 i = 0; i < num_out_channels; ++i) {
        self->out_f32[i] = alloc(alloc_context, NAIVE_MEM_STATE, NAIVE_ALIGNOF(NaiveF32), sizeof(NaiveF32) * (NaiveUSize)max_block_size);
        if (!self->out_f32[i])
            return NAIVE_ERR_NOMEM;

        self->ref_f32[i] = alloc(alloc_context, NAIVE_MEM_STATE, NAIVE_ALIGNOF(NaiveF32), sizeof(NaiveF32) * (NaiveUSize)max_block_size);
        if (!self->ref_f32[i])
            return NAIVE_ERR_NOMEM;
    }

    return NAIVE_OK;
}

NaiveI32 naive_test_run(NaiveTest *test)
{
    NaiveI32 num_passed = 0;
    NaiveI32 num_failed = 0;

    NaiveI32 i = 0;
    for (TomlTableIter it = toml_table_iter_new(test->config);
         toml_table_iter_has_next(&it);
         toml_table_iter_next(&it), ++i)
    {
        NAIVE_CONST char *case_name = NULL;
        NAIVE_CONST TomlTable *case_config = NULL;
        NAIVE_CONST char *case_in_fname = NULL;
        NAIVE_CONST char *case_out_fname = NULL;
        NaiveBool failed = NAIVE_FALSE;
        NaiveI32 block_size = 0;

        if (toml_table_iter_get(&it)->value->type != TOML_TABLE) {
            fprintf(stderr, "test %d: invalid test config", i);
            failed = NAIVE_TRUE;
        }

        if (!failed) {
            case_name = toml_table_iter_get(&it)->key->str;
            case_config = toml_table_iter_get(&it)->value->value.table;

            printf("test %u [%s] begin\n", i, case_name);

            block_size = (NaiveI32)toml_table_get_as_integer(case_config, "block-size");
            if (block_size > test->max_block_size) {
                fprintf(stderr, "test %d: block size is too large\n", i);
                failed = NAIVE_TRUE;
            }
        }

        if (!failed) {
            case_in_fname = toml_table_get_as_string(case_config, "input")->str;
            case_out_fname = toml_table_get_as_string(case_config, "output")->str;

            if (strlen(test->inputs_dir) + 1 + strlen(case_in_fname) > NAIVE_TEST_FILENAME_MAX) {
                fprintf(stderr, "test %d: input filename is too long\n", i);
                failed = NAIVE_TRUE;
            }
        }

        if (!failed) {
            strcpy(test->in_fname, test->inputs_dir);
            strcat(test->in_fname, "/");
            strcat(test->in_fname, case_in_fname);

            if (strlen(test->outputs_dir) + 1 + strlen(case_out_fname) > NAIVE_TEST_FILENAME_MAX) {
                fprintf(stderr, "test %d: output filename is too long\n", i);
                failed = NAIVE_TRUE;
            }
        }

        if (!failed) {
            strcpy(test->out_fname, test->outputs_dir);
            strcat(test->out_fname, "/");
            strcat(test->out_fname, case_out_fname);

            if (strlen(test->refs_dir) + 1 + strlen(case_out_fname) > NAIVE_TEST_FILENAME_MAX) {
                fprintf(stderr, "test %d: output filename is too long\n", i);
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
                fprintf(stderr, "test %d: could not open %s: %s [OS error %d]\n", i, test->in_fname, strerror(errno), errno);
                failed = NAIVE_TRUE;
            }
        }

        FILE *fout = NULL;
        if (!failed) {
            fout = fopen(test->out_fname, "wb");
            if (fout == NULL) {
                fprintf(stderr, "test %d: could not open %s: %s [OS error %d]\n", i, test->out_fname, strerror(errno), errno);
                failed = NAIVE_TRUE;
            }
        }

        FILE *fref = NULL;
        if (!failed) {
            fref = fopen(test->ref_fname, "rb");
        }

        if (!failed) {
            NaiveErr setup_err = test->setup(test->context, case_config, (NaiveI32)toml_table_get_as_integer(case_config, "sample-rate"));
            if (setup_err != NAIVE_OK) {
                fprintf(stderr, "test %d: setup failed with error %d\n", i, setup_err);
                failed = NAIVE_TRUE;
            }
        }

        if (!failed) {
            for (NaiveI32 ich = 0; ich < test->num_in_channels; ich++) {
                memset(test->in_f32[ich], 0, sizeof(NaiveF32) * (NaiveUSize)block_size);
            }

            for (NaiveI32 ich = 0; ich < test->num_out_channels; ich++) {
                memset(test->out_f32[ich], 0, sizeof(NaiveF32) * (NaiveUSize)block_size);
            }

            if (fref != NULL) {
                for (NaiveI32 ich = 0; ich < test->num_out_channels; ich++) {
                    memset(test->ref_f32[ich], 0, sizeof(NaiveF32) * (NaiveUSize)block_size);
                }

                for (NaiveI32 ich = 0; ich < test->num_out_channels; ich++) {
                    naive_test_stats_init(&test->stats[ich]);
                }
            }
        }

        while (!failed) {
            size_t count = fread(test->in_i16, sizeof(NaiveI16), (NaiveUSize)(block_size * test->num_in_channels), fin);

            NaiveI32 real_block_size = (NaiveI32)count / test->num_in_channels;

            naive_i16_q15_interleaved_to_f32_planar(test->in_f32, test->in_i16, test->num_in_channels, real_block_size);

            NaiveErr proc_err = test->process(test->context, test->in_f32, test->out_f32, real_block_size);
            if (proc_err != NAIVE_OK) {
                fprintf(stderr, "test %d: the process function returned %d\n", i, proc_err);
                failed = NAIVE_TRUE;
            }

            if (!failed) {
                naive_f32_planar_to_i16_q15_interleaved(test->out_i16, test->out_f32, test->num_out_channels, real_block_size);
                if (fwrite(test->out_i16, sizeof(NaiveI16), count, fout) != count) {
                    fprintf(stderr, "test %d: failed to write out_fname file\n", i);
                    failed = NAIVE_TRUE;
                }
            }

            if (!failed) {
                if (fref != NULL) {
                    size_t count_ref = fread(test->ref_i16, sizeof(NaiveI16), (NaiveUSize)(real_block_size * test->num_out_channels), fref);
                    if ((NaiveI32)count_ref != real_block_size * test->num_out_channels) {
                        fprintf(stderr, "test %d: ref_fname file is incorrect\n", i);
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
                printf("test %d: channel %d: ", i, ich);
                naive_test_stats_print(&test->stats[ich]);
            }
        }

        if (failed) {
            printf("test %d [%s] failed\n", i, case_name);
            num_failed++;
        } else {
            printf("test %d [%s] passed\n", i, case_name);
            num_passed++;
        }

        if (test->teardown) {
            test->teardown(test->context);
        }
        if (fref) fclose(fref);
        if (fout) fclose(fout);
        if (fin) fclose(fin);
    }

    printf("total %zu tests, %d passed, %d failed\n", test->config->len, num_passed, num_failed);

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
        printf("total %d samples, max diff: %f dB at sample %u\n", stats->num_samples, NAIVE_DB(stats->max_diff), stats->max_diff_idx);
    } else {
        printf("total %d samples, max diff: 0 db\n", stats->num_samples);
    }
}
