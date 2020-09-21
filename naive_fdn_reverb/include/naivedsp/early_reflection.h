#ifndef __NAIVE_EARLY_REFLECTION__
#define __NAIVE_EARLY_REFLECTION__

#ifdef __cplusplus
extern "C" {
#endif

#include "naivedsp/delay_buf.h"
#include "naivedsp/iir.h"

#define NAIVE_EARLY_REFLECTION_SCRATCH_SIZE(block_size_cap) (       \
    NAIVE_CEIL_8_BYTES(sizeof(NaiveF32) * (block_size_cap)) +       \
    NAIVE_CEIL_8_BYTES(sizeof(NaiveF32) * (block_size_cap)) * 2     \
)

typedef enum {
    NAIVE_EARLY_REFLECTION_TYPE_MOORER,
    NAIVE_EARLY_REFLECTION_TYPE_MOORER_HALL,
    NAIVE_EARLY_REFLECTION_TYPE_RANDOM,
    NAIVE_EARLY_REFLECTION_TYPE_ROOM_SIM,
    NAIVE_EARLY_REFLECTION_TYPE_CUSTOM_DELAY_TAPS,
    NAIVE_EARLY_REFLECTION_TYPE_CUSTOM_IR,
} NaiveEarlyReflectionType;

typedef struct {
    NaiveI32                    num_taps_cap;
    NaiveI32                    delay_len_cap;
    NaiveEarlyReflectionType    type;
    NaiveI32                    left_taps;
    NaiveI32                    right_taps;
    NaiveI32*                   left_delay_lens;
    NaiveF32*                   left_delay_gains;
    NaiveI32*                   right_delay_lens;
    NaiveF32*                   right_delay_gains;
    NaiveDelayBuf*              left_delay_bufs;
    NaiveDelayBuf*              right_delay_bufs;
    NaiveI32                    left_send_len;
    NaiveI32                    right_send_len;
    NaiveI32                    left_send_delay_len;
    NaiveI32                    right_send_delay_len;
    NaiveDelayBuf               left_send_delay_buf;
    NaiveDelayBuf               right_send_delay_buf;
    NaiveCascadedIir1stDf1      *left_delay_filters;
    NaiveCascadedIir1stDf1      *right_delay_filters;
} NaiveEarlyReflection;

NaiveErr naive_early_reflection_init(NaiveEarlyReflection *self,
                                     void *alloc_context,
                                     NaiveAllocFunc *alloc,
                                     NaiveI32 num_taps_cap,
                                     NaiveI32 delay_len_cap);

void naive_early_reflection_reset(NaiveEarlyReflection *self);

NaiveErr naive_early_reflection_process(NaiveEarlyReflection *self,
                                        NaiveF32 *left_in,
                                        NaiveF32 *right_in,
                                        NaiveF32 *left_out,
                                        NaiveF32 *right_out,
                                        NaiveF32 *left_send_out,
                                        NaiveF32 *right_send_out,
                                        NaiveI32 block_size,
                                        void *scratch);

NaiveErr naive_early_reflection_set_preset(NaiveEarlyReflection *self,
                                           NaiveEarlyReflectionType type,
                                           NaiveI32 sample_rate,
                                           NaiveF32 low_mid_xover_freq,
                                           NaiveF32 high_damp_freq,
                                           NaiveF32 dc_reverb_time,
                                           NaiveF32 mid_freq_reverb_time);

NaiveErr naive_early_reflection_set_left_send_time(NaiveEarlyReflection *self, NaiveF32 left_send_time, NaiveI32 sample_rate);
NaiveErr naive_early_reflection_set_right_send_time(NaiveEarlyReflection *self, NaiveF32 right_send_time, NaiveI32 sample_rate);
NaiveErr naive_early_reflection_set_left_send_delay_time(NaiveEarlyReflection *self, NaiveF32 left_send_delay_time, NaiveI32 sample_rate);
NaiveErr naive_early_reflection_set_right_send_delay_time(NaiveEarlyReflection *self, NaiveF32 left_send_delay_time, NaiveI32 sample_rate);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: __NAIVE_EARLY_REFLECTION__ */
