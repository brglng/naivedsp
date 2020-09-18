#ifndef __NAIVE_FDN_REVERB_H__
#define __NAIVE_FDN_REVERB_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "naivedsp/delay_buf.h"
#include "naivedsp/early_reflection.h"
#include "naivedsp/iir.h"
#include "naivedsp/math.h"
#include "naivedsp/math_types.h"
#include "naivedsp/memory.h"

/** Scratch buffer size used by NaiveFdnReverb
 *
 * @param   num_delays_cap  Maximum allowed number of delays
 * @param   block_size_cap  Maximum allowed block size
 *
 * @return the size of the scratch buffer
 */
#define NAIVE_FDN_REVERB_SCRATCH_SIZE(num_delays_cap, block_size_cap) (                 \
    NAIVE_CEIL_8_BYTES(sizeof(NaiveF32) * (block_size_cap)) * 2 +                       \
    NAIVE_MAX(NAIVE_CEIL_8_BYTES(sizeof(NaiveF32) * (block_size_cap)) * 2,              \
              NAIVE_EARLY_REFLECTION_SCRATCH_SIZE(block_size_cap)) +                    \
    NAIVE_CEIL_8_BYTES(sizeof(NaiveF32) * (block_size_cap)) * 2 * 2 +                   \
    NAIVE_CEIL_8_BYTES(sizeof(NaiveF32) * (block_size_cap)) * (num_delays_cap) * 3 +    \
    NAIVE_CEIL_8_BYTES(sizeof(NaiveF32) * (block_size_cap)) * 2)

typedef struct {
    NaiveI32                    sample_rate;

    // LR Crosstalk
    NaiveF32                    left_to_left_gain;
    NaiveF32                    left_to_right_gain;
    NaiveF32                    right_to_left_gain;
    NaiveF32                    right_to_right_gain;

    // Pre Delay
    NaiveI32                    left_pre_delay_len;
    NaiveI32                    right_pre_delay_len;
    NaiveDelayBuf               left_pre_delay_buf;     // Left pre-delay buffer
    NaiveDelayBuf               right_pre_delay_buf;    // Right pre-delay buffer

    // Early Reflection
    NaiveEarlyReflection        early_reflection;
    NaiveF32                    left_early_send_gain;
    NaiveF32                    right_early_send_gain;

    // FDN
    NaiveI32                    num_delays_cap;         // Max allowed number of delay lines
    NaiveI32                    num_delays;             // Number of delay lines:               N
    NaiveI32*                   delay_lens;             // Delay lengths of each delay line:    m[i]
    NaiveDelayBuf*              delay_bufs;             // Delay buffers of each delay line:    s[i]
    NaiveF32*                   in_gains;               // Input gains of each delay line:      b[i]
    NaiveF32*                   out_gains;              // Output gains of each delay line:     c[i]
    NaiveCascadedIir1stDf1*     delay_filters;          // Post delay line filters
    NaiveF32*                   feedback_mat;           // Feedback matrix:                     A[i, j]

    // Final Mixing
    NaiveF32                    left_dry_gain;
    NaiveF32                    left_early_gain;
    NaiveF32                    left_late_gain;
    NaiveF32                    right_dry_gain;
    NaiveF32                    right_early_gain;
    NaiveF32                    right_late_gain;

    // Parameters recoreded for comparison when being set
    NaiveF32                    min_acoustic_path;
    NaiveF32                    max_acoustic_path;
} NaiveFdnReverb;

/** Initialize the NaiveReverb instance
 *
 * @param   self                    NaiveReverb instance to initialize
 * @param   alloc_context           Allocator context
 * @param   alloc                   Allocator callback function
 * @param   sample_rate             Sampling rate
 * @param   pre_delay_len_cap       Maximum allowed delay length for pre-delay
 * @param   early_num_taps_cap      Maximum allowed number of early reflection delay taps
 * @param   early_delay_len_cap     Maximum allowed early reflection delay length
 * @param   num_delays_cap          Maximum allowed number of delays to use
 * @param   delay_len_cap           Maximum allowed delay length
 *
 * @return NAIVE_OK if successful, else an error code.
 */
NaiveErr naive_fdn_reverb_init(
    NaiveFdnReverb *self,
    void *alloc_context,
    NaiveAllocFunc alloc,
    NaiveI32 sample_rate,
    NaiveI32 pre_delay_len_cap,
    NaiveI32 early_num_taps_cap,
    NaiveI32 early_delay_len_cap,
    NaiveI32 num_delays_cap,
    NaiveI32 delay_len_cap);

NaiveErr naive_fdn_reverb_process(
    NaiveFdnReverb *states,
    NaiveF32 *left_in,
    NaiveF32 *right_in,
    NaiveF32 *left_out,
    NaiveF32 *right_out,
    NaiveI32 block_size,
    void *scratch);

void naive_fdn_reverb_reset(NaiveFdnReverb *self);

NaiveErr naive_fdn_reverb_set_balance_crosstalk(NaiveFdnReverb *self,
                                                NaiveF32 balance,
                                                NaiveF32 crosstalk_gain);

NaiveErr naive_fdn_reverb_set_left_pre_delay_time(NaiveFdnReverb *self, NaiveF32 left_pre_delay_time);
NaiveErr naive_fdn_reverb_set_right_pre_delay_time(NaiveFdnReverb *self, NaiveF32 right_pre_delay_time);

NaiveErr naive_fdn_reverb_set_early_reflection_preset(NaiveFdnReverb *self, NaiveEarlyReflectionType type);
NaiveErr naive_fdn_reverb_set_left_early_reflection_send_gain(NaiveFdnReverb *self, NaiveF32 gain);
NaiveErr naive_fdn_reverb_set_right_early_reflection_send_gain(NaiveFdnReverb *self, NaiveF32 gain);

NaiveErr naive_fdn_reverb_set_diffuse_params(NaiveFdnReverb *self,
                                             NaiveI32 num_delays,
                                             NaiveF32 min_acoustic_path,
                                             NaiveF32 max_acoustic_path,
                                             NaiveF32 low_mid_xover_freq,
                                             NaiveF32 high_damp_freq,
                                             NaiveF32 dc_reverb_time,
                                             NaiveF32 mid_freq_reverb_time);
NaiveErr naive_fdn_reverb_set_input_gain(NaiveFdnReverb *self, NaiveF32 input_gain);
NaiveErr naive_fdn_reverb_set_output_gain(NaiveFdnReverb *self, NaiveF32 output_gain);


NaiveErr naive_fdn_reverb_set_left_early_send_time(NaiveFdnReverb *self, NaiveF32 send_time);
NaiveErr naive_fdn_reverb_set_right_early_send_time(NaiveFdnReverb *self, NaiveF32 send_time);

NAIVE_INLINE NaiveErr naive_fdn_reverb_set_left_early_lpf(NaiveFdnReverb *self, NaiveF32 freq)
{
    return naive_early_reflection_set_left_lpf(&self->early_reflection, self->sample_rate, freq);
}

NAIVE_INLINE NaiveErr naive_fdn_reverb_set_right_early_lpf(NaiveFdnReverb *self, NaiveF32 freq)
{
    return naive_early_reflection_set_right_lpf(&self->early_reflection, self->sample_rate, freq);
}

NAIVE_INLINE NaiveErr naive_fdn_reverb_set_left_early_hpf(NaiveFdnReverb *self, NaiveF32 freq)
{
    return naive_early_reflection_set_left_hpf(&self->early_reflection, self->sample_rate, freq);
}

NAIVE_INLINE NaiveErr naive_fdn_reverb_set_right_early_hpf(NaiveFdnReverb *self, NaiveF32 freq)
{
    return naive_early_reflection_set_right_hpf(&self->early_reflection, self->sample_rate, freq);
}

NAIVE_INLINE NaiveErr naive_fdn_reverb_set_left_early_apf(NaiveFdnReverb *self, NaiveF32 freq, NaiveF32 q)
{
    return naive_early_reflection_set_left_apf(&self->early_reflection, self->sample_rate, freq, q);
}

NAIVE_INLINE NaiveErr naive_fdn_reverb_set_right_early_apf(NaiveFdnReverb *self, NaiveF32 freq, NaiveF32 q)
{
    return naive_early_reflection_set_right_apf(&self->early_reflection, self->sample_rate, freq, q);
}

NaiveErr naive_fdn_reverb_set_left_early_send_gain(NaiveFdnReverb *self, NaiveF32 gain);
NaiveErr naive_fdn_reverb_set_right_early_send_gain(NaiveFdnReverb *self, NaiveF32 gain);

NaiveErr naive_fdn_reverb_set_left_dry_gain(NaiveFdnReverb *self, NaiveF32 dry_gain);
NaiveErr naive_fdn_reverb_set_left_early_gain(NaiveFdnReverb *self, NaiveF32 early_gain);
NaiveErr naive_fdn_reverb_set_left_late_gain(NaiveFdnReverb *self, NaiveF32 late_gain);
NaiveErr naive_fdn_reverb_set_right_dry_gain(NaiveFdnReverb *self, NaiveF32 dry_gain);
NaiveErr naive_fdn_reverb_set_right_early_gain(NaiveFdnReverb *self, NaiveF32 early_gain);
NaiveErr naive_fdn_reverb_set_right_late_gain(NaiveFdnReverb *self, NaiveF32 late_gain);
void naive_fdn_reverb_set_default_params(NaiveFdnReverb *self);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: __NAIVE_FDN_REVERB_H__ */
