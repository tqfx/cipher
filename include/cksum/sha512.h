/*!
 @file sha512.h
 @brief RFC 5754 compliant SHA2 implementation
 @details https://www.ietf.org/rfc/rfc5754.txt
 @copyright Copyright (C) 2020-present tqfx, All rights reserved.
*/

#ifndef __CKSUM_SHA512_H__
#define __CKSUM_SHA512_H__

#include <stddef.h>
#include <stdint.h>

#define SHA512_BUFSIZ 0x80
#define SHA512_OUTSIZ (512 >> 3)
#define SHA384_OUTSIZ (384 >> 3)
#define SHA512_224_OUTSIZ (224 >> 3)
#define SHA512_256_OUTSIZ (256 >> 3)

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
#endif /* __GNUC__ || __clang__ */

typedef struct sha512_s
{
    uint64_t __length;
    uint64_t __state[SHA512_OUTSIZ >> 3];
    unsigned char __buf[SHA512_BUFSIZ];
    unsigned char out[SHA512_OUTSIZ];
    uint32_t __cursiz;
} sha512_s;

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif /* __GNUC__ || __clang__ */

typedef sha512_s sha384_s;
typedef sha512_s sha512_224_s;
typedef sha512_s sha512_256_s;

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

void sha512_init(sha512_s *ctx);
int sha512_proc(sha512_s *ctx, const void *pdata, size_t nbyte);
unsigned char *sha512_done(sha512_s *ctx, void *out);

void sha384_init(sha512_s *ctx);
#define sha384_proc(ctx, pdata, nbyte) sha512_proc(ctx, pdata, nbyte)
unsigned char *sha384_done(sha512_s *ctx, void *out);

void sha512_224_init(sha512_s *ctx);
#define sha512_224_proc(ctx, pdata, nbyte) sha512_proc(ctx, pdata, nbyte)
unsigned char *sha512_224_done(sha512_s *ctx, void *out);

void sha512_256_init(sha512_s *ctx);
#define sha512_256_proc(ctx, pdata, nbyte) sha512_proc(ctx, pdata, nbyte)
unsigned char *sha512_256_done(sha512_s *ctx, void *out);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* __CKSUM_SHA512_H__ */
