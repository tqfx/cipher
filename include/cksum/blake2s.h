/*!
 @file blake2s.h
 @brief RFC 7693 compliant BLAKE implementation
 @details https://www.ietf.org/rfc/rfc7693.txt
 @copyright Copyright (C) 2020-present tqfx, All rights reserved.
*/

#ifndef __CKSUM_BLAKE2S_H__
#define __CKSUM_BLAKE2S_H__

#include <stddef.h>
#include <stdint.h>

#define BLAKE2S_BUFSIZ 0x40
#define BLAKE2S_OUTSIZ 0x20
#define BLAKE2S_128_OUTSIZ (128 >> 3)
#define BLAKE2S_160_OUTSIZ (160 >> 3)
#define BLAKE2S_224_OUTSIZ (224 >> 3)
#define BLAKE2S_256_OUTSIZ (256 >> 3)

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
#endif /* __GNUC__ || __clang__ */

typedef struct blake2s_s
{
    uint32_t __t[2];
    uint32_t __f[2];
    uint32_t __cursiz;
    uint32_t outsiz;
    uint32_t __state[BLAKE2S_OUTSIZ >> 2];
    unsigned char out[BLAKE2S_OUTSIZ];
    unsigned char __buf[BLAKE2S_BUFSIZ];
    unsigned char __lastnode;
} blake2s_s;

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif /* __GNUC__ || __clang__ */

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

void blake2s_128_init(blake2s_s *ctx);
void blake2s_160_init(blake2s_s *ctx);
void blake2s_224_init(blake2s_s *ctx);
void blake2s_256_init(blake2s_s *ctx);
int blake2s_init(blake2s_s *ctx, size_t siz, const void *pdata, size_t nbyte);
int blake2s_proc(blake2s_s *ctx, const void *pdata, size_t nbyte);
unsigned char *blake2s_done(blake2s_s *ctx, void *out);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* __CKSUM_BLAKE2S_H__ */
