#ifndef __NAIVE_DELAY_H__
#define __NAIVE_DELAY_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "naivedsp/math.h"
#include "naivedsp/memory.h"
#include "naivedsp/typedefs.h"
#include "naivedsp/delay_params.h"
#include "naivedsp/delay_states.h"

#define NAIVE_DELAY_SCRATCH_SIZE(block_size, delay_len) \
  (3 * NAIVE_CEIL_8_BYTES(sizeof(NaiveF32) * NAIVE_MIN(block_size, delay_len)))

NaiveResult naive_delay_states_init(NaiveDelayStates *states, NaiveAllocFunc *alloc, void *allocator, NaiveI32 delay_len_cap);

void naive_delay_reset(NaiveDelayStates *states);

void naive_delay_process(NaiveDelayStates *states, NAIVE_CONST NaiveDelayParams *params, NaiveF32 *inout, NaiveI32 block_size, void *scratch);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: __NAIVE_DELAY_H__ */
