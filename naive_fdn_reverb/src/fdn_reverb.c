#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "naivedsp/math.h"
#include "naivedsp/memory.h"
#include "naivedsp/fdn_reverb.h"

#define SOUND_SPEED 343
#define DLMIN 0
#define DLMAX 8192

static NAIVE_CONST NaiveI32 PRIMES[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
    73, 79, 83, 89, 97, 101, 103, 107, 109, 113,
    127, 131, 137, 139, 149, 151, 157, 163, 167, 173,
    179, 181, 191, 193, 197, 199, 211, 223, 227, 229,
    233, 239, 241, 251, 257, 263, 269, 271, 277, 281,
    283, 293, 307, 311, 313, 317, 331, 337, 347, 349,
    353, 359, 367, 373, 379, 383, 389, 397, 401, 409,
    419, 421, 431, 433, 439, 443, 449, 457, 461, 463,
    467, 479, 487, 491, 499, 503, 509, 521, 523, 541,
    547, 557, 563, 569, 571, 577, 587, 593, 599, 601,
    607, 613, 617, 619, 631, 641, 643, 647, 653, 659,
    661, 673, 677, 683, 691, 701, 709, 719, 727, 733,
    739, 743, 751, 757, 761, 769, 773, 787, 797, 809,
    811, 821, 823, 827, 829, 839, 853, 857, 859, 863,
    877, 881, 883, 887, 907, 911, 919, 929, 937, 941,
    947, 953, 967, 971, 977, 983, 991, 997, 1009, 1013,
    1019, 1021, 1031, 1033, 1039, 1049, 1051, 1061, 1063, 1069,
    1087, 1091, 1093, 1097, 1103, 1109, 1117, 1123, 1129, 1151,
    1153, 1163, 1171, 1181, 1187, 1193, 1201, 1213, 1217, 1223,
    1229, 1231, 1237, 1249, 1259, 1277, 1279, 1283, 1289, 1291,
    1297, 1301, 1303, 1307, 1319, 1321, 1327, 1361, 1367, 1373,
    1381, 1399, 1409, 1423, 1427, 1429, 1433, 1439, 1447, 1451,
    1453, 1459, 1471, 1481, 1483, 1487, 1489, 1493, 1499, 1511,
    1523, 1531, 1543, 1549, 1553, 1559, 1567, 1571, 1579, 1583,
    1597, 1601, 1607, 1609, 1613, 1619, 1621, 1627, 1637, 1657,
    1663, 1667, 1669, 1693, 1697, 1699, 1709, 1721, 1723, 1733,
    1741, 1747, 1753, 1759, 1777, 1783, 1787, 1789, 1801, 1811,
    1823, 1831, 1847, 1861, 1867, 1871, 1873, 1877, 1879, 1889,
    1901, 1907, 1913, 1931, 1933, 1949, 1951, 1973, 1979, 1987,
    1993, 1997, 1999, 2003, 2011, 2017, 2027, 2029, 2039, 2053,
    2063, 2069, 2081, 2083, 2087, 2089, 2099, 2111, 2113, 2129,
    2131, 2137, 2141, 2143, 2153, 2161, 2179, 2203, 2207, 2213,
    2221, 2237, 2239, 2243, 2251, 2267, 2269, 2273, 2281, 2287,
    2293, 2297, 2309, 2311, 2333, 2339, 2341, 2347, 2351, 2357,
    2371, 2377, 2381, 2383, 2389, 2393, 2399, 2411, 2417, 2423,
    2437, 2441, 2447, 2459, 2467, 2473, 2477, 2503, 2521, 2531,
    2539, 2543, 2549, 2551, 2557, 2579, 2591, 2593, 2609, 2617,
    2621, 2633, 2647, 2657, 2659, 2663, 2671, 2677, 2683, 2687,
    2689, 2693, 2699, 2707, 2711, 2713, 2719, 2729, 2731, 2741,
    2749, 2753, 2767, 2777, 2789, 2791, 2797, 2801, 2803, 2819,
    2833, 2837, 2843, 2851, 2857, 2861, 2879, 2887, 2897, 2903,
    2909, 2917, 2927, 2939, 2953, 2957, 2963, 2969, 2971, 2999,
    3001, 3011, 3019, 3023, 3037, 3041, 3049, 3061, 3067, 3079,
    3083, 3089, 3109, 3119, 3121, 3137, 3163, 3167, 3169, 3181,
    3187, 3191, 3203, 3209, 3217, 3221, 3229, 3251, 3253, 3257,
    3259, 3271, 3299, 3301, 3307, 3313, 3319, 3323, 3329, 3331,
    3343, 3347, 3359, 3361, 3371, 3373, 3389, 3391, 3407, 3413,
    3433, 3449, 3457, 3461, 3463, 3467, 3469, 3491, 3499, 3511,
    3517, 3527, 3529, 3533, 3539, 3541, 3547, 3557, 3559, 3571,
    3581, 3583, 3593, 3607, 3613, 3617, 3623, 3631, 3637, 3643,
    3659, 3671, 3673, 3677, 3691, 3697, 3701, 3709, 3719, 3727,
    3733, 3739, 3761, 3767, 3769, 3779, 3793, 3797, 3803, 3821,
    3823, 3833, 3847, 3851, 3853, 3863, 3877, 3881, 3889, 3907,
    3911, 3917, 3919, 3923, 3929, 3931, 3943, 3947, 3967, 3989,
    4001, 4003, 4007, 4013, 4019, 4021, 4027, 4049, 4051, 4057,
    4073, 4079, 4091, 4093, 4099, 4111, 4127, 4129, 4133, 4139,
    4153, 4157, 4159, 4177, 4201, 4211, 4217, 4219, 4229, 4231,
    4241, 4243, 4253, 4259, 4261, 4271, 4273, 4283, 4289, 4297,
    4327, 4337, 4339, 4349, 4357, 4363, 4373, 4391, 4397, 4409,
    4421, 4423, 4441, 4447, 4451, 4457, 4463, 4481, 4483, 4493,
    4507, 4513, 4517, 4519, 4523, 4547, 4549, 4561, 4567, 4583,
    4591, 4597, 4603, 4621, 4637, 4639, 4643, 4649, 4651, 4657,
    4663, 4673, 4679, 4691, 4703, 4721, 4723, 4729, 4733, 4751,
    4759, 4783, 4787, 4789, 4793, 4799, 4801, 4813, 4817, 4831,
    4861, 4871, 4877, 4889, 4903, 4909, 4919, 4931, 4933, 4937,
    4943, 4951, 4957, 4967, 4969, 4973, 4987, 4993, 4999, 5003,
    5009, 5011, 5021, 5023, 5039, 5051, 5059, 5077, 5081, 5087,
    5099, 5101, 5107, 5113, 5119, 5147, 5153, 5167, 5171, 5179,
    5189, 5197, 5209, 5227, 5231, 5233, 5237, 5261, 5273, 5279,
    5281, 5297, 5303, 5309, 5323, 5333, 5347, 5351, 5381, 5387,
    5393, 5399, 5407, 5413, 5417, 5419, 5431, 5437, 5441, 5443,
    5449, 5471, 5477, 5479, 5483, 5501, 5503, 5507, 5519, 5521,
    5527, 5531, 5557, 5563, 5569, 5573, 5581, 5591, 5623, 5639,
    5641, 5647, 5651, 5653, 5657, 5659, 5669, 5683, 5689, 5693,
    5701, 5711, 5717, 5737, 5741, 5743, 5749, 5779, 5783, 5791,
    5801, 5807, 5813, 5821, 5827, 5839, 5843, 5849, 5851, 5857,
    5861, 5867, 5869, 5879, 5881, 5897, 5903, 5923, 5927, 5939,
    5953, 5981, 5987, 6007, 6011, 6029, 6037, 6043, 6047, 6053,
    6067, 6073, 6079, 6089, 6091, 6101, 6113, 6121, 6131, 6133,
    6143, 6151, 6163, 6173, 6197, 6199, 6203, 6211, 6217, 6221,
    6229, 6247, 6257, 6263, 6269, 6271, 6277, 6287, 6299, 6301,
    6311, 6317, 6323, 6329, 6337, 6343, 6353, 6359, 6361, 6367,
    6373, 6379, 6389, 6397, 6421, 6427, 6449, 6451, 6469, 6473,
    6481, 6491, 6521, 6529, 6547, 6551, 6553, 6563, 6569, 6571,
    6577, 6581, 6599, 6607, 6619, 6637, 6653, 6659, 6661, 6673,
    6679, 6689, 6691, 6701, 6703, 6709, 6719, 6733, 6737, 6761,
    6763, 6779, 6781, 6791, 6793, 6803, 6823, 6827, 6829, 6833,
    6841, 6857, 6863, 6869, 6871, 6883, 6899, 6907, 6911, 6917,
    6947, 6949, 6959, 6961, 6967, 6971, 6977, 6983, 6991, 6997,
    7001, 7013, 7019, 7027, 7039, 7043, 7057, 7069, 7079, 7103,
    7109, 7121, 7127, 7129, 7151, 7159, 7177, 7187, 7193, 7207,
    7211, 7213, 7219, 7229, 7237, 7243, 7247, 7253, 7283, 7297,
    7307, 7309, 7321, 7331, 7333, 7349, 7351, 7369, 7393, 7411,
    7417, 7433, 7451, 7457, 7459, 7477, 7481, 7487, 7489, 7499,
    7507, 7517, 7523, 7529, 7537, 7541, 7547, 7549, 7559, 7561,
    7573, 7577, 7583, 7589, 7591, 7603, 7607, 7621, 7639, 7643,
    7649, 7669, 7673, 7681, 7687, 7691, 7699, 7703, 7717, 7723,
    7727, 7741, 7753, 7757, 7759, 7789, 7793, 7817, 7823, 7829,
    7841, 7853, 7867, 7873, 7877, 7879, 7883, 7901, 7907, 7919,
    7927, 7933, 7937, 7949, 7951, 7963, 7993, 8009, 8011, 8017,
    8039, 8053, 8059, 8069, 8081, 8087, 8089, 8093, 8101, 8111,
    8117, 8123, 8147, 8161, 8167, 8171, 8179, 8191, 8209, 8219,
    8221, 8231, 8233, 8237, 8243, 8263, 8269, 8273, 8287, 8291,
    8293, 8297, 8311, 8317, 8329, 8353, 8363, 8369, 8377, 8387,
    8389, 8419, 8423, 8429, 8431, 8443, 8447, 8461, 8467, 8501,
    8513, 8521, 8527, 8537, 8539, 8543, 8563, 8573, 8581, 8597,
    8599, 8609, 8623, 8627, 8629, 8641, 8647, 8663, 8669, 8677,
    8681, 8689, 8693, 8699, 8707, 8713, 8719, 8731, 8737, 8741,
    8747, 8753, 8761, 8779, 8783, 8803, 8807, 8819, 8821, 8831,
    8837, 8839, 8849, 8861, 8863, 8867, 8887, 8893, 8923, 8929,
    8933, 8941, 8951, 8963, 8969, 8971, 8999, 9001, 9007, 9011,
    9013, 9029, 9041, 9043, 9049, 9059, 9067, 9091, 9103, 9109,
    9127, 9133, 9137, 9151, 9157, 9161, 9173, 9181, 9187, 9199,
    9203, 9209, 9221, 9227, 9239, 9241, 9257, 9277, 9281, 9283,
    9293, 9311, 9319, 9323, 9337, 9341, 9343, 9349, 9371, 9377,
    9391, 9397, 9403, 9413, 9419, 9421, 9431, 9433, 9437, 9439,
    9461, 9463, 9467, 9473, 9479, 9491, 9497, 9511, 9521, 9533,
    9539, 9547, 9551, 9587, 9601, 9613, 9619, 9623, 9629, 9631,
    9643, 9649, 9661, 9677, 9679, 9689, 9697, 9719, 9721, 9733,
    9739, 9743, 9749, 9767, 9769, 9781, 9787, 9791, 9803, 9811,
    9817, 9829, 9833, 9839, 9851, 9857, 9859, 9871, 9883, 9887,
    9901, 9907, 9923, 9929, 9931, 9941, 9949, 9967, 9973,
};

/** Scratch buffer size used by NaiveFdnReverb
 *
 * @param   num_delays_cap  Maximum allowed number of delays
 * @param   delay_len_cap   Maximum allowed delay length
 * @param   block_size_cap  Maximum allowed block size
 *
 * @return the size of the scratch buffer
 */
#define NAIVE_FDN_REVERB_SCRATCH_SIZE(num_delays_cap, block_size_cap) (             \
    NAIVE_CEIL_8_BYTES(sizeof(NaiveF32) * (block_size_cap)) * 2 +                   \
    NAIVE_CEIL_8_BYTES(sizeof(NaiveF32) * (block_size_cap)) * num_delays_cap * 3 +  \
    NAIVE_CEIL_8_BYTES(sizeof(NaiveF32) * (block_size_cap)) * 2)

NaiveErr naive_fdn_reverb_init(
    NaiveFdnReverb *self,
    void *alloc_context,
    NaiveAllocFunc alloc,
    NaiveI32 sample_rate,
    NaiveI32 block_size_cap,
    NaiveI32 pre_delay_len_cap,
    NaiveI32 num_delays_cap,
    NaiveI32 delay_len_cap)
{
    NaiveErr err = NAIVE_OK;

    self->sample_rate = sample_rate;
    self->block_size_cap = block_size_cap;
    self->num_delays_cap = num_delays_cap;
    self->pre_delay_len = 0;

    NaiveErr rc = naive_delay_buf_init(&self->left_pre_delay_buf, alloc_context, alloc, pre_delay_len_cap);
    if (!err && rc)
        err = rc;

    rc = naive_delay_buf_init(&self->right_pre_delay_buf, alloc_context, alloc, pre_delay_len_cap);
    if (!err && rc)
        err = rc;

    self->num_delays = 0;
    self->delay_lens = alloc(alloc_context, NAIVE_MEM_PARAM, sizeof(NaiveI32) * (NaiveUSize)num_delays_cap);
    if (self->delay_lens) {
        for (NaiveI32 i = 0; i < num_delays_cap; ++i) {
            self->delay_lens[i] = 0;
        }
    } else if (!err) {
        err = NAIVE_ERR_NOMEM;
    }

    self->delay_bufs = alloc(alloc_context, NAIVE_MEM_STATE, sizeof(NaiveDelayBuf) * (NaiveUSize)num_delays_cap);
    if (self->delay_bufs) {
        for (NaiveI32 i = 0; i < num_delays_cap; ++i) {
            rc = naive_delay_buf_init(&self->delay_bufs[i], alloc_context, alloc, delay_len_cap);
            if (!err && rc)
                err = rc;
        }
    } else if (!err) {
        err = NAIVE_ERR_NOMEM;
    }

    self->in_gains = alloc(alloc_context, NAIVE_MEM_PARAM, sizeof(NaiveF32) * (NaiveUSize)num_delays_cap);
    if (self->in_gains) {
        for (NaiveI32 i = 0; i < num_delays_cap; ++i) {
            self->in_gains[i] = 1.0;
        }
    } else if (!err) {
        err = NAIVE_ERR_NOMEM;
    }

    self->out_gains = alloc(alloc_context, NAIVE_MEM_PARAM, sizeof(NaiveF32) * (NaiveUSize)num_delays_cap);
    if (self->out_gains) {
        for (NaiveI32 i = 0; i < num_delays_cap; ++i) {
            self->out_gains[i] = 1.0;
        }
    } else if (!err) {
        err = NAIVE_ERR_NOMEM;
    }

    self->delay_filters = alloc(alloc_context, NAIVE_MEM_STATE, sizeof(NaiveCascadedIir1stDf1) * (NaiveUSize)num_delays_cap);
    if (self->delay_filters) {
        for (NaiveI32 i = 0; i < num_delays_cap; ++i) {
            rc = naive_cascaded_iir_1st_df1_init(&self->delay_filters[i], alloc_context, alloc, 2);
            if (!err && rc)
                err = rc;
        }
    } else if (!err) {
        err = NAIVE_ERR_NOMEM;
    }

    self->feedback_mat = alloc(alloc_context, NAIVE_MEM_PARAM, sizeof(NaiveF32) * (NaiveUSize)num_delays_cap * (NaiveUSize)num_delays_cap);
    if (self->feedback_mat) {
        for (NaiveI32 i = 0; i < num_delays_cap * num_delays_cap; ++i) {
            self->feedback_mat[i] = 0.0f;
        }
    } else if (!err) {
        err = NAIVE_ERR_NOMEM;
    }

    self->dry_gain = 1.0f;
    self->wet_gain = 0.0f;
    self->min_acoustic_path = 0.0f;
    self->max_acoustic_path = 0.0f;
    self->low_mid_xover_freq = 1.0f;
    self->high_damp_freq = 1.0f;
    self->dc_reverb_time = 0.0f;
    self->mid_freq_reverb_time = 0.0f;

    naive_fdn_reverb_set_default_params(self);

    self->scratch = alloc(alloc_context, NAIVE_MEM_SCRATCH, NAIVE_FDN_REVERB_SCRATCH_SIZE(num_delays_cap, block_size_cap));

    return err;
}

NaiveErr naive_fdn_reverb_process(NaiveFdnReverb *self, NaiveF32 *left_in, NaiveF32 *right_in, NaiveF32 *left_out, NaiveF32 *right_out, NaiveI32 block_size)
{
    if (block_size < 0 || block_size > self->block_size_cap)
        return NAIVE_ERR_INVALID_PARAMETER;

    if (block_size == 0) {
        return NAIVE_OK;
    }

    NaiveI32 num_delays = self->num_delays;
    NaiveF32 *left_predelayed = self->scratch;
    NaiveF32 *right_predelayed = (NaiveF32*)((NaiveUIntPtr)left_predelayed + NAIVE_CEIL_8_BYTES(sizeof(NaiveF32) * block_size));
    NaiveF32 *predelayed[] = {left_predelayed, right_predelayed};

    // input buffer of each delay line
    NaiveF32 *x = (NaiveF32*)((NaiveUIntPtr)right_predelayed + NAIVE_CEIL_8_BYTES(sizeof(NaiveF32) * block_size));

    // delayed input of each delay line
    NaiveF32 *xd = (NaiveF32 *)((NaiveUIntPtr)x + NAIVE_CEIL_8_BYTES(sizeof(NaiveF32) * block_size) * (NaiveUSize)num_delays);

    // feedback output of each delay line
    NaiveF32 *s = (NaiveF32*)((NaiveUIntPtr)xd + NAIVE_CEIL_8_BYTES(sizeof(NaiveF32) * block_size) * (NaiveUSize)num_delays);

    // output buffers
    NaiveF32 *yl = (NaiveF32*)((NaiveUIntPtr)s + NAIVE_CEIL_8_BYTES(sizeof(NaiveF32) * block_size) * (NaiveUSize)num_delays);
    NaiveF32 *yr = (NaiveF32*)((NaiveUIntPtr)yl + NAIVE_CEIL_8_BYTES(sizeof(NaiveF32) * block_size));

    NaiveF32 *feedback_mat = self->feedback_mat;
    NaiveF32 *in_gains = self->in_gains;
    NaiveF32 *out_gains = self->out_gains;
    NaiveF32 dry_gain = self->dry_gain;
    NaiveF32 wet_gain = self->wet_gain;

    // write to pre delay buffers if not enough pre-delayed data
    NaiveI32 n = 0;
    while (n < block_size && self->left_pre_delay_buf.len < self->pre_delay_len) {
        NaiveI32 len = NAIVE_MIN(self->pre_delay_len - self->left_pre_delay_buf.len,
                                 block_size - n);
        naive_delay_buf_write(&self->left_pre_delay_buf, &left_in[n], len);
        naive_delay_buf_write(&self->right_pre_delay_buf, &right_in[n], len);
        memset(&left_predelayed[n], 0, sizeof(NaiveF32) * (NaiveUSize)len);
        memset(&right_predelayed[n], 0, sizeof(NaiveF32) * (NaiveUSize)len);
        n += len;
    }

    // read pre deleyed data
    while (n < block_size && self->left_pre_delay_buf.len >= self->pre_delay_len) {
        NaiveI32 len = NAIVE_MIN(self->pre_delay_len, block_size - n);
        memcpy(&left_predelayed[n], naive_delay_buf_front(&self->left_pre_delay_buf), sizeof(NaiveF32) * (NaiveUSize)len);
        memcpy(&right_predelayed[n], naive_delay_buf_front(&self->right_pre_delay_buf), sizeof(NaiveF32) * (NaiveUSize)len);
        naive_delay_buf_drain(&self->left_pre_delay_buf, len);
        naive_delay_buf_drain(&self->right_pre_delay_buf, len);
        naive_delay_buf_write(&self->left_pre_delay_buf, &left_in[n], len);
        naive_delay_buf_write(&self->right_pre_delay_buf, &right_in[n], len);
        n += len;
    }

    // apply delay line input gains
    for (NaiveI32 i = 0; i < num_delays; ++i) {
        NaiveF32 *xi = (NaiveF32*)((NaiveUIntPtr)x + NAIVE_CEIL_8_BYTES(sizeof(NaiveF32) * block_size) * (NaiveUSize)i);

        // select predelayed left/right for input to each delay line
        NaiveF32 *u = predelayed[i & 1];

        memcpy(xi, u, sizeof(NaiveF32) * (NaiveUSize)block_size);
        naive_gain(xi, block_size, in_gains[i]);
    }

    n = 0;
    while (n < block_size) {
        NaiveI32 len1 = NAIVE_MIN(self->delay_lens[0], block_size - n);

        // prepare the output from each delay line
        for (NaiveI32 i = 0; i < num_delays; ++i) {
            // xi is the input of each delay line
            NaiveF32 *xi = (NaiveF32*)((NaiveUIntPtr)x + NAIVE_CEIL_8_BYTES(sizeof(NaiveF32) * block_size) * (NaiveUSize)i);

            // xdi is the delayed input of each delay line
            NaiveF32 *xdi = (NaiveF32*)((NaiveUIntPtr)xd + NAIVE_CEIL_8_BYTES(sizeof(NaiveF32) * block_size) * (NaiveUSize)i);

            NaiveI32 nn = 0;

            // if not enough delayed data, write to the delay buffer
            while (nn < len1 && self->delay_bufs[i].len < self->delay_lens[i]) {
                NaiveI32 len2 = NAIVE_MIN(self->delay_lens[i] - self->delay_bufs[i].len, len1 - nn);
                memset(&xdi[n + nn], 0, sizeof(NaiveF32) * (NaiveUSize)len2);
                naive_delay_buf_write(&self->delay_bufs[i], &xi[n + nn], len2);
                nn += len2;
            }

            // read delayed data
            while (nn < len1 && self->delay_bufs[i].len >= self->delay_lens[i]) {
                NaiveI32 len2 = NAIVE_MIN(self->delay_lens[i], len1 - nn);
                memcpy(&xdi[n + nn], naive_delay_buf_front(&self->delay_bufs[i]), sizeof(NaiveF32) * (NaiveUSize)len2);
                naive_delay_buf_drain(&self->delay_bufs[i], len2);
                nn += len2;
            }
        }

        // feedback matrix and delay line filters
        for (NaiveI32 i = 0; i < num_delays; ++i) {
            // si is the output after applying feedback matrix and delay line filters
            NaiveF32 *si = (NaiveF32 *)((NaiveUIntPtr)s + NAIVE_CEIL_8_BYTES(sizeof(NaiveF32) * block_size) * (NaiveUSize)i);

            NaiveF32 *xi = (NaiveF32*)((NaiveUIntPtr)x + NAIVE_CEIL_8_BYTES(sizeof(NaiveF32) * block_size) * (NaiveUSize)i);

            // apply FDN feedback matrix
            memset(&si[n], 0, sizeof(NaiveF32) * (NaiveUSize)len1);
            for (NaiveI32 j = 0; j < num_delays; ++j) {
                NaiveF32 *xdj = (NaiveF32*)((NaiveUIntPtr)xd + NAIVE_CEIL_8_BYTES(sizeof(NaiveF32) * block_size) * (NaiveUSize)j);
                naive_mix_with_gain(&si[n], &xdj[n], feedback_mat[num_delays * j + i], len1);
            }

            // apply FDN delay filters
            naive_cascaded_iir_1st_df1_process(&self->delay_filters[i], &si[n], len1);

            // mix with the delay line input and write to the delay buffer
            naive_mix(&xi[n], &si[n], len1);
            naive_delay_buf_write(&self->delay_bufs[i], &xi[n], len1);
        }

        n += len1;
    }

    memset(yl, 0, sizeof(NaiveF32) * (NaiveUSize)block_size);
    for (NaiveI32 i = 0; i < num_delays; i += 2) {
        NaiveF32 *si = (NaiveF32*)((NaiveUIntPtr)s + NAIVE_CEIL_8_BYTES(sizeof(NaiveF32) * block_size) * (NaiveUSize)i);
        for (NaiveI32 nn = 0; nn < block_size; ++nn) {
            yl[nn] += out_gains[i] * si[nn];
        }
    }

    memset(yr, 0, sizeof(NaiveF32) * (NaiveUSize)block_size);
    for (NaiveI32 i = 1; i < num_delays; i += 2) {
        NaiveF32 *si = (NaiveF32 *)((NaiveUIntPtr)s + NAIVE_CEIL_8_BYTES(sizeof(NaiveF32) * block_size) * (NaiveUSize)i);
        for (NaiveI32 nn = 0; nn < block_size; ++nn) {
            yr[nn] += out_gains[i] * si[nn];
        }
    }

    // naive_peq_process(&self->left_wet_eq_states, &params->wet_eq_coeffs, yl, block_size, wet_eq_scratch);
    // naive_peq_process(&self->right_wet_eq_states, &params->wet_eq_coeffs, yr, block_size, wet_eq_scratch);

    for (NaiveI32 nn = 0; nn < block_size; ++nn) {
        left_out[nn] = wet_gain * yl[nn] + dry_gain * left_in[nn];
        right_out[nn] = wet_gain * yr[nn] + dry_gain * right_in[nn];
    }

    return NAIVE_OK;
}

void naive_fdn_reverb_reset(NaiveFdnReverb *self)
{
    naive_delay_buf_reset(&self->left_pre_delay_buf);
    naive_delay_buf_reset(&self->right_pre_delay_buf);
    for (NaiveI32 i = 0; i < self->num_delays_cap; ++i) {
        naive_delay_buf_reset(&self->delay_bufs[i]);
        naive_cascaded_iir_1st_df1_reset(&self->delay_filters[i]);
    }
}

NAIVE_INLINE void sub_hadamard_multiply_scalar(NaiveF32 *hadamard, NaiveI32 row_stride, NaiveI32 order, NaiveF32 scalar)
{
    for (NaiveI32 i = 0; i < order; ++i) {
        for (NaiveI32 j = 0; j < order; ++j) {
            hadamard[row_stride * j + i] *= scalar;
        }
    }
}

NAIVE_INLINE void make_sub_hadamard(NaiveF32 *hadamard, NaiveI32 row_stride, NaiveI32 order)
{
    if (order == 2) {
        hadamard[0] = 1.0f / sqrtf(2.0f);
        hadamard[1] = -1.0f / sqrtf(2.0f);
        hadamard[row_stride] = 1.0f / sqrtf(2.0f);
        hadamard[row_stride + 1] = 1.0f / sqrtf(2.0f);
    } else if (order > 2) {
        NaiveI32 sub_order = order / 2;
        NaiveF32 *topleft = hadamard;
        NaiveF32 *bottomleft = hadamard + sub_order;
        NaiveF32 *topright = hadamard + row_stride * sub_order;
        NaiveF32 *bottomright = hadamard + row_stride * sub_order + sub_order;
        make_sub_hadamard(topleft, row_stride, sub_order);
        sub_hadamard_multiply_scalar(topleft, row_stride, sub_order, 1.0f / sqrtf(2.0f));
        make_sub_hadamard(bottomleft, row_stride, sub_order);
        sub_hadamard_multiply_scalar(bottomleft, row_stride, sub_order, -1.0f / sqrtf(2.0f));
        make_sub_hadamard(topright, row_stride, sub_order);
        sub_hadamard_multiply_scalar(topright, row_stride, sub_order, 1.0f / sqrtf(2.0f));
        make_sub_hadamard(bottomright, row_stride, sub_order);
        sub_hadamard_multiply_scalar(bottomright, row_stride, sub_order, 1.0f / sqrtf(2.0f));
    }
}

NAIVE_INLINE void make_hadamard(NaiveF32 *hadamard, NaiveI32 order) {
    make_sub_hadamard(hadamard, order, order);
}

NaiveErr naive_fdn_reverb_set_room_params(NaiveFdnReverb *self,
                                          NaiveI32 num_delays,
                                          NaiveF32 min_acoustic_path,
                                          NaiveF32 max_acoustic_path,
                                          NaiveF32 low_mid_xover_freq,
                                          NaiveF32 high_damp_freq,
                                          NaiveF32 dc_reverb_time,
                                          NaiveF32 mid_freq_reverb_time)
{
    if (num_delays > self->num_delays_cap)
        return NAIVE_ERR_INVALID_PARAMETER;

    if ((NaiveUSize)num_delays >= sizeof(PRIMES) / sizeof(NaiveI32))
        return NAIVE_ERR_INVALID_PARAMETER;

    if (!NAIVE_IS_POW_OF_TWO(num_delays))
        return NAIVE_ERR_INVALID_PARAMETER;

    if (min_acoustic_path <= 0.0f || min_acoustic_path > max_acoustic_path)
        return NAIVE_ERR_INVALID_PARAMETER;

    if (low_mid_xover_freq <= 0 || low_mid_xover_freq >= (NaiveF32)self->sample_rate / 2)
        return NAIVE_ERR_INVALID_PARAMETER;

    if (high_damp_freq < low_mid_xover_freq || high_damp_freq >= (NaiveF32)self->sample_rate / 2)
        return NAIVE_ERR_INVALID_PARAMETER;

    if (dc_reverb_time < 0)
        return NAIVE_ERR_INVALID_PARAMETER;

    if (mid_freq_reverb_time < 0)
        return NAIVE_ERR_INVALID_PARAMETER;

    NaiveI32 delay_lens[num_delays];
    NaiveI32 delay_len_sum = 0;
    if (num_delays != self->num_delays ||
        min_acoustic_path != self->min_acoustic_path ||
        max_acoustic_path != self->max_acoustic_path)
    {
        NaiveF32 dmax = max_acoustic_path * (NaiveF32)self->sample_rate / SOUND_SPEED;
        NaiveF32 dmin = min_acoustic_path * (NaiveF32)self->sample_rate / SOUND_SPEED;

        NaiveUSize dmax_idx = 0;
        NaiveUSize dmin_idx = 0;

        for (NaiveUSize i = 0; i < sizeof(PRIMES) / sizeof(NaiveI32); ++i) {
            if (dmin_idx == 0 && (NaiveF32)PRIMES[i] >= dmin) {
                dmin_idx = i;
            }
            if (dmax_idx == 0 && (NaiveF32)PRIMES[i] >= dmax) {
                dmax_idx = i;
                break;
            }
        }

        for (NaiveUSize i = 0; i < (NaiveUSize)num_delays; ++i) {
            delay_lens[i] = PRIMES[(dmax_idx - dmin_idx) / (NaiveUSize)num_delays * i + dmin_idx];
            delay_len_sum += delay_lens[i];
        }

        /* for (NaiveI32 i = 0; i < num_delays; ++i) { */
        /*     NaiveF32 dl = powf(dmax / dmin, (NaiveF32)i / (NaiveF32)(num_delays - 1)) * dmin; */

        /*     if (dl > (NaiveF32)PRIMES[sizeof(PRIMES) / sizeof(NaiveI32) - 1]) */
        /*         return NAIVE_ERR_INVALID_PARAMETER; */

        /*     NaiveI32 delayval = 0; */
        /*     for (NaiveI32 j = 0; (NaiveUSize)j < sizeof(PRIMES) / sizeof(NaiveI32); ++j) { */
        /*         if ((NaiveF32)PRIMES[j] >= dl) { */
        /*             delayval = PRIMES[j]; */
        /*             break; */
        /*         } */
        /*     } */

        /*     delay_lens[i] = delayval; */
        /*     delay_len_sum += delayval; */
        /* } */

        /* for (NaiveI32 i = 0; i < num_delays; ++i) { */
        /*     NaiveF32 dl = powf(dmax / dmin, (NaiveF32)i / (NaiveF32)(num_delays - 1)) * dmin; */
        /*     NaiveF32 ppwr = floorf(logf(dl) / logf((NaiveF32)PRIMES[i]) + 0.5f); */
        /*     NaiveI32 delayval = (NaiveI32)powf((NaiveF32)PRIMES[i], ppwr); */

        /*     delayval = NAIVE_MIN(NAIVE_MAX(delayval - 1, DLMIN), DLMAX); */
        /*     delayval = NAIVE_MAX(delayval - 1, DLMIN); */

        /*     delay_lens[i] = delayval; */
        /*     delay_len_sum += delayval; */
        /* } */
    } else {
        for (NaiveI32 i = 0; i < self->num_delays; ++i) {
            delay_len_sum += self->delay_lens[i];
        }
    }

    if (0.15f * dc_reverb_time * (NaiveF32)self->sample_rate > (NaiveF32)delay_len_sum ||
        0.15f * mid_freq_reverb_time * (NaiveF32)self->sample_rate > (NaiveF32)delay_len_sum)
        return NAIVE_ERR_INVALID_PARAMETER;

    if (num_delays != self->num_delays) {
        make_hadamard(self->feedback_mat, num_delays);
        self->num_delays = num_delays;
    }

    if (num_delays != self->num_delays ||
        min_acoustic_path != self->min_acoustic_path ||
        max_acoustic_path != self->max_acoustic_path)
    {
        self->min_acoustic_path = min_acoustic_path;
        self->max_acoustic_path = max_acoustic_path;
        memcpy(self->delay_lens, delay_lens, sizeof(NaiveI32) * (NaiveUSize)num_delays);
    }

    self->low_mid_xover_freq = low_mid_xover_freq;
    self->high_damp_freq = high_damp_freq;
    self->dc_reverb_time = dc_reverb_time;
    self->mid_freq_reverb_time = mid_freq_reverb_time;

    // https://ccrma.stanford.edu/~jos/pasp/Zita_Rev1_Delay_Line_Filters.html
    for (NaiveI32 i = 0; i < num_delays; ++i) {
        NaiveF32 pl = (1 - NAIVE_PI * low_mid_xover_freq / (NaiveF32)self->sample_rate) / (1 + NAIVE_PI * low_mid_xover_freq / (NaiveF32)self->sample_rate);
        NaiveF32 g0 = powf(10.0f, -3 * (NaiveF32)self->delay_lens[i] / (dc_reverb_time * (NaiveF32)self->sample_rate));
        NaiveF32 gm = powf(10.0f, -3 * (NaiveF32)self->delay_lens[i] / (mid_freq_reverb_time * (NaiveF32)self->sample_rate));
        self->delay_filters[i].coeffs[0].b0 = gm + (g0 - gm) * (1 - pl) / 2.0f;
        self->delay_filters[i].coeffs[0].b1 = -gm * pl + (g0 - gm) * (1 - pl) / 2.0f;
        self->delay_filters[i].coeffs[0].a1 = -pl;
        self->delay_filters[i].gain = 1.0f;

        NaiveF32 neg_half_b = (1 - gm * gm * cosf(2 * NAIVE_PI * high_damp_freq / (NaiveF32)self->sample_rate)) / (1 - gm * gm);
        NaiveF32 ph = neg_half_b - sqrtf(neg_half_b * neg_half_b - 1.0f);
        self->delay_filters[i].coeffs[1].b0 = 1 - ph;
        self->delay_filters[i].coeffs[1].b1 = 0;
        self->delay_filters[i].coeffs[1].a1 = -ph;
        self->delay_filters[i].gain = 1.0f;

        self->delay_filters[i].num_fos = 2;
    }

    return NAIVE_OK;
}

NaiveErr naive_fdn_reverb_set_pre_delay_time(NaiveFdnReverb *self, NaiveF32 pre_delay_time)
{
    if (pre_delay_time < 0)
        return NAIVE_ERR_INVALID_PARAMETER;

    NaiveI32 delay_len = (NaiveI32)(pre_delay_time * (NaiveF32)self->sample_rate);

    if (delay_len > self->left_pre_delay_buf.size)
        return NAIVE_ERR_INVALID_PARAMETER;

    self->pre_delay_len = delay_len;

    return NAIVE_OK;
}

NaiveErr naive_fdn_reverb_set_input_gain(NaiveFdnReverb *self, NaiveF32 input_gain)
{
    for (NaiveI32 i = 0; i < self->num_delays; ++i) {
        self->in_gains[i] = input_gain;
    }
    return NAIVE_OK;
}

NaiveErr naive_fdn_reverb_set_output_gain(NaiveFdnReverb *self, NaiveF32 output_gain)
{
    for (NaiveI32 i = 0; i < self->num_delays; ++i) {
        self->out_gains[i] = sqrtf(output_gain / (NaiveF32)self->num_delays);
    }
    return NAIVE_OK;
}

NaiveErr naive_fdn_reverb_set_dry_gain(NaiveFdnReverb *self, NaiveF32 dry_gain)
{
    self->dry_gain = dry_gain;
    return NAIVE_OK;
}

NaiveErr naive_fdn_reverb_set_wet_gain(NaiveFdnReverb *self, NaiveF32 wet_gain)
{
    self->wet_gain = wet_gain;
    return NAIVE_OK;
}

void naive_fdn_reverb_set_default_params(NaiveFdnReverb *self)
{
    naive_fdn_reverb_set_pre_delay_time(self, 0.005f);
    naive_fdn_reverb_set_room_params(self,
                                     16,
                                     1.0f,
                                     10.0f,
                                     300.0f,
                                     4000.0f,
                                     1.2f,
                                     1.0f);
    naive_fdn_reverb_set_input_gain(self, 1.0f);
    naive_fdn_reverb_set_output_gain(self, 1.0f);
    self->dry_gain = 0.293f;
    self->wet_gain = 0.707f;
}
