/*!
 @file hmac.c
 @brief Testing hmac library
 @copyright Copyright (C) 2020 tqfx, All rights reserved.
*/

#include "cksum/hmac.h"

#include "hash.h"

static const char *key = "12345678901234567890123456789012345678901234567890123456789012345678901234567890";
static const char *msg = "text";

static void test_hmac_md5(void)
{
    static const unsigned char hash[] = {
        /* clang-format off */
        0x85, 0x8D, 0x61, 0xC1, 0xA6, 0x35, 0x37, 0x68,
        0x2B, 0xD9, 0x75, 0x9C, 0xF3, 0x8A, 0xDB, 0xCB,
        /* clang-format on */
    };

    hmac_s ctx[1];

    hmac_init(ctx, &hash_md5, key, strlen(key));
    hmac_proc(ctx, msg, strlen(msg));
    hmac_done(ctx, ctx->buf);

    HASH_DIFF(ctx->buf, hash, sizeof(hash), "hmac_md5");
}

static void test_hmac_sha1(void)
{
    static const unsigned char hash[] = {
        /* clang-format off */
        0x85, 0x94, 0x17, 0x55, 0x56, 0xE5, 0x30, 0x95,
        0x7B, 0xA8, 0x13, 0x0B, 0x11, 0x33, 0xE4, 0x33,
        0x5C, 0x67, 0x55, 0x91,
        /* clang-format on */

    };

    hmac_s ctx[1];

    hmac_init(ctx, &hash_sha1, key, strlen(key));
    hmac_proc(ctx, msg, strlen(msg));
    hmac_done(ctx, ctx->buf);

    HASH_DIFF(ctx->buf, hash, sizeof(hash), "hmac_sha1");
}

static void test_hmac_sha224(void)
{
    static const unsigned char hash[] = {
        /* clang-format off */
        0x80, 0x4B, 0x16, 0x26, 0x21, 0xC8, 0x5C, 0x36,
        0xBC, 0xE8, 0xD9, 0x8F, 0xF2, 0x53, 0x4C, 0xC8,
        0xDA, 0x77, 0x1C, 0xC0, 0x29, 0x56, 0xD5, 0xF7,
        0x9F, 0xDD, 0xE0, 0xDD,
        /* clang-format on */
    };

    hmac_s ctx[1];

    hmac_init(ctx, &hash_sha224, key, strlen(key));
    hmac_proc(ctx, msg, strlen(msg));
    hmac_done(ctx, ctx->buf);

    HASH_DIFF(ctx->buf, hash, sizeof(hash), "hmac_sha224");
}

static void test_hmac_sha256(void)
{
    static const unsigned char hash[] = {
        /* clang-format off */
        0x46, 0x92, 0xAE, 0x8C, 0x8B, 0x03, 0x09, 0xC3,
        0xDF, 0xA8, 0xF4, 0xE5, 0x3B, 0x06, 0xFF, 0x12,
        0xA8, 0xF7, 0x91, 0x6B, 0x64, 0xA0, 0x6E, 0x53,
        0x00, 0xD0, 0x8B, 0xC4, 0xCB, 0xA5, 0xD7, 0x5A,
        /* clang-format on */
    };

    hmac_s ctx[1];

    hmac_init(ctx, &hash_sha256, key, strlen(key));
    hmac_proc(ctx, msg, strlen(msg));
    hmac_done(ctx, ctx->buf);

    HASH_DIFF(ctx->buf, hash, sizeof(hash), "hmac_sha256");
}

static void test_hmac_sha384(void)
{
    static const unsigned char hash[] = {
        /* clang-format off */
        0xD6, 0x5A, 0xB8, 0x2E, 0x22, 0x2F, 0xAE, 0x95,
        0x57, 0xA4, 0x9D, 0x5D, 0xF2, 0x94, 0x29, 0x12,
        0x6E, 0xB8, 0x02, 0xB1, 0x7D, 0xB1, 0x9D, 0x6A,
        0xE1, 0x55, 0x9F, 0x75, 0x3A, 0x37, 0x56, 0x49,
        0xC2, 0xAF, 0x8B, 0x0D, 0xEC, 0x5B, 0xD8, 0x7D,
        0x2D, 0x75, 0x4D, 0x64, 0xCD, 0xB0, 0x93, 0x1C,
        /* clang-format on */
    };

    hmac_s ctx[1];

    hmac_init(ctx, &hash_sha384, key, strlen(key));
    hmac_proc(ctx, msg, strlen(msg));
    hmac_done(ctx, ctx->buf);

    HASH_DIFF(ctx->buf, hash, sizeof(hash), "hmac_sha384");
}

static void test_hmac_sha512(void)
{
    static const unsigned char hash[] = {
        /* clang-format off */
        0x21, 0x72, 0x72, 0xCD, 0x6D, 0x37, 0xEA, 0x33,
        0x59, 0x91, 0xE0, 0xB7, 0x29, 0x57, 0xD3, 0x23,
        0x19, 0x2F, 0x29, 0x01, 0x0E, 0x19, 0x0C, 0x25,
        0xF4, 0xBA, 0xA6, 0xC6, 0xC0, 0xBE, 0xB3, 0x15,
        0xD6, 0x4D, 0xE4, 0xDE, 0x74, 0xE3, 0xDC, 0x58,
        0x5F, 0x44, 0x9A, 0x92, 0xE1, 0x47, 0xBA, 0xB8,
        0x75, 0x80, 0xEA, 0x9A, 0x9E, 0x62, 0x69, 0xD4,
        0x23, 0xCB, 0xCD, 0xDD, 0xE8, 0x82, 0xD1, 0xAD,
        /* clang-format on */
    };

    hmac_s ctx[1];

    hmac_init(ctx, &hash_sha512, key, strlen(key));
    hmac_proc(ctx, msg, strlen(msg));
    hmac_done(ctx, ctx->buf);

    HASH_DIFF(ctx->buf, hash, sizeof(hash), "hmac_sha512");
}

static void test_hmac_sha3_224(void)
{
    static const unsigned char hash[] = {
        /* clang-format off */
        0x0B, 0x80, 0x31, 0x3D, 0x5A, 0x14, 0x84, 0x79,
        0x67, 0x47, 0xC8, 0xFD, 0x2C, 0x15, 0xB2, 0xE1,
        0x56, 0xF7, 0x42, 0x9C, 0x46, 0x10, 0x12, 0xBD,
        0xD4, 0x06, 0x64, 0xA0,
        /* clang-format on */
    };

    hmac_s ctx[1];

    hmac_init(ctx, &hash_sha3_224, key, strlen(key));
    hmac_proc(ctx, msg, strlen(msg));
    hmac_done(ctx, ctx->buf);

    HASH_DIFF(ctx->buf, hash, sizeof(hash), "hmac_sha3_224");
}

static void test_hmac_sha3_256(void)
{
    static const unsigned char hash[] = {
        /* clang-format off */
        0xBD, 0x38, 0xD3, 0x98, 0x23, 0x16, 0x7B, 0x90,
        0xDD, 0x11, 0xF6, 0x59, 0xBA, 0x80, 0xD9, 0xFD,
        0xEB, 0x0B, 0xE4, 0x02, 0x69, 0xFD, 0xFC, 0x68,
        0x2E, 0xE1, 0x50, 0x0B, 0x86, 0xDE, 0x89, 0xF6,
        /* clang-format on */
    };

    hmac_s ctx[1];

    hmac_init(ctx, &hash_sha3_256, key, strlen(key));
    hmac_proc(ctx, msg, strlen(msg));
    hmac_done(ctx, ctx->buf);

    HASH_DIFF(ctx->buf, hash, sizeof(hash), "hmac_sha3_256");
}

static void test_hmac_sha3_384(void)
{
    static const unsigned char hash[] = {
        /* clang-format off */
        0x7F, 0x45, 0xE0, 0x3F, 0x9F, 0x52, 0x1F, 0xFE,
        0xFF, 0x9E, 0x92, 0x5A, 0x3A, 0x13, 0x2F, 0xD1,
        0xF8, 0x2B, 0x52, 0xF1, 0x56, 0x4A, 0x3C, 0x81,
        0x63, 0x80, 0x53, 0xCA, 0x91, 0xFB, 0x1B, 0xEF,
        0x76, 0x17, 0xBC, 0x2B, 0xED, 0xE1, 0x43, 0xD6,
        0xDF, 0x30, 0x0C, 0x6A, 0x1E, 0x91, 0x97, 0x7D,
        /* clang-format on */
    };

    hmac_s ctx[1];

    hmac_init(ctx, &hash_sha3_384, key, strlen(key));
    hmac_proc(ctx, msg, strlen(msg));
    hmac_done(ctx, ctx->buf);

    HASH_DIFF(ctx->buf, hash, sizeof(hash), "hmac_sha3_384");
}

static void test_hmac_sha3_512(void)
{
    static const unsigned char hash[] = {
        /* clang-format off */
        0x78, 0xE0, 0xDF, 0xD3, 0x12, 0x9A, 0x9A, 0xA6,
        0x38, 0xA3, 0x1D, 0x93, 0xBA, 0x17, 0x13, 0x6A,
        0x35, 0xEA, 0xB2, 0xD9, 0xB0, 0x5D, 0xDA, 0xB6,
        0xE2, 0x39, 0x16, 0x1B, 0xEB, 0x18, 0x2D, 0xB9,
        0xBC, 0x41, 0x3C, 0x3D, 0xFF, 0xFA, 0xEA, 0xDC,
        0x2F, 0x4F, 0xFD, 0xB4, 0x77, 0x87, 0x26, 0x84,
        0x0D, 0xB5, 0xA3, 0x42, 0xA4, 0xCD, 0xA3, 0x85,
        0x1E, 0x89, 0x25, 0x7B, 0xFF, 0x0A, 0x98, 0x24,
        /* clang-format on */
    };

    hmac_s ctx[1];

    hmac_init(ctx, &hash_sha3_512, key, strlen(key));
    hmac_proc(ctx, msg, strlen(msg));
    hmac_done(ctx, ctx->buf);

    HASH_DIFF(ctx->buf, hash, sizeof(hash), "hmac_sha3_512");
}

static void test_hmac_blake2s(void)
{
    static const unsigned char hash[] = {
        /* clang-format off */
        0x4A, 0x8D, 0x4C, 0x7E, 0x65, 0x35, 0x7B, 0x06,
        0xCB, 0x83, 0x70, 0xC5, 0x47, 0x13, 0xC6, 0x04,
        0x2D, 0xBC, 0x6E, 0xB2, 0x61, 0x46, 0x3D, 0xF3,
        0x32, 0xB6, 0xE6, 0x3C, 0x3C, 0xD0, 0xCA, 0x39,
        /* clang-format on */
    };

    hmac_s ctx[1];

    hmac_init(ctx, &hash_blake2s_256, key, strlen(key));
    hmac_proc(ctx, msg, strlen(msg));
    hmac_done(ctx, ctx->buf);

    HASH_DIFF(ctx->buf, hash, sizeof(hash), "hmac_blake2s_256");
}

static void test_hmac_blake2b(void)
{
    static const unsigned char hash[] = {
        /* clang-format off */
        0x71, 0xF0, 0x29, 0x70, 0x0C, 0x7E, 0x44, 0x3B,
        0xA2, 0x22, 0x1F, 0x05, 0x73, 0x71, 0x80, 0x63,
        0x3F, 0x95, 0xB8, 0xA7, 0x0F, 0x3E, 0x51, 0xA0,
        0x86, 0x28, 0xBD, 0x26, 0x94, 0x7B, 0xE1, 0x69,
        0x89, 0xD1, 0x2E, 0xD2, 0x64, 0x67, 0xCF, 0x37,
        0xAF, 0x7D, 0x0F, 0x36, 0x64, 0x9F, 0x91, 0x0A,
        0x9A, 0x73, 0xB9, 0x40, 0x85, 0x4A, 0x65, 0x02,
        0x4A, 0x0B, 0xCC, 0xE7, 0x45, 0x4B, 0xCC, 0x65,
        /* clang-format on */
    };

    hmac_s ctx[1];

    hmac_init(ctx, &hash_blake2b_512, key, strlen(key));
    hmac_proc(ctx, msg, strlen(msg));
    hmac_done(ctx, ctx->buf);

    HASH_DIFF(ctx->buf, hash, sizeof(hash), "hmac_blake2b_512");
}

int main(void)
{
    test_hmac_md5();

    test_hmac_sha1();

    test_hmac_sha224();
    test_hmac_sha256();
    test_hmac_sha384();
    test_hmac_sha512();

    test_hmac_sha3_224();
    test_hmac_sha3_256();
    test_hmac_sha3_384();
    test_hmac_sha3_512();

    test_hmac_blake2s();
    test_hmac_blake2b();

    return 0;
}