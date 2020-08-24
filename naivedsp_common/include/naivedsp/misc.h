#ifndef __NAIVE_MISC_H__
#define __NAIVE_MISC_H__

#ifdef __cplusplus
extern "C" {
#endif

#define NAIVE_CHECK_SAMPLE_RATE(sr) do {        \
    if ((sr) != 8000 &&                         \
        (sr) != 11025 &&                        \
        (sr) != 16000 &&                        \
        (sr) != 22050 &&                        \
        (sr) != 32000 &&                        \
        (sr) != 44100 &&                        \
        (sr) != 48000 &&                        \
        (sr) != 64000 &&                        \
        (sr) != 88200 &&                        \
        (sr) != 96000 &&                        \
        (sr) != 128000 &&                       \
        (sr) != 176400 &&                       \
        (sr) != 192000) {                       \
        return NAIVE_ERR_INVALID_PARAMETER;     \
    }                                           \
} while (0)


#ifdef __cplusplus
}
#endif

#endif /* end of include guard: __NAIVE_MISC_H__ */
