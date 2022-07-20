/*!
 @file hash.c
 @brief hash library
 @copyright Copyright (C) 2020 tqfx, All rights reserved.
*/

#include "cksum/hash.h"

#include <assert.h>

#undef HASH_INIT
#define HASH_INIT(stat, init, func) \
    static inline void func(hash_u *ctx) { init(ctx->stat); }
#if defined(__CKSUM_MD5_H__)
HASH_INIT(md5, md5_init, hash_init_md5)
#endif /* __CKSUM_MD5_H__ */
#if defined(__CKSUM_SHA1_H__)
HASH_INIT(sha1, sha1_init, hash_init_sha1)
#endif /* __CKSUM_SHA1_H__ */
#if defined(__CKSUM_SHA256_H__)
HASH_INIT(sha256, sha256_init, hash_init_sha256)
HASH_INIT(sha256, sha224_init, hash_init_sha224)
#endif /* __CKSUM_SHA256_H__ */
#if defined(__CKSUM_SHA512_H__)
HASH_INIT(sha512, sha512_init, hash_init_sha512)
HASH_INIT(sha512, sha384_init, hash_init_sha384)
HASH_INIT(sha512, sha512_224_init, hash_init_sha512_224)
HASH_INIT(sha512, sha512_256_init, hash_init_sha512_256)
#endif /* __CKSUM_SHA512_H__ */
#if defined(__CKSUM_SHA3_H__)
HASH_INIT(sha3, sha3_224_init, hash_init_sha3_224)
HASH_INIT(sha3, sha3_256_init, hash_init_sha3_256)
HASH_INIT(sha3, sha3_384_init, hash_init_sha3_384)
HASH_INIT(sha3, sha3_512_init, hash_init_sha3_512)
HASH_INIT(sha3, shake128_init, hash_init_shake128)
HASH_INIT(sha3, shake256_init, hash_init_shake256)
HASH_INIT(sha3, sha3_224_init, hash_init_keccak224)
HASH_INIT(sha3, sha3_256_init, hash_init_keccak256)
HASH_INIT(sha3, sha3_384_init, hash_init_keccak384)
HASH_INIT(sha3, sha3_512_init, hash_init_keccak512)
#endif /* __CKSUM_SHA3_H__ */
#if defined(__CKSUM_BLAKE2S_H__)
HASH_INIT(blake2s, blake2s_128_init, hash_init_blake2s_128)
HASH_INIT(blake2s, blake2s_160_init, hash_init_blake2s_160)
HASH_INIT(blake2s, blake2s_224_init, hash_init_blake2s_224)
HASH_INIT(blake2s, blake2s_256_init, hash_init_blake2s_256)
#endif /* __CKSUM_BLAKE2S_H__ */
#if defined(__CKSUM_BLAKE2B_H__)
HASH_INIT(blake2b, blake2b_160_init, hash_init_blake2b_160)
HASH_INIT(blake2b, blake2b_256_init, hash_init_blake2b_256)
HASH_INIT(blake2b, blake2b_384_init, hash_init_blake2b_384)
HASH_INIT(blake2b, blake2b_512_init, hash_init_blake2b_512)
#endif /* __CKSUM_BLAKE2B_H__ */
#undef HASH_INIT

#undef HASH_PROC
#define HASH_PROC(stat, proc, func) \
    static inline int func(hash_u *ctx, const void *pdata, size_t nbyte) { return proc(ctx->stat, pdata, nbyte); }
#if defined(__CKSUM_MD5_H__)
HASH_PROC(md5, md5_proc, hash_proc_md5)
#endif /* __CKSUM_MD5_H__ */
#if defined(__CKSUM_SHA1_H__)
HASH_PROC(sha1, sha1_proc, hash_proc_sha1)
#endif /* __CKSUM_SHA1_H__ */
#if defined(__CKSUM_SHA256_H__)
HASH_PROC(sha256, sha256_proc, hash_proc_sha256)
HASH_PROC(sha256, sha256_proc, hash_proc_sha224)
#endif /* __CKSUM_SHA256_H__ */
#if defined(__CKSUM_SHA512_H__)
HASH_PROC(sha512, sha512_proc, hash_proc_sha512)
HASH_PROC(sha512, sha512_proc, hash_proc_sha384)
HASH_PROC(sha512, sha512_proc, hash_proc_sha512_224)
HASH_PROC(sha512, sha512_proc, hash_proc_sha512_256)
#endif /* __CKSUM_SHA512_H__ */
#if defined(__CKSUM_SHA3_H__)
HASH_PROC(sha3, sha3_proc, hash_proc_sha3_224)
HASH_PROC(sha3, sha3_proc, hash_proc_sha3_256)
HASH_PROC(sha3, sha3_proc, hash_proc_sha3_384)
HASH_PROC(sha3, sha3_proc, hash_proc_sha3_512)
HASH_PROC(sha3, sha3_proc, hash_proc_shake128)
HASH_PROC(sha3, sha3_proc, hash_proc_shake256)
HASH_PROC(sha3, sha3_proc, hash_proc_keccak224)
HASH_PROC(sha3, sha3_proc, hash_proc_keccak256)
HASH_PROC(sha3, sha3_proc, hash_proc_keccak384)
HASH_PROC(sha3, sha3_proc, hash_proc_keccak512)
#endif /* __CKSUM_SHA3_H__ */
#if defined(__CKSUM_BLAKE2S_H__)
HASH_PROC(blake2s, blake2s_proc, hash_proc_blake2s_128)
HASH_PROC(blake2s, blake2s_proc, hash_proc_blake2s_160)
HASH_PROC(blake2s, blake2s_proc, hash_proc_blake2s_224)
HASH_PROC(blake2s, blake2s_proc, hash_proc_blake2s_256)
#endif /* __CKSUM_BLAKE2S_H__ */
#if defined(__CKSUM_BLAKE2B_H__)
HASH_PROC(blake2b, blake2b_proc, hash_proc_blake2b_160)
HASH_PROC(blake2b, blake2b_proc, hash_proc_blake2b_256)
HASH_PROC(blake2b, blake2b_proc, hash_proc_blake2b_384)
HASH_PROC(blake2b, blake2b_proc, hash_proc_blake2b_512)
#endif /* __CKSUM_BLAKE2B_H__ */
#undef HASH_PROC

#undef HASH_DONE
#define HASH_DONE(stat, done, func) \
    static inline unsigned char *func(hash_u *ctx, void *out) { return done(ctx->stat, out); }
#if defined(__CKSUM_MD5_H__)
HASH_DONE(md5, md5_done, hash_done_md5)
#endif /* __CKSUM_MD5_H__ */
#if defined(__CKSUM_SHA1_H__)
HASH_DONE(sha1, sha1_done, hash_done_sha1)
#endif /* __CKSUM_SHA1_H__ */
#if defined(__CKSUM_SHA256_H__)
HASH_DONE(sha256, sha256_done, hash_done_sha256)
HASH_DONE(sha256, sha256_done, hash_done_sha224)
#endif /* __CKSUM_SHA256_H__ */
#if defined(__CKSUM_SHA512_H__)
HASH_DONE(sha512, sha512_done, hash_done_sha512)
HASH_DONE(sha512, sha512_done, hash_done_sha384)
HASH_DONE(sha512, sha512_done, hash_done_sha512_224)
HASH_DONE(sha512, sha512_done, hash_done_sha512_256)
#endif /* __CKSUM_SHA512_H__ */
#if defined(__CKSUM_SHA3_H__)
HASH_DONE(sha3, sha3_done, hash_done_sha3_224)
HASH_DONE(sha3, sha3_done, hash_done_sha3_256)
HASH_DONE(sha3, sha3_done, hash_done_sha3_384)
HASH_DONE(sha3, sha3_done, hash_done_sha3_512)
HASH_DONE(sha3, shake128_done, hash_done_shake128)
HASH_DONE(sha3, shake256_done, hash_done_shake256)
HASH_DONE(sha3, keccak_done, hash_done_keccak224)
HASH_DONE(sha3, keccak_done, hash_done_keccak256)
HASH_DONE(sha3, keccak_done, hash_done_keccak384)
HASH_DONE(sha3, keccak_done, hash_done_keccak512)
#endif /* __CKSUM_SHA3_H__ */
#if defined(__CKSUM_BLAKE2S_H__)
HASH_DONE(blake2s, blake2s_done, hash_done_blake2s_128)
HASH_DONE(blake2s, blake2s_done, hash_done_blake2s_160)
HASH_DONE(blake2s, blake2s_done, hash_done_blake2s_224)
HASH_DONE(blake2s, blake2s_done, hash_done_blake2s_256)
#endif /* __CKSUM_BLAKE2S_H__ */
#if defined(__CKSUM_BLAKE2B_H__)
HASH_DONE(blake2b, blake2b_done, hash_done_blake2b_160)
HASH_DONE(blake2b, blake2b_done, hash_done_blake2b_256)
HASH_DONE(blake2b, blake2b_done, hash_done_blake2b_384)
HASH_DONE(blake2b, blake2b_done, hash_done_blake2b_512)
#endif /* __CKSUM_BLAKE2B_H__ */
#undef HASH_DONE

#if defined(__CKSUM_MD5_H__)
const hash_s hash_md5 = {
    .bufsiz = MD5_BUFSIZ,
    .outsiz = MD5_OUTSIZ,
    .init = hash_init_md5,
    .proc = hash_proc_md5,
    .done = hash_done_md5,
};
#endif /* __CKSUM_MD5_H__ */
#if defined(__CKSUM_SHA1_H__)
const hash_s hash_sha1 = {
    .bufsiz = SHA1_BUFSIZ,
    .outsiz = SHA1_OUTSIZ,
    .init = hash_init_sha1,
    .proc = hash_proc_sha1,
    .done = hash_done_sha1,
};
#endif /* __CKSUM_SHA1_H__ */
#if defined(__CKSUM_SHA256_H__)
const hash_s hash_sha224 = {
    .bufsiz = SHA256_BUFSIZ,
    .outsiz = SHA224_OUTSIZ,
    .init = hash_init_sha224,
    .proc = hash_proc_sha224,
    .done = hash_done_sha224,
};
const hash_s hash_sha256 = {
    .bufsiz = SHA256_BUFSIZ,
    .outsiz = SHA256_OUTSIZ,
    .init = hash_init_sha256,
    .proc = hash_proc_sha256,
    .done = hash_done_sha256,
};
#endif /* __CKSUM_SHA256_H__ */
#if defined(__CKSUM_SHA512_H__)
const hash_s hash_sha384 = {
    .bufsiz = SHA512_BUFSIZ,
    .outsiz = SHA384_OUTSIZ,
    .init = hash_init_sha384,
    .proc = hash_proc_sha384,
    .done = hash_done_sha384,
};
const hash_s hash_sha512 = {
    .bufsiz = SHA512_BUFSIZ,
    .outsiz = SHA512_OUTSIZ,
    .init = hash_init_sha512,
    .proc = hash_proc_sha512,
    .done = hash_done_sha512,
};
const hash_s hash_sha512_224 = {
    .bufsiz = SHA512_BUFSIZ,
    .outsiz = SHA512_224_OUTSIZ,
    .init = hash_init_sha512_224,
    .proc = hash_proc_sha512_224,
    .done = hash_done_sha512_224,
};
const hash_s hash_sha512_256 = {
    .bufsiz = SHA512_BUFSIZ,
    .outsiz = SHA512_256_OUTSIZ,
    .init = hash_init_sha512_256,
    .proc = hash_proc_sha512_256,
    .done = hash_done_sha512_256,
};
#endif /* __CKSUM_SHA512_H__ */
#if defined(__CKSUM_SHA3_H__)
const hash_s hash_sha3_224 = {
    .bufsiz = SHA3_224_BUFSIZ,
    .outsiz = SHA3_224_OUTSIZ,
    .init = hash_init_sha3_224,
    .proc = hash_proc_sha3_224,
    .done = hash_done_sha3_224,
};
const hash_s hash_sha3_256 = {
    .bufsiz = SHA3_256_BUFSIZ,
    .outsiz = SHA3_256_OUTSIZ,
    .init = hash_init_sha3_256,
    .proc = hash_proc_sha3_256,
    .done = hash_done_sha3_256,
};
const hash_s hash_sha3_384 = {
    .bufsiz = SHA3_384_BUFSIZ,
    .outsiz = SHA3_384_OUTSIZ,
    .init = hash_init_sha3_384,
    .proc = hash_proc_sha3_384,
    .done = hash_done_sha3_384,
};
const hash_s hash_sha3_512 = {
    .bufsiz = SHA3_512_BUFSIZ,
    .outsiz = SHA3_512_OUTSIZ,
    .init = hash_init_sha3_512,
    .proc = hash_proc_sha3_512,
    .done = hash_done_sha3_512,
};
const hash_s hash_shake128 = {
    .bufsiz = SHAKE128_BUFSIZ,
    .outsiz = SHAKE128_OUTSIZ,
    .init = hash_init_shake128,
    .proc = hash_proc_shake128,
    .done = hash_done_shake128,
};
const hash_s hash_shake256 = {
    .bufsiz = SHAKE256_BUFSIZ,
    .outsiz = SHAKE256_OUTSIZ,
    .init = hash_init_shake256,
    .proc = hash_proc_shake256,
    .done = hash_done_shake256,
};
const hash_s hash_keccak224 = {
    .bufsiz = KECCAK224_BUFSIZ,
    .outsiz = KECCAK224_OUTSIZ,
    .init = hash_init_keccak224,
    .proc = hash_proc_keccak224,
    .done = hash_done_keccak224,
};
const hash_s hash_keccak256 = {
    .bufsiz = KECCAK256_BUFSIZ,
    .outsiz = KECCAK256_OUTSIZ,
    .init = hash_init_keccak256,
    .proc = hash_proc_keccak256,
    .done = hash_done_keccak256,
};
const hash_s hash_keccak384 = {
    .bufsiz = KECCAK384_BUFSIZ,
    .outsiz = KECCAK384_OUTSIZ,
    .init = hash_init_keccak384,
    .proc = hash_proc_keccak384,
    .done = hash_done_keccak384,
};
const hash_s hash_keccak512 = {
    .bufsiz = KECCAK512_BUFSIZ,
    .outsiz = KECCAK512_OUTSIZ,
    .init = hash_init_keccak512,
    .proc = hash_proc_keccak512,
    .done = hash_done_keccak512,
};
#endif /* __CKSUM_SHA3_H__ */
#if defined(__CKSUM_BLAKE2S_H__)
const hash_s hash_blake2s_128 = {
    .bufsiz = BLAKE2S_BUFSIZ,
    .outsiz = BLAKE2S_128_OUTSIZ,
    .init = hash_init_blake2s_128,
    .proc = hash_proc_blake2s_128,
    .done = hash_done_blake2s_128,
};
const hash_s hash_blake2s_160 = {
    .bufsiz = BLAKE2S_BUFSIZ,
    .outsiz = BLAKE2S_160_OUTSIZ,
    .init = hash_init_blake2s_160,
    .proc = hash_proc_blake2s_160,
    .done = hash_done_blake2s_160,
};
const hash_s hash_blake2s_224 = {
    .bufsiz = BLAKE2S_BUFSIZ,
    .outsiz = BLAKE2S_224_OUTSIZ,
    .init = hash_init_blake2s_224,
    .proc = hash_proc_blake2s_224,
    .done = hash_done_blake2s_224,
};
const hash_s hash_blake2s_256 = {
    .bufsiz = BLAKE2S_BUFSIZ,
    .outsiz = BLAKE2S_256_OUTSIZ,
    .init = hash_init_blake2s_256,
    .proc = hash_proc_blake2s_256,
    .done = hash_done_blake2s_256,
};
#endif /* __CKSUM_BLAKE2S_H__ */
#if defined(__CKSUM_BLAKE2B_H__)
const hash_s hash_blake2b_160 = {
    .bufsiz = BLAKE2B_BUFSIZ,
    .outsiz = BLAKE2B_160_OUTSIZ,
    .init = hash_init_blake2b_160,
    .proc = hash_proc_blake2b_160,
    .done = hash_done_blake2b_160,
};
const hash_s hash_blake2b_256 = {
    .bufsiz = BLAKE2B_BUFSIZ,
    .outsiz = BLAKE2B_256_OUTSIZ,
    .init = hash_init_blake2b_256,
    .proc = hash_proc_blake2b_256,
    .done = hash_done_blake2b_256,
};
const hash_s hash_blake2b_384 = {
    .bufsiz = BLAKE2B_BUFSIZ,
    .outsiz = BLAKE2B_384_OUTSIZ,
    .init = hash_init_blake2b_384,
    .proc = hash_proc_blake2b_384,
    .done = hash_done_blake2b_384,
};
const hash_s hash_blake2b_512 = {
    .bufsiz = BLAKE2B_BUFSIZ,
    .outsiz = BLAKE2B_512_OUTSIZ,
    .init = hash_init_blake2b_512,
    .proc = hash_proc_blake2b_512,
    .done = hash_done_blake2b_512,
};
#endif /* __CKSUM_BLAKE2B_H__ */
