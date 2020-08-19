#ifndef __NAIVE_PLATDEFS_H__
#define __NAIVE_PLATDEFS_H__

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN64) || defined(__x86_64) || defined(__amd64)
#define NAIVE_X86_64 1
#elif defined(__i386__)
#define NAIVE_X86 1
#endif

#if defined(__APPLE__)
#define NAIVE_APPLE 1
#endif

#if defined(_WIN64) || defined(_WIN32)
#define NAIVE_WINDOWS 1
#elif defined(__linux__)
#define NAIVE_LINUX 1
#elif NAIVE_APPLE
#if TARGET_OS_MAC
#define NAIVE_MAC 1
#else
#define NAIVE_IOS 1
#endif
#endif

#if defined(_MSC_VER)
#define NAIVE_MSVC 1
#elif defined(__GNUC__)
#define NAIVE_GCC 1
#elif defined(__clang__)
#define NAIVE_CLANG 1
#endif

#if NAIVE_MSVC && _MSC_VER < 1800
#define NAIVE_INLINE static __inline
#define NAIVE_CONST
#define NAIVE_RESTRICT __restrict
#else
#define NAIVE_INLINE static inline
#define NAIVE_CONST const
#define NAIVE_RESTRICT restrict
#endif

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: __NAIVE_PLATDEFS_H__ */
