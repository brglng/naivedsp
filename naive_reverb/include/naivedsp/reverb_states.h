#ifndef __NAIVE_REVERB_DEF_H__
#define __NAIVE_REVERB_DEF_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "naivedsp/fifo_def.h"
#include "naivedsp/peq_states.h"

typedef struct {
    // 湿声预延时
    NaiveFifo         left_pre_delay_buf;     // 左声道预延时缓冲区
    NaiveFifo         right_pre_delay_buf;    // 右声道预延时缓冲区

    // FDN 部分
    NaiveU32          num_delays_cap;         // 最大允许的延时个数
    NaiveFifo         left_in_delay_buf;      // 左声道输入 xl[i] 延时缓冲区
    NaiveFifo         right_in_delay_buf;     // 右声道输入 xr[i] 延时缓冲区
    NaiveFifo         *delay_bufs;            // 各延时的延时缓冲区 s[i]
    NaiveU32          max_delay_len;          // 所有延时长度中的最大值
    NaiveU32          min_delay_len;          // 所有延时长度中的最小值
    NaivePeqStates    *delay_eq_states;       // 每个延时后加的 EQ 状态

    // 湿声总输出处理
    NaivePeqStates    left_wet_eq_states;     // 湿声总输出 EQ 状态
    NaivePeqStates    right_wet_eq_states;    // 湿声总输出 EQ 状态
} NaiveReverbStates;

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: __NAIVE_REVERB_DEF_H__ */
