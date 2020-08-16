#ifndef __NAIVE_REVERB_PARAMS_H__
#define __NAIVE_REVERB_PARAMS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "naivedsp/inttypes.h"
#include "naivedsp/floattypes.h"
#include "naivedsp/peq_coeffs.h"

typedef struct {
    // 湿声预延时
    NaiveU32          pre_delay_len;      // 预延时长度

    // FDN 部分
    NaiveU32          num_delays_cap;     // 最大允许的延时个数
    NaiveU32          num_delays;         // 延时的个数 N
    NaiveU32          *delay_lens;        // 各延时的延时长度 m[i]
    NaiveF32          *in_gains;          // 各延时的输入增益 b[i]
    NaiveF32          *out_gains;         // 各延时的输出增益 c[i]
    NaivePeqCoeffs    *delay_eq_coeffs;   // 各延时后加的 EQ 的系数
    NaiveF32          *feedback_mat;      // 反馈增益矩阵 A[i, j]

    // 湿声总输出处理
    NaivePeqCoeffs    wet_eq_coeffs;      // 湿声总输出 EQ 系数

    // 干声处理
    NaiveF32          dry_gain;           // 干声增益
} NaiveReverbParams;

/** Initialize the NaiveReverbParams instance
 *
 * @param   param                   NaiveReverbParams instance to initialize
 * @param   alloc                   Allocator callback function
 * @param   allocator               Allocator instance
 * @param   max_num_delays          Maximum allowed number of delays to use
 * @param   wet_eq_max_bands        Maximum allowed number of bands for the wet master EQ
 * @param   wet_eq_band_max_sos     Maximum allowed number of cascaded 2nd order section for the wet master EQ
 *
 * @return NAIVE_OK if successful, else an error code.
 */
NaiveResult naive_reverb_params_init(
    NaiveReverbParams *params,
    NaiveAllocFunc alloc,
    void *allocator,
    NaiveU32 max_num_delays,
    NaiveU32 wet_eq_max_bands,
    NaiveU32 wet_eq_band_max_sos);

void naive_reverb_params_reset(NaiveReverbParams *params);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: __NAIVE_REVERB_PARAMS_H__ */
