#ifndef __NAIVE_FDN_REVERB_H__
#define __NAIVE_FDN_REVERB_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "naivedsp/iir.h"
#include "naivedsp/delay_buf.h"
#include "naivedsp/math.h"
#include "naivedsp/math_types.h"
#include "naivedsp/memory.h"

typedef struct {
    NaiveI32                    sample_rate;
    NaiveI32                    block_size_cap;
    NaiveI32                    num_delays_cap;         // Max allowed number of delay lines

    // Pre Delay
    NaiveI32                    pre_delay_len;
    NaiveDelayBuf               left_pre_delay_buf;     // Left pre-delay buffer
    NaiveDelayBuf               right_pre_delay_buf;    // Right pre-delay buffer

    // FDN
    NaiveI32                    num_delays;             // Number of delay lines:               N
    NaiveI32*                   delay_lens;             // Delay lengths of each delay line:    m[i]
    NaiveDelayBuf*              delay_bufs;             // Delay buffers of each delay line:    s[i]
    NaiveF32*                   in_gains;               // Input gains of each delay line:      b[i]
    NaiveF32*                   out_gains;              // Output gains of each delay line:     c[i]
    NaiveCascadedIir1stDf1*     delay_filters;          // Post delay line filters
    NaiveF32*                   feedback_mat;           // Feedback matrix:                     A[i, j]

    NaiveF32                    dry_gain;
    NaiveF32                    wet_gain;

    NaiveF32                    min_acoustic_path;
    NaiveF32                    max_acoustic_path;
    NaiveF32                    low_mid_xover_freq;
    NaiveF32                    high_damp_freq;
    NaiveF32                    dc_reverb_time;
    NaiveF32                    mid_freq_reverb_time;

    NaiveF32*                   scratch;
} NaiveFdnReverb;

/** Initialize the NaiveReverb instance
 *
 * @param   self                    NaiveReverb instance to initialize
 * @param   alloc                   Allocator callback function
 * @param   allocator               Allocator instance
 * @param   pre_delay_len_cap       Maximum allowed delay length for pre-delay
 * @param   num_delays_cap          Maximum allowed number of delays to use
 * @param   delay_len_cap           Maximum allowed delay length
 * @param   wet_eq_max_bands        Maximum allowed number of bands for the wet master EQ
 * @param   wet_eq_band_max_sos     Maximum allowed number of cascaded 2nd order section for the wet master EQ
 *
 * @return NAIVE_OK if successful, else an error code.
 */
NaiveErr naive_fdn_reverb_init(
    NaiveFdnReverb *self,
    void *alloc_context,
    NaiveAllocFunc alloc,
    NaiveI32 sample_rate,
    NaiveI32 block_size_cap,
    NaiveI32 pre_delay_len_cap,
    NaiveI32 num_delays_cap,
    NaiveI32 delay_len_cap);

NaiveErr naive_fdn_reverb_process(
    NaiveFdnReverb *states,
    NaiveF32 *left_in,
    NaiveF32 *right_in,
    NaiveF32 *left_out,
    NaiveF32 *right_out,
    NaiveI32 block_size);

void naive_fdn_reverb_reset(NaiveFdnReverb *self);

NaiveErr naive_fdn_reverb_set_pre_delay_time(NaiveFdnReverb *self, NaiveF32 pre_delay_time);
NaiveErr naive_fdn_reverb_set_room_params(NaiveFdnReverb *self,
                                          NaiveI32 num_delays,
                                          NaiveF32 min_acoustic_path,
                                          NaiveF32 max_acoustic_path,
                                          NaiveF32 low_mid_xover_freq,
                                          NaiveF32 high_damp_freq,
                                          NaiveF32 dc_reverb_time,
                                          NaiveF32 mid_freq_reverb_time);
NaiveErr naive_fdn_reverb_set_input_gain(NaiveFdnReverb *self, NaiveF32 input_gain);
NaiveErr naive_fdn_reverb_set_output_gain(NaiveFdnReverb *self, NaiveF32 output_gain);
NaiveErr naive_fdn_reverb_set_dry_gain(NaiveFdnReverb *self, NaiveF32 dry_gain);
NaiveErr naive_fdn_reverb_set_wet_gain(NaiveFdnReverb *self, NaiveF32 wet_gain);
void naive_fdn_reverb_set_default_params(NaiveFdnReverb *self);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: __NAIVE_FDN_REVERB_H__ */
