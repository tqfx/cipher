/*!
 @file hash.h
 @brief hash library
 @copyright Copyright (C) 2020-present tqfx, All rights reserved.
*/

#ifndef __CKSUM_HASH_H__
#define __CKSUM_HASH_H__

#include "md5.h"

#include "sha1.h"

#include "sha256.h"
#include "sha512.h"

#include "sha3.h"

#include "blake2s.h"
#include "blake2b.h"

/*!
    shake128    0xA8    168
    sha3-224    0x90    144
*/
#define HASH_BUFSIZ 0xA8

typedef union hash_u
{
#if defined(__CKSUM_MD5_H__)
    md5_s md5[1];
#endif /* __CKSUM_MD5_H__ */
#if defined(__CKSUM_SHA1_H__)
    sha1_s sha1[1];
#endif /* __CKSUM_SHA1_H__ */
#if defined(__CKSUM_SHA256_H__)
    sha256_s sha256[1];
#endif /* __CKSUM_SHA256_H__ */
#if defined(__CKSUM_SHA512_H__)
    sha512_s sha512[1];
#endif /* __CKSUM_SHA512_H__ */
#if defined(__CKSUM_SHA3_H__)
    sha3_s sha3[1];
#endif /* __CKSUM_SHA3_H__ */
#if defined(__CKSUM_BLAKE2S_H__)
    blake2s_s blake2s[1];
#endif /* __CKSUM_BLAKE2S_H__ */
#if defined(__CKSUM_BLAKE2B_H__)
    blake2b_s blake2b[1];
#endif /* __CKSUM_BLAKE2B_H__ */
} hash_u;

typedef struct hash_s
{
    unsigned int bufsiz; /*!< size of block */
    unsigned int outsiz; /*!< size of digest */
    /*!
     @brief Initialize function for hash.
     @param[in,out] ctx points to an instance of hash state.
    */
    void (*init)(hash_u *ctx);
    /*!
     @brief Process function for hash.
     @param[in,out] ctx points to an instance of hash state.
     @param[in] pdata points to data to hash.
     @param[in] nbyte length of data to hash.
     @return the execution state of the function.
      @retval 0 success
    */
    int (*proc)(hash_u *ctx, const void *pdata, size_t nbyte);
    /*!
     @brief Terminate function for hash.
     @param[in,out] ctx points to an instance of hash state.
     @param[in,out] out points to buffer that holds the digest.
     @return the digest internal buffer.
      @retval 0 generic invalid argument.
    */
    unsigned char *(*done)(hash_u *ctx, void *out);
} hash_s;

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

#if defined(__CKSUM_MD5_H__)
extern const hash_s hash_md5;
#endif /* __CKSUM_MD5_H__ */
#if defined(__CKSUM_SHA1_H__)
extern const hash_s hash_sha1;
#endif /* __CKSUM_SHA1_H__ */
#if defined(__CKSUM_SHA256_H__)
extern const hash_s hash_sha224;
extern const hash_s hash_sha256;
#endif /* __CKSUM_SHA256_H__ */
#if defined(__CKSUM_SHA512_H__)
extern const hash_s hash_sha384;
extern const hash_s hash_sha512;
extern const hash_s hash_sha512_224;
extern const hash_s hash_sha512_256;
#endif /* __CKSUM_SHA512_H__ */
#if defined(__CKSUM_SHA3_H__)
extern const hash_s hash_sha3_224;
extern const hash_s hash_sha3_256;
extern const hash_s hash_sha3_384;
extern const hash_s hash_sha3_512;
extern const hash_s hash_shake128;
extern const hash_s hash_shake256;
extern const hash_s hash_keccak224;
extern const hash_s hash_keccak256;
extern const hash_s hash_keccak384;
extern const hash_s hash_keccak512;
#endif /* __CKSUM_SHA3_H__ */
#if defined(__CKSUM_BLAKE2S_H__)
extern const hash_s hash_blake2s_128;
extern const hash_s hash_blake2s_160;
extern const hash_s hash_blake2s_224;
extern const hash_s hash_blake2s_256;
#endif /* __CKSUM_BLAKE2S_H__ */
#if defined(__CKSUM_BLAKE2B_H__)
extern const hash_s hash_blake2b_160;
extern const hash_s hash_blake2b_256;
extern const hash_s hash_blake2b_384;
extern const hash_s hash_blake2b_512;
#endif /* __CKSUM_BLAKE2B_H__ */

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* __CKSUM_HASH_H__ */
