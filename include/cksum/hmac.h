/*!
 @file hmac.h
 @brief RFC 2104 compliant HMAC implementation
 @details https://www.ietf.org/rfc/rfc2104.txt
 @copyright Copyright (C) 2020 tqfx, All rights reserved.
*/

#ifndef __CKSUM_HMAC_H__
#define __CKSUM_HMAC_H__

#include "hash.h"

#define HMAC_BUFSIZ HASH_BUFSIZ

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
#endif /* __GNUC__ || __clang__ */

typedef struct hmac_s
{
    hash_u __state[1];
    unsigned int outsiz;
    const hash_s *__hash;
    unsigned char buf[HMAC_BUFSIZ];
} hmac_s;

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif /* __GNUC__ || __clang__ */

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*!
 @brief Initialize function for HMAC.
 @param[in,out] ctx points to an instance of HMAC.
 @param[in] hash points to an instance of hash descriptor.
 @param[in] pdata points to key.
 @param[in] nbyte length of key.
 @return the execution state of the function
  @retval 0 success
*/
int hmac_init(hmac_s *ctx, const hash_s *hash, const void *pdata, size_t nbyte);

/*!
 @brief Process function for HMAC.
 @param[in,out] ctx points to an instance of HMAC.
 @param[in] pdata points to text.
 @param[in] nbyte length of text.
 @return the execution state of the function
  @retval 0 success
*/
int hmac_proc(hmac_s *ctx, const void *pdata, size_t nbyte);

/*!
 @brief Terminate function for HMAC.
 @param[in,out] ctx points to an instance of HMAC.
 @param[in,out] out points to buffer that holds the digest.
 @return the digest internal buffer.
  @retval 0 generic invalid argument.
*/
unsigned char *hmac_done(hmac_s *ctx, void *out);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* __CKSUM_HMAC_H__ */
