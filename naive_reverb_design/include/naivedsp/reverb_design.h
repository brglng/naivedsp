#ifndef __NAIVE_REVERB_DESIGN_H__
#define __NAIVE_REVERB_DESIGN_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "naivedsp/reverb_params.h"
#include "naivedsp/result.h"

NaiveResult naive_reverb_params_design(NaiveReverbParams *params,
                                   NaiveU32 sample_rate,                      // 采样率
                                   NaiveF32 pre_delay_time,                   // 预延时时间（秒）
                                   NaiveU32 num_delays,                       // 使用的延时单元个数
                                   NaiveF32 min_acoustic_ray_len,             // 最短声音射线长度（米）
                                   NaiveF32 max_acoustic_ray_len,             // 最长声音射线长度（米）
                                   NaiveF32 low_mid_xover_freq,               // 中低频段分界频率
                                   NaiveF32 high_damp_freq,                   // 中高频段分界频率（此处开始衰减）
                                   NaiveF32 dc_reverb_time,                   // 直流混响时间
                                   NaiveF32 mid_freq_reverb_time,             // 中频混响时间
                                   NaiveF32 in_gain,                          // 输入增益
                                   NaiveF32 out_gain,                         // 输出增益
                                   NaiveF32 dry_gain                          // 干声增益
                                   );

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: __NAIVE_REVERB_DESIGN_H__ */
