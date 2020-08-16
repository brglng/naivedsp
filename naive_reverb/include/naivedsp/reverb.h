#ifndef __NAIVE_REVERB_H__
#define __NAIVE_REVERB_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "naivedsp/math.h"
#include "naivedsp/memory.h"
#include "naivedsp/result.h"
#include "naivedsp/peq.h"
#include "naivedsp/reverb_states.h"
#include "naivedsp/reverb_params.h"

/** Scratch buffer size used by naive_reverb_process
 *
 * @param   num_delays      Number of delays
 * @param   min_delay_len   The minimum value of the delay lengths
 * @param   max_delay_len   The maximum value of the delay lengths
 * @param   block_size      Block size
 *
 * @return the size of the scratch buffer
 */
#define NAIVE_REVERB_SCRATCH_SIZE(num_delays, min_delay_len, max_delay_len, block_size) (     \
    2 * NAIVE_CEIL_8_BYTES(sizeof(NaiveF32) * block_size) +                                     \
    2 * NAIVE_CEIL_8_BYTES(sizeof(NaiveF32) * (max_delay_len)) +                                \
    NAIVE_CEIL_8_BYTES(sizeof(NaiveF32) * NAIVE_MIN(min_delay_len, block_size)) * (num_delays) +  \
    NAIVE_CEIL_8_BYTES(NAIVE_PEQ_SCRATCH_SIZE(NAIVE_MIN(min_delay_len, block_size))) +            \
    NAIVE_CEIL_8_BYTES(sizeof(NaiveF32) * (block_size)) * (num_delays) +                        \
    NAIVE_CEIL_8_BYTES(NAIVE_PEQ_SCRATCH_SIZE(sizeof(NaiveF32) * (block_size))))

/** Initialize the NaiveReverbStates instance
 *
 * @param   states                  NaiveReverbStates instance to initialize
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
NaiveResult naive_reverb_states_init(
    NaiveReverbStates *states,
    NaiveAllocFunc alloc,
    void *allocator,
    NaiveU32 pre_delay_len_cap,
    NaiveU32 num_delays_cap,
    NaiveU32 delay_len_cap,
    NaiveU32 wet_eq_max_bands,
    NaiveU32 wet_eq_band_max_sos);

void naive_reverb_process(
    NaiveReverbStates *states,
    NAIVE_CONST NaiveReverbParams *params,
    NaiveF32 *left,
    NaiveF32 *right,
    NaiveU32 block_size,
    void *scratch);

void naive_reverb_reset(NaiveReverbStates *states);

NaiveResult naive_reverb_set_params(
    NaiveReverbStates *states,
    NaiveReverbParams *params,
    NAIVE_CONST NaiveReverbParams *from);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: __NAIVE_REVERB_H__ */
