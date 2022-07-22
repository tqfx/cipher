/*!
 @file output.h
 @brief generated from https://github.com/tqfx/liba.git
 @copyright Copyright (C) 2020-present tqfx, All rights reserved.
*/

#ifndef __A_OUTPUT_H__
#define __A_OUTPUT_H__

#if defined(__MINGW32__)
#undef __USE_MINGW_ANSI_STDIO
#define __USE_MINGW_ANSI_STDIO 1
#define __format__(...)
#endif /* __MINGW32__ */

#ifndef __has_attribute
#define __attribute__(...)
#define __has_attribute(...) 0
#endif /* __has_attribute */

#if defined(__has_builtin)
#define likely(...) __builtin_expect((__VA_ARGS__), 1)
#define unlikely(...) __builtin_expect((__VA_ARGS__), 0)
#else /* !__has_builtin */
#define __has_builtin(...) 0
#define likely(...) (__VA_ARGS__)
#define unlikely(...) (__VA_ARGS__)
#endif /* __has_builtin */

#include <stddef.h>
#include <stdint.h>

#undef CAST
#if defined(__cplusplus)
#define CAST(T, ...) static_cast<T>(__VA_ARGS__)
#else /* !__cplusplus */
#define CAST(T, ...) (T)(__VA_ARGS__)
#endif /* __cplusplus */

#if defined(__cplusplus)
#undef NULL
#define NULL nullptr
#endif /* __cplusplus */

typedef int8_t i8_t;
typedef uint8_t u8_t;

typedef int16_t i16_t;
typedef uint16_t u16_t;

typedef int32_t i32_t;
typedef uint32_t u32_t;

typedef int64_t i64_t;
typedef uint64_t u64_t;

typedef intmax_t imax_t;
typedef uintmax_t umax_t;

typedef int int_t;
typedef unsigned int uint_t;

typedef char *str_t;
typedef void *vptr_t;
typedef const char *cstr_t;
typedef const void *cptr_t;
typedef unsigned char byte_t;

enum
{
    SUCCESS,
    FAILURE,
    INVALID,
};

#endif /* __A_OUTPUT_H__ */
