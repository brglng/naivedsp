#ifndef __NAIVE_TEST_H__
#define __NAIVE_TEST_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "toml.h"
#include "naivedsp/assert.h"
#include "naivedsp/math_types.h"
#include "naivedsp/memory.h"

typedef struct {
    NaiveI32    num_samples;
    NaiveF32    max_diff;
    NaiveI32    max_diff_idx;
} NaiveTestStats;

#define NAIVE_TEST_STATS_INIT {0, 0, 0}
void naive_test_stats_init(NaiveTestStats *stats);
void naive_test_stats_update(NaiveTestStats *stats, NaiveF32 *out, NaiveF32 *ref_out, NaiveI32 block_size);
void naive_test_stats_print(NAIVE_CONST NaiveTestStats *stats);


typedef NaiveErr    NaiveTestSetUpFunc(void *context, NAIVE_CONST TomlTable *config, NaiveI32 sample_rate);
typedef void        NaiveTestTearDownFunc(void *context);
typedef NaiveErr    NaiveTestProcessFunc(void *context, NaiveF32 **in, NaiveF32 **out, NaiveI32 block_size);

#define NAIVE_TEST_FILENAME_MAX       511
#define NAIVE_TEST_FAIL_DB_THRESH     -70.0f

typedef struct {
    NAIVE_CONST char            *config_file;
    NAIVE_CONST char            *inputs_dir;
    NAIVE_CONST char            *outputs_dir;
    NAIVE_CONST char            *refs_dir;
    TomlTable                   *config;
    NaiveI32                    num_in_channels_cap;
    NaiveI32                    num_out_channels_cap;
    NaiveI32                    block_size_cap;
    NaiveTestSetUpFunc          *setup;
    NaiveTestTearDownFunc       *teardown;
    NaiveTestProcessFunc        *process;
    void                        *context;

    // temporary buffers
    char                        *in_fname;
    char                        *out_fname;
    char                        *ref_fname;
    NaiveI16                    *in_i16;
    NaiveI16                    *out_i16;
    NaiveI16                    *ref_i16;
    NaiveF32                    **in_f32;
    NaiveF32                    **out_f32;
    NaiveF32                    **ref_f32;
    NaiveTestStats              *stats;
} NaiveTest;

NaiveErr naive_test_init(NaiveTest *self,
                         NaiveAllocFunc alloc,
                         void *alloc_context,
                         NAIVE_CONST char *config_file,
                         NAIVE_CONST char *inputs_dir,
                         NAIVE_CONST char *outputs_dir,
                         NAIVE_CONST char *refs_dir,
                         NaiveI32 num_in_channels_cap,
                         NaiveI32 num_out_channels_cap,
                         NaiveI32 block_size_cap,
                         NaiveTestSetUpFunc *setup,
                         NaiveTestTearDownFunc *teardown,
                         NaiveTestProcessFunc *process,
                         void *context);

NaiveI32 naive_test_run(NaiveTest *test);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: __NAIVE_TEST_H__ */
