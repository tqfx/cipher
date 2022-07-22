/*!
 @file hash.c
 @brief Testing hash library
 @copyright Copyright (C) 2020-present tqfx, All rights reserved.
*/

#include "cksum/hash.h"

#include "hash.h"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
#endif /* __GNUC__ || __clang__ */

static void test_md5(void)
{
    static const struct
    {
        const char *msg;
        unsigned char hash[MD5_OUTSIZ];
    } tests[] = {
        /* clang-format off */
        {
            "",
            {
                0xD4, 0x1D, 0x8C, 0xD9, 0x8F, 0x00, 0xB2, 0x04,
                0xE9, 0x80, 0x09, 0x98, 0xEC, 0xF8, 0x42, 0x7E,
            },
        },
        {
            "a",
            {
                0x0C, 0xC1, 0x75, 0xB9, 0xC0, 0xF1, 0xB6, 0xA8,
                0x31, 0xC3, 0x99, 0xE2, 0x69, 0x77, 0x26, 0x61,

            },
        },
        {
            "abc",
            {
                0x90, 0x01, 0x50, 0x98, 0x3C, 0xD2, 0x4F, 0xB0,
                0xD6, 0x96, 0x3F, 0x7D, 0x28, 0xE1, 0x7F, 0x72,
            },
        },
        {
            "message digest",
            {
                0xF9, 0x6B, 0x69, 0x7D, 0x7C, 0xB7, 0x93, 0x8D,
                0x52, 0x5A, 0x2F, 0x31, 0xAA, 0xF1, 0x61, 0xD0,
            },
        },
        {
            "abcdefghijklmnopqrstuvwxyz",
            {
                0xC3, 0xFC, 0xD3, 0xD7, 0x61, 0x92, 0xE4, 0x00,
                0x7D, 0xFB, 0x49, 0x6C, 0xCA, 0x67, 0xE1, 0x3B,
            },
        },
        {
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
            {
                0xD1, 0x74, 0xAB, 0x98, 0xD2, 0x77, 0xD9, 0xF5,
                0xA5, 0x61, 0x1C, 0x2C, 0x9F, 0x41, 0x9D, 0x9F,
            },
        },
        {
            "1234567890123456789012345678901234567890"
            "1234567890123456789012345678901234567890",
            {
                0x57, 0xED, 0xF4, 0xA2, 0x2B, 0xE3, 0xC9, 0x55,
                0xAC, 0x49, 0xDA, 0x2E, 0x21, 0x07, 0xB6, 0x7A,
            },
        },
        /* clang-format on */
    };

    md5_s ctx[1];

    for (unsigned int i = 0; i != sizeof(tests) / sizeof(*tests); ++i)
    {
        md5_init(ctx);
        md5_proc(ctx, tests[i].msg, strlen(tests[i].msg));
        md5_done(ctx, ctx->out);
        HASH_DIFF(ctx->out, tests[i].hash, MD5_OUTSIZ, "md5");
    }
}

static void test_sha1(void)
{
    static const struct
    {
        const char *msg;
        unsigned char hash[SHA1_OUTSIZ];
    } tests[] = {
        /* clang-format off */
        {
            "abc",
            {
                0xA9, 0x99, 0x3E, 0x36, 0x47, 0x06, 0x81, 0x6A, 0xBA, 0x3E,
                0x25, 0x71, 0x78, 0x50, 0xC2, 0x6C, 0x9C, 0xD0, 0xD8, 0x9D,
            },
        },
        {
            "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
            {
                0x84, 0x98, 0x3E, 0x44, 0x1C, 0x3B, 0xD2, 0x6E, 0xBA, 0xAE,
                0x4A, 0xA1, 0xF9, 0x51, 0x29, 0xE5, 0xE5, 0x46, 0x70, 0xF1,
            },
        },
        /* clang-format on */
    };

    sha1_s ctx[1];

    for (unsigned int i = 0; i != sizeof(tests) / sizeof(*tests); ++i)
    {
        sha1_init(ctx);
        sha1_proc(ctx, tests[i].msg, strlen(tests[i].msg));
        sha1_done(ctx, ctx->out);
        HASH_DIFF(ctx->out, tests[i].hash, SHA1_OUTSIZ, "sha1");
    }
}

static void test_sha256(void)
{
    static const struct
    {
        const char *msg;
        unsigned char hash[SHA256_OUTSIZ];
    } tests[] = {
        /* clang-format off */
        {
            "abc",
            {
                0xBA, 0x78, 0x16, 0xBF, 0x8F, 0x01, 0xCF, 0xEA,
                0x41, 0x41, 0x40, 0xDE, 0x5D, 0xAE, 0x22, 0x23,
                0xB0, 0x03, 0x61, 0xA3, 0x96, 0x17, 0x7A, 0x9C,
                0xB4, 0x10, 0xFF, 0x61, 0xF2, 0x00, 0x15, 0xAD,
            },
        },
        {
            "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
            {
                0x24, 0x8D, 0x6A, 0x61, 0xD2, 0x06, 0x38, 0xB8,
                0xE5, 0xC0, 0x26, 0x93, 0x0C, 0x3E, 0x60, 0x39,
                0xA3, 0x3C, 0xE4, 0x59, 0x64, 0xFF, 0x21, 0x67,
                0xF6, 0xEC, 0xED, 0xD4, 0x19, 0xDB, 0x06, 0xC1,
            },
        },
        /* clang-format on */
    };

    sha256_s ctx[1];

    for (unsigned int i = 0; i != sizeof(tests) / sizeof(*tests); ++i)
    {
        sha256_init(ctx);
        sha256_proc(ctx, tests[i].msg, strlen(tests[i].msg));
        sha256_done(ctx, ctx->out);
        HASH_DIFF(ctx->out, tests[i].hash, SHA256_OUTSIZ, "sha256");
    }
}

static void test_sha224(void)
{
    static const struct
    {
        const char *msg;
        unsigned char hash[SHA224_OUTSIZ];
    } tests[] = {
        /* clang-format off */
        {
            "abc",
            {
                0x23, 0x09, 0x7D, 0x22, 0x34, 0x05, 0xD8, 0x22,
                0x86, 0x42, 0xA4, 0x77, 0xBD, 0xA2, 0x55, 0xB3,
                0x2A, 0xAD, 0xBC, 0xE4, 0xBD, 0xA0, 0xB3, 0xF7,
                0xE3, 0x6C, 0x9D, 0xA7,
            },
        },
        {
            "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
            {
                0x75, 0x38, 0x8B, 0x16, 0x51, 0x27, 0x76, 0xCC,
                0x5D, 0xBA, 0x5D, 0xA1, 0xFD, 0x89, 0x01, 0x50,
                0xB0, 0xC6, 0x45, 0x5C, 0xB4, 0xF5, 0x8B, 0x19,
                0x52, 0x52, 0x25, 0x25,
            },
        },
        /* clang-format on */
    };

    sha224_s ctx[1];

    for (unsigned int i = 0; i != sizeof(tests) / sizeof(*tests); ++i)
    {
        sha224_init(ctx);
        sha224_proc(ctx, tests[i].msg, strlen(tests[i].msg));
        sha224_done(ctx, ctx->out);
        HASH_DIFF(ctx->out, tests[i].hash, SHA224_OUTSIZ, "sha224");
    }
}

static void test_sha512(void)
{
    static const struct
    {
        const char *msg;
        unsigned char hash[SHA512_OUTSIZ];
    } tests[] = {
        /* clang-format off */
        {
            "abc",
            {
                0xDD, 0xAF, 0x35, 0xA1, 0x93, 0x61, 0x7A, 0xBA,
                0xCC, 0x41, 0x73, 0x49, 0xAE, 0x20, 0x41, 0x31,
                0x12, 0xE6, 0xFA, 0x4E, 0x89, 0xA9, 0x7E, 0xA2,
                0x0A, 0x9E, 0xEE, 0xE6, 0x4B, 0x55, 0xD3, 0x9A,
                0x21, 0x92, 0x99, 0x2A, 0x27, 0x4F, 0xC1, 0xA8,
                0x36, 0xBA, 0x3C, 0x23, 0xA3, 0xFE, 0xEB, 0xBD,
                0x45, 0x4D, 0x44, 0x23, 0x64, 0x3C, 0xE8, 0x0E,
                0x2A, 0x9A, 0xC9, 0x4F, 0xA5, 0x4C, 0xA4, 0x9F,
            },
        },
        {
            "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmn"
            "hijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu",
            {
                0x8E, 0x95, 0x9B, 0x75, 0xDA, 0xE3, 0x13, 0xDA,
                0x8C, 0xF4, 0xF7, 0x28, 0x14, 0xFC, 0x14, 0x3F,
                0x8F, 0x77, 0x79, 0xC6, 0xEB, 0x9F, 0x7F, 0xA1,
                0x72, 0x99, 0xAE, 0xAD, 0xB6, 0x88, 0x90, 0x18,
                0x50, 0x1D, 0x28, 0x9E, 0x49, 0x00, 0xF7, 0xE4,
                0x33, 0x1B, 0x99, 0xDE, 0xC4, 0xB5, 0x43, 0x3A,
                0xC7, 0xD3, 0x29, 0xEE, 0xB6, 0xDD, 0x26, 0x54,
                0x5E, 0x96, 0xE5, 0x5B, 0x87, 0x4B, 0xE9, 0x09,
            },
        },
        /* clang-format on */
    };

    sha512_s ctx[1];

    for (unsigned int i = 0; i != sizeof(tests) / sizeof(*tests); ++i)
    {
        sha512_init(ctx);
        sha512_proc(ctx, tests[i].msg, strlen(tests[i].msg));
        sha512_done(ctx, ctx->out);
        HASH_DIFF(ctx->out, tests[i].hash, SHA512_OUTSIZ, "sha512");
    }
}

static void test_sha384(void)
{
    static const struct
    {
        const char *msg;
        unsigned char hash[SHA384_OUTSIZ];
    } tests[] = {
        /* clang-format off */
        {
            "abc",
            {
                0xCB, 0x00, 0x75, 0x3F, 0x45, 0xA3, 0x5E, 0x8B,
                0xB5, 0xA0, 0x3D, 0x69, 0x9A, 0xC6, 0x50, 0x07,
                0x27, 0x2C, 0x32, 0xAB, 0x0E, 0xDE, 0xD1, 0x63,
                0x1A, 0x8B, 0x60, 0x5A, 0x43, 0xFF, 0x5B, 0xED,
                0x80, 0x86, 0x07, 0x2B, 0xA1, 0xE7, 0xCC, 0x23,
                0x58, 0xBA, 0xEC, 0xA1, 0x34, 0xC8, 0x25, 0xA7,
            },
        },
        {
            "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmn"
            "hijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu",
            {
                0x09, 0x33, 0x0C, 0x33, 0xF7, 0x11, 0x47, 0xE8,
                0x3D, 0x19, 0x2F, 0xC7, 0x82, 0xCD, 0x1B, 0x47,
                0x53, 0x11, 0x1B, 0x17, 0x3B, 0x3B, 0x05, 0xD2,
                0x2F, 0xA0, 0x80, 0x86, 0xE3, 0xB0, 0xF7, 0x12,
                0xFC, 0xC7, 0xC7, 0x1A, 0x55, 0x7E, 0x2D, 0xB9,
                0x66, 0xC3, 0xE9, 0xFA, 0x91, 0x74, 0x60, 0x39,
            },
        },
        /* clang-format on */
    };

    sha384_s ctx[1];

    for (unsigned int i = 0; i != sizeof(tests) / sizeof(*tests); ++i)
    {
        sha384_init(ctx);
        sha384_proc(ctx, tests[i].msg, strlen(tests[i].msg));
        sha384_done(ctx, ctx->out);
        HASH_DIFF(ctx->out, tests[i].hash, SHA384_OUTSIZ, "sha384");
    }
}

static void test_sha512_224(void)
{
    static const struct
    {
        const char *msg;
        unsigned char hash[SHA512_224_OUTSIZ];
    } tests[] = {
        /* clang-format off */
        {
            "abc",
            {
                0x46, 0x34, 0x27, 0x0F, 0x70, 0x7B, 0x6A, 0x54,
                0xDA, 0xAE, 0x75, 0x30, 0x46, 0x08, 0x42, 0xE2,
                0x0E, 0x37, 0xED, 0x26, 0x5C, 0xEE, 0xE9, 0xA4,
                0x3E, 0x89, 0x24, 0xAA,
            },
        },
        {
            "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmn"
            "hijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu",
            {
                0x23, 0xFE, 0xC5, 0xBB, 0x94, 0xD6, 0x0B, 0x23,
                0x30, 0x81, 0x92, 0x64, 0x0B, 0x0C, 0x45, 0x33,
                0x35, 0xD6, 0x64, 0x73, 0x4F, 0xE4, 0x0E, 0x72,
                0x68, 0x67, 0x4A, 0xF9,
            },
        },
        /* clang-format on */
    };

    sha512_224_s ctx[1];

    for (unsigned int i = 0; i != sizeof(tests) / sizeof(*tests); ++i)
    {
        sha512_224_init(ctx);
        sha512_224_proc(ctx, tests[i].msg, strlen(tests[i].msg));
        sha512_224_done(ctx, ctx->out);
        HASH_DIFF(ctx->out, tests[i].hash, SHA512_224_OUTSIZ, "sha512-224");
    }
}

static void test_sha512_256(void)
{
    static const struct
    {
        const char *msg;
        unsigned char hash[SHA512_256_OUTSIZ];
    } tests[] = {
        /* clang-format off */
        {
            "abc",
            {
                0x53, 0x04, 0x8E, 0x26, 0x81, 0x94, 0x1E, 0xF9,
                0x9B, 0x2E, 0x29, 0xB7, 0x6B, 0x4C, 0x7D, 0xAB,
                0xE4, 0xC2, 0xD0, 0xC6, 0x34, 0xFC, 0x6D, 0x46,
                0xE0, 0xE2, 0xF1, 0x31, 0x07, 0xE7, 0xAF, 0x23,
            },
        },
        {
            "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmn"
            "hijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu",
            {
                0x39, 0x28, 0xE1, 0x84, 0xFB, 0x86, 0x90, 0xF8,
                0x40, 0xDA, 0x39, 0x88, 0x12, 0x1D, 0x31, 0xBE,
                0x65, 0xCB, 0x9D, 0x3E, 0xF8, 0x3E, 0xE6, 0x14,
                0x6F, 0xEA, 0xC8, 0x61, 0xE1, 0x9B, 0x56, 0x3A,
            },
        },
        /* clang-format on */
    };

    sha512_256_s ctx[1];

    for (unsigned int i = 0; i != sizeof(tests) / sizeof(*tests); ++i)
    {
        sha512_256_init(ctx);
        sha512_256_proc(ctx, tests[i].msg, strlen(tests[i].msg));
        sha512_256_done(ctx, ctx->out);
        HASH_DIFF(ctx->out, tests[i].hash, SHA512_256_OUTSIZ, "sha512-256");
    }
}

static void test_sha3_224(void)
{
    /* clang-format off */
    const unsigned char sha3_224_empty[SHA3_224_OUTSIZ] = {
        0x6B, 0x4E, 0x03, 0x42, 0x36, 0x67, 0xDB, 0xB7,
        0x3B, 0x6E, 0x15, 0x45, 0x4F, 0x0E, 0xB1, 0xAB,
        0xD4, 0x59, 0x7F, 0x9A, 0x1B, 0x07, 0x8E, 0x3F,
        0x5B, 0x5A, 0x6B, 0xC7,
    };
    const unsigned char sha3_224_0xa3_200_times[SHA3_224_OUTSIZ] = {
        0x93, 0x76, 0x81, 0x6A, 0xBA, 0x50, 0x3F, 0x72,
        0xF9, 0x6C, 0xE7, 0xEB, 0x65, 0xAC, 0x09, 0x5D,
        0xEE, 0xE3, 0xBE, 0x4B, 0xF9, 0xBB, 0xC2, 0xA1,
        0xCB, 0x7E, 0x11, 0xE0,
    };
    /* clang-format on */

    sha3_s ctx[1];

    unsigned char buf[200], hash[SHA3_224_OUTSIZ];

    const unsigned char c1 = 0xa3;
    memset(buf, c1, sizeof(buf));

    /* SHA3-224 on an empty buffer */
    sha3_224_init(ctx);
    sha3_done(ctx, hash);
    HASH_DIFF(hash, sha3_224_empty, SHA3_224_OUTSIZ, "sha3-224");

    /* SHA3-224 as a single buffer. [FIPS 202] */
    sha3_224_init(ctx);
    sha3_proc(ctx, buf, sizeof(buf));
    sha3_done(ctx, hash);
    HASH_DIFF(hash, sha3_224_0xa3_200_times, SHA3_224_OUTSIZ, "sha3-224");

    /* SHA3-224 in two steps. [FIPS 202] */
    sha3_224_init(ctx);
    sha3_proc(ctx, buf, sizeof(buf) >> 1);
    sha3_proc(ctx, buf + (sizeof(buf) >> 1), sizeof(buf) >> 1);
    sha3_done(ctx, hash);
    HASH_DIFF(hash, sha3_224_0xa3_200_times, SHA3_224_OUTSIZ, "sha3-224");

    /* SHA3-224 byte-by-byte: 200 steps. [FIPS 202] */
    sha3_224_init(ctx);
    for (unsigned int i = 0; i != sizeof(buf); ++i)
    {
        sha3_proc(ctx, &c1, 1);
    }
    sha3_done(ctx, hash);
    HASH_DIFF(hash, sha3_224_0xa3_200_times, SHA3_224_OUTSIZ, "sha3-224");
}

static void test_sha3_256(void)
{
    /* clang-format off */
    const unsigned char sha3_256_empty[SHA3_256_OUTSIZ] = {
        0xA7, 0xFF, 0xC6, 0xF8, 0xBF, 0x1E, 0xD7, 0x66,
        0x51, 0xC1, 0x47, 0x56, 0xA0, 0x61, 0xD6, 0x62,
        0xF5, 0x80, 0xFF, 0x4D, 0xE4, 0x3B, 0x49, 0xFA,
        0x82, 0xD8, 0x0A, 0x4B, 0x80, 0xF8, 0x43, 0x4A,
    };
    const unsigned char sha3_256_0xa3_200_times[SHA3_256_OUTSIZ] = {
        0x79, 0xF3, 0x8A, 0xDE, 0xC5, 0xC2, 0x03, 0x07,
        0xA9, 0x8E, 0xF7, 0x6E, 0x83, 0x24, 0xAF, 0xBF,
        0xD4, 0x6C, 0xFD, 0x81, 0xB2, 0x2E, 0x39, 0x73,
        0xC6, 0x5F, 0xA1, 0xBD, 0x9D, 0xE3, 0x17, 0x87,
    };
    /* clang-format on */

    sha3_s ctx[1];

    unsigned char buf[200], hash[SHA3_256_OUTSIZ];

    const unsigned char c1 = 0xa3;
    memset(buf, c1, sizeof(buf));

    /* SHA3-256 on an empty buffer */
    sha3_256_init(ctx);
    sha3_done(ctx, hash);
    HASH_DIFF(hash, sha3_256_empty, SHA3_256_OUTSIZ, "sha3-256");

    /* SHA3-256 as a single buffer. [FIPS 202] */
    sha3_256_init(ctx);
    sha3_proc(ctx, buf, sizeof(buf));
    sha3_done(ctx, hash);
    HASH_DIFF(hash, sha3_256_0xa3_200_times, SHA3_256_OUTSIZ, "sha3-256");

    /* SHA3-256 in two steps. [FIPS 202] */
    sha3_256_init(ctx);
    sha3_proc(ctx, buf, sizeof(buf) >> 1);
    sha3_proc(ctx, buf + (sizeof(buf) >> 1), sizeof(buf) >> 1);
    sha3_done(ctx, hash);
    HASH_DIFF(hash, sha3_256_0xa3_200_times, SHA3_256_OUTSIZ, "sha3-256");

    /* SHA3-256 byte-by-byte: 200 steps. [FIPS 202] */
    sha3_256_init(ctx);
    for (unsigned int i = 0; i != sizeof(buf); ++i)
    {
        sha3_proc(ctx, &c1, 1);
    }
    sha3_done(ctx, hash);
    HASH_DIFF(hash, sha3_256_0xa3_200_times, SHA3_256_OUTSIZ, "sha3-256");

    const char *src = "\xb7\x71\xd5\xce\xf5\xd1\xa4\x1a"
                      "\x93\xd1\x56\x43\xd7\x18\x1d\x2a"
                      "\x2e\xf0\xa8\xe8\x4d\x91\x81\x2f"
                      "\x20\xed\x21\xf1\x47\xbe\xf7\x32"
                      "\xbf\x3a\x60\xef\x40\x67\xc3\x73"
                      "\x4b\x85\xbc\x8c\xd4\x71\x78\x0f"
                      "\x10\xdc\x9e\x82\x91\xb5\x83\x39"
                      "\xa6\x77\xb9\x60\x21\x8f\x71\xe7"
                      "\x93\xf2\x79\x7a\xea\x34\x94\x06"
                      "\x51\x28\x29\x06\x5d\x37\xbb\x55"
                      "\xea\x79\x6f\xa4\xf5\x6f\xd8\x89"
                      "\x6b\x49\xb2\xcd\x19\xb4\x32\x15"
                      "\xad\x96\x7c\x71\x2b\x24\xe5\x03"
                      "\x2d\x06\x52\x32\xe0\x2c\x12\x74"
                      "\x09\xd2\xed\x41\x46\xb9\xd7\x5d"
                      "\x76\x3d\x52\xdb\x98\xd9\x49\xd3"
                      "\xb0\xfe\xd6\xa8\x05\x2f\xbb";
    const char *dst = "\xa1\x9e\xee\x92\xbb\x20\x97\xb6"
                      "\x4e\x82\x3d\x59\x77\x98\xaa\x18"
                      "\xbe\x9b\x7c\x73\x6b\x80\x59\xab"
                      "\xfd\x67\x79\xac\x35\xac\x81\xb5";

    /* SHA3-256 byte-by-byte: 135 bytes. Input from [Keccak]. Output
    * matched with sha3sum. */
    sha3_256_init(ctx);
    sha3_proc(ctx, src, 1080 >> 3);
    sha3_done(ctx, hash);
    HASH_DIFF(hash, dst, SHA3_256_OUTSIZ, "sha3-256");
}

static void test_sha3_384(void)
{
    /* clang-format off */
    const unsigned char sha3_384_0xa3_200_times[SHA3_384_OUTSIZ] = {
        0x18, 0x81, 0xDE, 0x2C, 0xA7, 0xE4, 0x1E, 0xF9,
        0x5D, 0xC4, 0x73, 0x2B, 0x8F, 0x5F, 0x00, 0x2B,
        0x18, 0x9C, 0xC1, 0xE4, 0x2B, 0x74, 0x16, 0x8E,
        0xD1, 0x73, 0x26, 0x49, 0xCE, 0x1D, 0xBC, 0xDD,
        0x76, 0x19, 0x7A, 0x31, 0xFD, 0x55, 0xEE, 0x98,
        0x9F, 0x2D, 0x70, 0x50, 0xDD, 0x47, 0x3E, 0x8F,
    };
    /* clang-format on */

    sha3_s ctx[1];

    unsigned char buf[200], hash[SHA3_384_OUTSIZ];

    const unsigned char c1 = 0xa3;
    memset(buf, c1, sizeof(buf));

    /* SHA3-384 as a single buffer. [FIPS 202] */
    sha3_384_init(ctx);
    sha3_proc(ctx, buf, sizeof(buf));
    sha3_done(ctx, hash);
    HASH_DIFF(hash, sha3_384_0xa3_200_times, SHA3_256_OUTSIZ, "sha3-384");

    /* SHA3-384 in two steps. [FIPS 202] */
    sha3_384_init(ctx);
    sha3_proc(ctx, buf, sizeof(buf) >> 1);
    sha3_proc(ctx, buf + (sizeof(buf) >> 1), sizeof(buf) >> 1);
    sha3_done(ctx, hash);
    HASH_DIFF(hash, sha3_384_0xa3_200_times, SHA3_384_OUTSIZ, "sha3-384");

    /* SHA3-384 byte-by-byte: 200 steps. [FIPS 202] */
    sha3_384_init(ctx);
    for (unsigned int i = 0; i != sizeof(buf); ++i)
    {
        sha3_proc(ctx, &c1, 1);
    }
    sha3_done(ctx, hash);
    HASH_DIFF(hash, sha3_384_0xa3_200_times, SHA3_384_OUTSIZ, "sha3-384");
}

static void test_sha3_512(void)
{
    /* clang-format off */
    const unsigned char sha3_512_0xa3_200_times[SHA3_512_OUTSIZ] = {
        0xE7, 0x6D, 0xFA, 0xD2, 0x20, 0x84, 0xA8, 0xB1,
        0x46, 0x7F, 0xCF, 0x2F, 0xFA, 0x58, 0x36, 0x1B,
        0xEC, 0x76, 0x28, 0xED, 0xF5, 0xF3, 0xFD, 0xC0,
        0xE4, 0x80, 0x5D, 0xC4, 0x8C, 0xAE, 0xEC, 0xA8,
        0x1B, 0x7C, 0x13, 0xC3, 0x0A, 0xDF, 0x52, 0xA3,
        0x65, 0x95, 0x84, 0x73, 0x9A, 0x2D, 0xF4, 0x6B,
        0xE5, 0x89, 0xC5, 0x1C, 0xA1, 0xA4, 0xA8, 0x41,
        0x6D, 0xF6, 0x54, 0x5A, 0x1C, 0xE8, 0xBA, 0x00,
    };
    /* clang-format on */

    sha3_s ctx[1];

    unsigned char buf[200], hash[SHA3_512_OUTSIZ];

    const unsigned char c1 = 0xa3;
    memset(buf, c1, sizeof(buf));

    /* SHA3-512 as a single buffer. [FIPS 202] */
    sha3_512_init(ctx);
    sha3_proc(ctx, buf, sizeof(buf));
    sha3_done(ctx, hash);
    HASH_DIFF(hash, sha3_512_0xa3_200_times, SHA3_256_OUTSIZ, "sha3-384");

    /* SHA3-512 in two steps. [FIPS 202] */
    sha3_512_init(ctx);
    sha3_proc(ctx, buf, sizeof(buf) >> 1);
    sha3_proc(ctx, buf + (sizeof(buf) >> 1), sizeof(buf) >> 1);
    sha3_done(ctx, hash);
    HASH_DIFF(hash, sha3_512_0xa3_200_times, SHA3_512_OUTSIZ, "sha3-384");

    /* SHA3-512 byte-by-byte: 200 steps. [FIPS 202] */
    sha3_512_init(ctx);
    for (unsigned int i = 0; i != sizeof(buf); ++i)
    {
        sha3_proc(ctx, &c1, 1);
    }
    sha3_done(ctx, hash);
    HASH_DIFF(hash, sha3_512_0xa3_200_times, SHA3_512_OUTSIZ, "sha3-384");
}

static void test_sha3shake(void)
{
    /* clang-format off */
    const unsigned char shake256_empty[32] = {
        0xAB, 0x0B, 0xAE, 0x31, 0x63, 0x39, 0x89, 0x43,
        0x04, 0xE3, 0x58, 0x77, 0xB0, 0xC2, 0x8A, 0x9B,
        0x1F, 0xD1, 0x66, 0xC7, 0x96, 0xB9, 0xCC, 0x25,
        0x8A, 0x06, 0x4A, 0x8F, 0x57, 0xE2, 0x7F, 0x2A,
    };
    const unsigned char shake256_0xa3_200_times[32] = {
        0x6A, 0x1A, 0x9D, 0x78, 0x46, 0x43, 0x6E, 0x4D,
        0xCA, 0x57, 0x28, 0xB6, 0xF7, 0x60, 0xEE, 0xF0,
        0xCA, 0x92, 0xBF, 0x0B, 0xE5, 0x61, 0x5E, 0x96,
        0x95, 0x9D, 0x76, 0x71, 0x97, 0xA0, 0xBE, 0xEB,
    };
    const unsigned char shake128_empty[32] = {
        0x43, 0xE4, 0x1B, 0x45, 0xA6, 0x53, 0xF2, 0xA5,
        0xC4, 0x49, 0x2C, 0x1A, 0xDD, 0x54, 0x45, 0x12,
        0xDD, 0xA2, 0x52, 0x98, 0x33, 0x46, 0x2B, 0x71,
        0xA4, 0x1A, 0x45, 0xBE, 0x97, 0x29, 0x0B, 0x6F,
    };
    const unsigned char shake128_0xa3_200_times[32] = {
        0x44, 0xC9, 0xFB, 0x35, 0x9F, 0xD5, 0x6A, 0xC0,
        0xA9, 0xA7, 0x5A, 0x74, 0x3C, 0xFF, 0x68, 0x62,
        0xF1, 0x7D, 0x72, 0x59, 0xAB, 0x07, 0x52, 0x16,
        0xC0, 0x69, 0x95, 0x11, 0x64, 0x3B, 0x64, 0x39,
    };
    /* clang-format on */

    sha3_s ctx[1];

    unsigned char buf[200], hash[512];

    const unsigned char c1 = 0xa3;
    memset(buf, c1, sizeof(buf));

    /* SHAKE256 on an empty buffer */
    sha3shake_init(ctx, 0x100);
    for (unsigned int i = 0; i != 0x10; ++i)
    {
        sha3shake_done(ctx, hash, 0x20); /* get 512 bytes, keep in hash the last 32 */
    }
    HASH_DIFF(hash, shake256_empty, sizeof(shake256_empty), "shake256");

    /* SHAKE256 via [FIPS 202] */
    sha3shake_init(ctx, 0x100);
    sha3shake_proc(ctx, buf, sizeof(buf));
    sha3shake_done(ctx, hash, 0x200);
    HASH_DIFF(hash + 0x200 - 0x20, shake256_0xa3_200_times, sizeof(shake256_0xa3_200_times), "shake256");

    /* SHAKE256 as a single buffer. [FIPS 202] */
    sha3shake_init(ctx, 0x100);
    sha3shake_proc(ctx, buf, sizeof(buf));
    for (unsigned int i = 0; i != 0x10; ++i)
    {
        sha3shake_done(ctx, hash, 0x20); /* get 512 bytes, keep in hash the last 32 */
    }
    HASH_DIFF(hash, shake256_0xa3_200_times, sizeof(shake256_0xa3_200_times), "shake256");

    /* SHAKE256 in two steps. [FIPS 202] */
    sha3shake_init(ctx, 0x100);
    sha3shake_proc(ctx, buf, sizeof(buf) >> 1);
    sha3shake_proc(ctx, buf + (sizeof(buf) >> 1), sizeof(buf) >> 1);
    for (unsigned int i = 0; i != 0x10; ++i)
    {
        sha3shake_done(ctx, hash, 0x20); /* get 512 bytes, keep in hash the last 32 */
    }
    HASH_DIFF(hash, shake256_0xa3_200_times, sizeof(shake256_0xa3_200_times), "shake256");

    /* SHAKE256 byte-by-byte: 200 steps. [FIPS 202] */
    sha3shake_init(ctx, 0x100);
    for (unsigned int i = 0; i != 200; ++i)
    {
        sha3shake_proc(ctx, &c1, 1);
    }
    for (unsigned int i = 0; i != 0x10; ++i)
    {
        sha3shake_done(ctx, hash, 0x20); /* get 512 bytes, keep in hash the last 32 */
    }
    HASH_DIFF(hash, shake256_0xa3_200_times, sizeof(shake256_0xa3_200_times), "shake256");

    /* SHAKE128 on an empty buffer */
    sha3shake_init(ctx, 0x80);
    for (unsigned int i = 0; i != 0x10; ++i)
    {
        sha3shake_done(ctx, hash, 0x20); /* get 512 bytes, keep in hash the last 32 */
    }
    HASH_DIFF(hash, shake128_empty, sizeof(shake128_empty), "shake128");

    /* SHAKE128 via [FIPS 202] */
    sha3shake_init(ctx, 0x80);
    sha3shake_proc(ctx, buf, sizeof(buf));
    sha3shake_done(ctx, hash, 0x200);
    HASH_DIFF(hash + 0x200 - 0x20, shake128_0xa3_200_times, sizeof(shake128_0xa3_200_times), "shake128");

    /* SHAKE128 as a single buffer. [FIPS 202] */
    sha3shake_init(ctx, 0x80);
    sha3shake_proc(ctx, buf, sizeof(buf));
    for (unsigned int i = 0; i != 0x10; ++i)
    {
        sha3shake_done(ctx, hash, 0x20); /* get 512 bytes, keep in hash the last 32 */
    }
    HASH_DIFF(hash, shake128_0xa3_200_times, sizeof(shake128_0xa3_200_times), "shake128");

    /* SHAKE128 in two steps. [FIPS 202] */
    sha3shake_init(ctx, 0x80);
    sha3shake_proc(ctx, buf, sizeof(buf) >> 1);
    sha3shake_proc(ctx, buf + (sizeof(buf) >> 1), sizeof(buf) >> 1);
    for (unsigned int i = 0; i != 0x10; ++i)
    {
        sha3shake_done(ctx, hash, 0x20); /* get 512 bytes, keep in hash the last 32 */
    }
    HASH_DIFF(hash, shake128_0xa3_200_times, sizeof(shake128_0xa3_200_times), "shake128");

    /* SHAKE128 byte-by-byte: 200 steps. [FIPS 202] */
    sha3shake_init(ctx, 0x80);
    for (unsigned int i = 0; i != 200; ++i)
    {
        sha3shake_proc(ctx, &c1, 1);
    }
    for (unsigned int i = 0; i != 0x10; ++i)
    {
        sha3shake_done(ctx, hash, 0x20); /* get 512 bytes, keep in hash the last 32 */
    }
    HASH_DIFF(hash, shake128_0xa3_200_times, sizeof(shake128_0xa3_200_times), "shake128");
}

static void test_keccak224(void)
{
    sha3_s ctx[1];
    unsigned char hash[KECCAK224_OUTSIZ];

    const char *src = "\xcc";
    const char *dst = "\xa9\xca\xb5\x9e\xb4\x0a\x10\xb2"
                      "\x46\x29\x0f\x2d\x60\x86\xe3\x2e"
                      "\x36\x89\xfa\xf1\xd2\x6b\x47\x0c"
                      "\x89\x9f\x28\x02";

    keccak224_init(ctx);
    keccak_proc(ctx, src, 1);
    keccak_done(ctx, hash);
    HASH_DIFF(hash, dst, KECCAK224_OUTSIZ, "keccak224");

    src = "\x41\xfb";
    dst = "\x61\x5b\xa3\x67\xaf\xdc\x35\xaa"
          "\xc3\x97\xbc\x7e\xb5\xd5\x8d\x10"
          "\x6a\x73\x4b\x24\x98\x6d\x5d\x97"
          "\x8f\xef\xd6\x2c";

    keccak224_init(ctx);
    keccak_proc(ctx, src, 2);
    keccak_done(ctx, hash);
    HASH_DIFF(hash, dst, KECCAK224_OUTSIZ, "keccak224");

    src = "\x52\xa6\x08\xab\x21\xcc\xdd\x8a"
          "\x44\x57\xa5\x7e\xde\x78\x21\x76";
    dst = "\x56\x79\xcd\x50\x9c\x51\x20\xaf"
          "\x54\x79\x5c\xf4\x77\x14\x96\x41"
          "\xcf\x27\xb2\xeb\xb6\xa5\xf9\x03"
          "\x40\x70\x4e\x57";

    keccak224_init(ctx);
    keccak_proc(ctx, src, 0x10);
    keccak_done(ctx, hash);
    HASH_DIFF(hash, dst, KECCAK224_OUTSIZ, "keccak224");

    src = "\x43\x3c\x53\x03\x13\x16\x24\xc0"
          "\x02\x1d\x86\x8a\x30\x82\x54\x75"
          "\xe8\xd0\xbd\x30\x52\xa0\x22\x18"
          "\x03\x98\xf4\xca\x44\x23\xb9\x82"
          "\x14\xb6\xbe\xaa\xc2\x1c\x88\x07"
          "\xa2\xc3\x3f\x8c\x93\xbd\x42\xb0"
          "\x92\xcc\x1b\x06\xce\xdf\x32\x24"
          "\xd5\xed\x1e\xc2\x97\x84\x44\x4f"
          "\x22\xe0\x8a\x55\xaa\x58\x54\x2b"
          "\x52\x4b\x02\xcd\x3d\x5d\x5f\x69"
          "\x07\xaf\xe7\x1c\x5d\x74\x62\x22"
          "\x4a\x3f\x9d\x9e\x53\xe7\xe0\x84"
          "\x6d\xcb\xb4\xce";
    dst = "\x62\xb1\x0f\x1b\x62\x36\xeb\xc2"
          "\xda\x72\x95\x77\x42\xa8\xd4\xe4"
          "\x8e\x21\x3b\x5f\x89\x34\x60\x4b"
          "\xfd\x4d\x2c\x3a";

    keccak224_init(ctx);
    keccak_proc(ctx, src, 100);
    keccak_done(ctx, hash);
    HASH_DIFF(hash, dst, KECCAK224_OUTSIZ, "keccak224");
}

static void test_keccak256(void)
{
    sha3_s ctx[1];
    unsigned char hash[KECCAK256_OUTSIZ];

    const char *src = "\xcc";
    const char *dst = "\xee\xad\x6d\xbf\xc7\x34\x0a\x56"
                      "\xca\xed\xc0\x44\x69\x6a\x16\x88"
                      "\x70\x54\x9a\x6a\x7f\x6f\x56\x96"
                      "\x1e\x84\xa5\x4b\xd9\x97\x0b\x8a";

    keccak256_init(ctx);
    keccak_proc(ctx, src, 1);
    keccak_done(ctx, hash);
    HASH_DIFF(hash, dst, KECCAK256_OUTSIZ, "keccak256");

    src = "\x41\xfb";
    dst = "\xa8\xea\xce\xda\x4d\x47\xb3\x28"
          "\x1a\x79\x5a\xd9\xe1\xea\x21\x22"
          "\xb4\x07\xba\xf9\xaa\xbc\xb9\xe1"
          "\x8b\x57\x17\xb7\x87\x35\x37\xd2";

    keccak256_init(ctx);
    keccak_proc(ctx, src, 2);
    keccak_done(ctx, hash);
    HASH_DIFF(hash, dst, KECCAK256_OUTSIZ, "keccak256");

    src = "\x52\xa6\x08\xab\x21\xcc\xdd\x8a"
          "\x44\x57\xa5\x7e\xde\x78\x21\x76";
    dst = "\x0e\x32\xde\xfa\x20\x71\xf0\xb5"
          "\xac\x0e\x6a\x10\x8b\x84\x2e\xd0"
          "\xf1\xd3\x24\x97\x12\xf5\x8e\xe0"
          "\xdd\xf9\x56\xfe\x33\x2a\x5f\x95";

    keccak256_init(ctx);
    keccak_proc(ctx, src, 0x10);
    keccak_done(ctx, hash);
    HASH_DIFF(hash, dst, KECCAK256_OUTSIZ, "keccak256");

    src = "\x43\x3c\x53\x03\x13\x16\x24\xc0"
          "\x02\x1d\x86\x8a\x30\x82\x54\x75"
          "\xe8\xd0\xbd\x30\x52\xa0\x22\x18"
          "\x03\x98\xf4\xca\x44\x23\xb9\x82"
          "\x14\xb6\xbe\xaa\xc2\x1c\x88\x07"
          "\xa2\xc3\x3f\x8c\x93\xbd\x42\xb0"
          "\x92\xcc\x1b\x06\xce\xdf\x32\x24"
          "\xd5\xed\x1e\xc2\x97\x84\x44\x4f"
          "\x22\xe0\x8a\x55\xaa\x58\x54\x2b"
          "\x52\x4b\x02\xcd\x3d\x5d\x5f\x69"
          "\x07\xaf\xe7\x1c\x5d\x74\x62\x22"
          "\x4a\x3f\x9d\x9e\x53\xe7\xe0\x84"
          "\x6d\xcb\xb4\xce";
    dst = "\xce\x87\xa5\x17\x3b\xff\xd9\x23"
          "\x99\x22\x16\x58\xf8\x01\xd4\x5c"
          "\x29\x4d\x90\x06\xee\x9f\x3f\x9d"
          "\x41\x9c\x8d\x42\x77\x48\xdc\x41";

    keccak256_init(ctx);
    keccak_proc(ctx, src, 100);
    keccak_done(ctx, hash);
    HASH_DIFF(hash, dst, KECCAK256_OUTSIZ, "keccak256");
}

static void test_keccak384(void)
{
    sha3_s ctx[1];
    unsigned char hash[KECCAK384_OUTSIZ];

    const char *src = "\xcc";
    const char *dst = "\x1b\x84\xe6\x2a\x46\xe5\xa2\x01"
                      "\x86\x17\x54\xaf\x5d\xc9\x5c\x4a"
                      "\x1a\x69\xca\xf4\xa7\x96\xae\x40"
                      "\x56\x80\x16\x1e\x29\x57\x26\x41"
                      "\xf5\xfa\x1e\x86\x41\xd7\x95\x83"
                      "\x36\xee\x7b\x11\xc5\x8f\x73\xe9";

    keccak384_init(ctx);
    keccak_proc(ctx, src, 1);
    keccak_done(ctx, hash);
    HASH_DIFF(hash, dst, KECCAK384_OUTSIZ, "keccak384");

    src = "\x41\xfb";
    dst = "\x49\x5c\xce\x27\x14\xcd\x72\xc8"
          "\xc5\x3c\x33\x63\xd2\x2c\x58\xb5"
          "\x59\x60\xfe\x26\xbe\x0b\xf3\xbb"
          "\xc7\xa3\x31\x6d\xd5\x63\xad\x1d"
          "\xb8\x41\x0e\x75\xee\xfe\xa6\x55"
          "\xe3\x9d\x46\x70\xec\x0b\x17\x92";

    keccak384_init(ctx);
    keccak_proc(ctx, src, 2);
    keccak_done(ctx, hash);
    HASH_DIFF(hash, dst, KECCAK384_OUTSIZ, "keccak384");

    src = "\x52\xa6\x08\xab\x21\xcc\xdd\x8a"
          "\x44\x57\xa5\x7e\xde\x78\x21\x76";
    dst = "\x18\x42\x2a\xc1\xd3\xa1\xe5\x4b"
          "\xad\x87\x68\x83\xd2\xd6\xdd\x65"
          "\xf6\x5c\x1d\x5f\x33\xa7\x12\x5c"
          "\xc4\xc1\x86\x40\x5a\x12\xed\x64"
          "\xba\x96\x67\x2e\xed\xda\x8c\x5a"
          "\x63\x31\xd2\x86\x83\xf4\x88\xeb";

    keccak384_init(ctx);
    keccak_proc(ctx, src, 0x10);
    keccak_done(ctx, hash);
    HASH_DIFF(hash, dst, KECCAK384_OUTSIZ, "keccak384");

    src = "\x43\x3c\x53\x03\x13\x16\x24\xc0"
          "\x02\x1d\x86\x8a\x30\x82\x54\x75"
          "\xe8\xd0\xbd\x30\x52\xa0\x22\x18"
          "\x03\x98\xf4\xca\x44\x23\xb9\x82"
          "\x14\xb6\xbe\xaa\xc2\x1c\x88\x07"
          "\xa2\xc3\x3f\x8c\x93\xbd\x42\xb0"
          "\x92\xcc\x1b\x06\xce\xdf\x32\x24"
          "\xd5\xed\x1e\xc2\x97\x84\x44\x4f"
          "\x22\xe0\x8a\x55\xaa\x58\x54\x2b"
          "\x52\x4b\x02\xcd\x3d\x5d\x5f\x69"
          "\x07\xaf\xe7\x1c\x5d\x74\x62\x22"
          "\x4a\x3f\x9d\x9e\x53\xe7\xe0\x84"
          "\x6d\xcb\xb4\xce";
    dst = "\x13\x51\x14\x50\x8d\xd6\x3e\x27"
          "\x9e\x70\x9c\x26\xf7\x81\x7c\x04"
          "\x82\x76\x6c\xde\x49\x13\x2e\x3e"
          "\xdf\x2e\xed\xd8\x99\x6f\x4e\x35"
          "\x96\xd1\x84\x10\x0b\x38\x48\x68"
          "\x24\x9f\x1d\x8b\x8f\xda\xa2\xc9";

    keccak384_init(ctx);
    keccak_proc(ctx, src, 100);
    keccak_done(ctx, hash);
    HASH_DIFF(hash, dst, KECCAK384_OUTSIZ, "keccak384");
}

static void test_keccak512(void)
{
    sha3_s ctx[1];
    unsigned char hash[KECCAK512_OUTSIZ];

    const char *src = "\xcc";
    const char *dst = "\x86\x30\xc1\x3c\xbd\x06\x6e\xa7"
                      "\x4b\xbe\x7f\xe4\x68\xfe\xc1\xde"
                      "\xe1\x0e\xdc\x12\x54\xfb\x4c\x1b"
                      "\x7c\x5f\xd6\x9b\x64\x6e\x44\x16"
                      "\x0b\x8c\xe0\x1d\x05\xa0\x90\x8c"
                      "\xa7\x90\xdf\xb0\x80\xf4\xb5\x13"
                      "\xbc\x3b\x62\x25\xec\xe7\xa8\x10"
                      "\x37\x14\x41\xa5\xac\x66\x6e\xb9";

    keccak512_init(ctx);
    keccak_proc(ctx, src, 1);
    keccak_done(ctx, hash);
    HASH_DIFF(hash, dst, KECCAK512_OUTSIZ, "keccak512");

    src = "\x41\xfb";
    dst = "\x55\x1d\xa6\x23\x6f\x8b\x96\xfc"
          "\xe9\xf9\x7f\x11\x90\xe9\x01\x32"
          "\x4f\x0b\x45\xe0\x6d\xbb\xb5\xcd"
          "\xb8\x35\x5d\x6e\xd1\xdc\x34\xb3"
          "\xf0\xea\xe7\xdc\xb6\x86\x22\xff"
          "\x23\x2f\xa3\xce\xce\x0d\x46\x16"
          "\xcd\xeb\x39\x31\xf9\x38\x03\x66"
          "\x2a\x28\xdf\x1c\xd5\x35\xb7\x31";

    keccak512_init(ctx);
    keccak_proc(ctx, src, 2);
    keccak_done(ctx, hash);
    HASH_DIFF(hash, dst, KECCAK512_OUTSIZ, "keccak512");

    src = "\x52\xa6\x08\xab\x21\xcc\xdd\x8a"
          "\x44\x57\xa5\x7e\xde\x78\x21\x76";
    dst = "\x4b\x39\xd3\xda\x5b\xcd\xf4\xd9"
          "\xb7\x69\x01\x59\x95\x64\x43\x11"
          "\xc1\x4c\x43\x5b\xf7\x2b\x10\x09"
          "\xd6\xdd\x71\xb0\x1a\x63\xb9\x7c"
          "\xfb\x59\x64\x18\xe8\xe4\x23\x42"
          "\xd1\x17\xe0\x74\x71\xa8\x91\x43"
          "\x14\xba\x7b\x0e\x26\x4d\xad\xf0"
          "\xce\xa3\x81\x86\x8c\xbd\x43\xd1";

    keccak512_init(ctx);
    keccak_proc(ctx, src, 0x10);
    keccak_done(ctx, hash);
    HASH_DIFF(hash, dst, KECCAK512_OUTSIZ, "keccak512");

    src = "\x43\x3c\x53\x03\x13\x16\x24\xc0"
          "\x02\x1d\x86\x8a\x30\x82\x54\x75"
          "\xe8\xd0\xbd\x30\x52\xa0\x22\x18"
          "\x03\x98\xf4\xca\x44\x23\xb9\x82"
          "\x14\xb6\xbe\xaa\xc2\x1c\x88\x07"
          "\xa2\xc3\x3f\x8c\x93\xbd\x42\xb0"
          "\x92\xcc\x1b\x06\xce\xdf\x32\x24"
          "\xd5\xed\x1e\xc2\x97\x84\x44\x4f"
          "\x22\xe0\x8a\x55\xaa\x58\x54\x2b"
          "\x52\x4b\x02\xcd\x3d\x5d\x5f\x69"
          "\x07\xaf\xe7\x1c\x5d\x74\x62\x22"
          "\x4a\x3f\x9d\x9e\x53\xe7\xe0\x84"
          "\x6d\xcb\xb4\xce";
    dst = "\x52\x7d\x28\xe3\x41\xe6\xb1\x4f"
          "\x46\x84\xad\xb4\xb8\x24\xc4\x96"
          "\xc6\x48\x2e\x51\x14\x95\x65\xd3"
          "\xd1\x72\x26\x82\x88\x84\x30\x6b"
          "\x51\xd6\x14\x8a\x72\x62\x2c\x2b"
          "\x75\xf5\xd3\x51\x0b\x79\x9d\x8b"
          "\xdc\x03\xea\xed\xe4\x53\x67\x6a"
          "\x6e\xc8\xfe\x03\xa1\xad\x0e\xab";

    keccak512_init(ctx);
    keccak_proc(ctx, src, 100);
    keccak_done(ctx, hash);
    HASH_DIFF(hash, dst, KECCAK512_OUTSIZ, "keccak512");
}

static void test_blake2s_128(void)
{
    static const struct
    {
        const char *msg;
        unsigned char hash[BLAKE2S_128_OUTSIZ];
    } tests[] = {
        /* clang-format off */
        {
            "",
            {
                0x64, 0x55, 0x0D, 0x6F, 0xFE, 0x2C, 0x0A, 0x01,
                0xA1, 0x4A, 0xBA, 0x1E, 0xAD, 0xE0, 0x20, 0x0C,
            },
        },
        {
            "abc",
            {
                0xAA, 0x49, 0x38, 0x11, 0x9B, 0x1D, 0xC7, 0xB8,
                0x7C, 0xBA, 0xD0, 0xFF, 0xD2, 0x00, 0xD0, 0xAE,
            },
        },
        /* clang-format on */
    };

    blake2s_s ctx[1];

    for (unsigned int i = 0; i != sizeof(tests) / sizeof(*tests); ++i)
    {
        blake2s_128_init(ctx);
        blake2s_proc(ctx, tests[i].msg, strlen(tests[i].msg));
        blake2s_done(ctx, ctx->out);
        HASH_DIFF(ctx->out, tests[i].hash, BLAKE2S_128_OUTSIZ, "blake2s-128");
    }
}

static void test_blake2s_160(void)
{
    static const struct
    {
        const char *msg;
        unsigned char hash[BLAKE2S_160_OUTSIZ];
    } tests[] = {
        /* clang-format off */
        {
            "",
            {
                0x35, 0x4C, 0x9C, 0x33, 0xF7, 0x35, 0x96, 0x24,
                0x18, 0xBD, 0xAC, 0xB9, 0x47, 0x98, 0x73, 0x42,
                0x9C, 0x34, 0x91, 0x6F,
            },
        },
        {
            "abc",
            {
                0x5A, 0xE3, 0xB9, 0x9B, 0xE2, 0x9B, 0x01, 0x83,
                0x4C, 0x3B, 0x50, 0x85, 0x21, 0xED, 0xE6, 0x04,
                0x38, 0xF8, 0xDE, 0x17,
            },
        },
        /* clang-format on */
    };

    blake2s_s ctx[1];

    for (unsigned int i = 0; i != sizeof(tests) / sizeof(*tests); ++i)
    {
        blake2s_160_init(ctx);
        blake2s_proc(ctx, tests[i].msg, strlen(tests[i].msg));
        blake2s_done(ctx, ctx->out);
        HASH_DIFF(ctx->out, tests[i].hash, BLAKE2S_160_OUTSIZ, "blake2s-160");
    }
}

static void test_blake2s_224(void)
{
    static const struct
    {
        const char *msg;
        unsigned char hash[BLAKE2S_224_OUTSIZ];
    } tests[] = {
        /* clang-format off */
        {
            "",
            {
                0x1F, 0xA1, 0x29, 0x1E, 0x65, 0x24, 0x8B, 0x37,
                0xB3, 0x43, 0x34, 0x75, 0xB2, 0xA0, 0xDD, 0x63,
                0xD5, 0x4A, 0x11, 0xEC, 0xC4, 0xE3, 0xE0, 0x34,
                0xE7, 0xBC, 0x1E, 0xF4,
            },
        },
        {
            "abc",
            {
                0x0B, 0x03, 0x3F, 0xC2, 0x26, 0xDF, 0x7A, 0xBD,
                0xE2, 0x9F, 0x67, 0xA0, 0x5D, 0x3D, 0xC6, 0x2C,
                0xF2, 0x71, 0xEF, 0x3D, 0xFE, 0xA4, 0xD3, 0x87,
                0x40, 0x7F, 0xBD, 0x55,
            },
        },
        /* clang-format on */
    };

    blake2s_s ctx[1];

    for (unsigned int i = 0; i != sizeof(tests) / sizeof(*tests); ++i)
    {
        blake2s_224_init(ctx);
        blake2s_proc(ctx, tests[i].msg, strlen(tests[i].msg));
        blake2s_done(ctx, ctx->out);
        HASH_DIFF(ctx->out, tests[i].hash, BLAKE2S_224_OUTSIZ, "blake2s-224");
    }
}

static void test_blake2s_256(void)
{
    static const struct
    {
        const char *msg;
        unsigned char hash[BLAKE2S_256_OUTSIZ];
    } tests[] = {
        /* clang-format off */
        {
            "",
            {
                0x69, 0x21, 0x7A, 0x30, 0x79, 0x90, 0x80, 0x94,
                0xE1, 0x11, 0x21, 0xD0, 0x42, 0x35, 0x4A, 0x7C,
                0x1F, 0x55, 0xB6, 0x48, 0x2C, 0xA1, 0xA5, 0x1E,
                0x1B, 0x25, 0x0D, 0xFD, 0x1E, 0xD0, 0xEE, 0xF9,
            },
        },
        {
            "abc",
            {
                0x50, 0x8C, 0x5E, 0x8C, 0x32, 0x7C, 0x14, 0xE2,
                0xE1, 0xA7, 0x2B, 0xA3, 0x4E, 0xEB, 0x45, 0x2F,
                0x37, 0x45, 0x8B, 0x20, 0x9E, 0xD6, 0x3A, 0x29,
                0x4D, 0x99, 0x9B, 0x4C, 0x86, 0x67, 0x59, 0x82,
            },
        },
        {
            "12345678901234567890123456789012345678901234567890"
            "12345678901234567890123456789012345678901234567890"
            "12345678901234567890123456789012345678901234567890"
            "12345678901234567890123456789012345678901234567890"
            "12345678901234567890123456789012345678901234567890"
            "12345678901234567890123456789012345678901234567890",
            {
                0xA3, 0x78, 0x8B, 0x5B, 0x59, 0xEE, 0xE4, 0x41,
                0x95, 0x23, 0x58, 0x00, 0xA4, 0xF9, 0xFA, 0x41,
                0x86, 0x0C, 0x7B, 0x1C, 0x35, 0xA2, 0x42, 0x70,
                0x50, 0x80, 0x79, 0x56, 0xE3, 0xBE, 0x31, 0x74,
            },
        },
        /* clang-format on */
    };

    blake2s_s ctx[1];

    for (unsigned int i = 0; i != sizeof(tests) / sizeof(*tests); ++i)
    {
        blake2s_256_init(ctx);
        blake2s_proc(ctx, tests[i].msg, strlen(tests[i].msg));
        blake2s_done(ctx, ctx->out);
        HASH_DIFF(ctx->out, tests[i].hash, BLAKE2S_256_OUTSIZ, "blake2s-256");
    }
}

static void test_blake2b_160(void)
{
    static const struct
    {
        const char *msg;
        unsigned char hash[BLAKE2B_160_OUTSIZ];
    } tests[] = {
        /* clang-format off */
        {
            "",
            {
                0x33, 0x45, 0x52, 0x4A, 0xBF, 0x6B, 0xBE, 0x18,
                0x09, 0x44, 0x92, 0x24, 0xB5, 0x97, 0x2C, 0x41,
                0x79, 0x0B, 0x6C, 0xF2,
            },
        },
        { "abc",
            {
                0x38, 0x42, 0x64, 0xF6, 0x76, 0xF3, 0x95, 0x36,
                0x84, 0x05, 0x23, 0xF2, 0x84, 0x92, 0x1C, 0xDC,
                0x68, 0xB6, 0x84, 0x6B,
            },
        },
        /* clang-format on */
    };

    blake2b_s ctx[1];

    for (unsigned int i = 0; i != sizeof(tests) / sizeof(*tests); ++i)
    {
        blake2b_160_init(ctx);
        blake2b_proc(ctx, tests[i].msg, strlen(tests[i].msg));
        blake2b_done(ctx, ctx->out);
        HASH_DIFF(ctx->out, tests[i].hash, BLAKE2B_160_OUTSIZ, "blake2b-160");
    }
}

static void test_blake2b_256(void)
{
    static const struct
    {
        const char *msg;
        unsigned char hash[BLAKE2B_256_OUTSIZ];
    } tests[] = {
        /* clang-format off */
        {
            "",
            {
                0x0E, 0x57, 0x51, 0xC0, 0x26, 0xE5, 0x43, 0xB2,
                0xE8, 0xAB, 0x2E, 0xB0, 0x60, 0x99, 0xDA, 0xA1,
                0xD1, 0xE5, 0xDF, 0x47, 0x77, 0x8F, 0x77, 0x87,
                0xFA, 0xAB, 0x45, 0xCD, 0xF1, 0x2F, 0xE3, 0xA8,
            },
        },
        {
            "abc",
            {
                0xBD, 0xDD, 0x81, 0x3C, 0x63, 0x42, 0x39, 0x72,
                0x31, 0x71, 0xEF, 0x3F, 0xEE, 0x98, 0x57, 0x9B,
                0x94, 0x96, 0x4E, 0x3B, 0xB1, 0xCB, 0x3E, 0x42,
                0x72, 0x62, 0xC8, 0xC0, 0x68, 0xD5, 0x23, 0x19,
            },
        },
        {
            "12345678901234567890123456789012345678901234567890"
            "12345678901234567890123456789012345678901234567890"
            "12345678901234567890123456789012345678901234567890"
            "12345678901234567890123456789012345678901234567890"
            "12345678901234567890123456789012345678901234567890"
            "12345678901234567890123456789012345678901234567890",
            {
                0x0F, 0x6E, 0x01, 0x8D, 0x38, 0xD6, 0x3F, 0x08,
                0x4D, 0x58, 0xE3, 0x0C, 0x90, 0xFB, 0xA2, 0x41,
                0x5F, 0xCA, 0x17, 0xFA, 0x66, 0x26, 0x49, 0xF3,
                0x8A, 0x30, 0x41, 0x7C, 0x57, 0xCD, 0xA8, 0x14,
            },
        },
        /* clang-format on */
    };

    blake2b_s ctx[1];

    for (unsigned int i = 0; i != sizeof(tests) / sizeof(*tests); ++i)
    {
        blake2b_256_init(ctx);
        blake2b_proc(ctx, tests[i].msg, strlen(tests[i].msg));
        blake2b_done(ctx, ctx->out);
        HASH_DIFF(ctx->out, tests[i].hash, BLAKE2B_256_OUTSIZ, "blake2b-256");
    }
}

static void test_blake2b_384(void)
{
    static const struct
    {
        const char *msg;
        unsigned char hash[BLAKE2B_384_OUTSIZ];
    } tests[] = {
        /* clang-format off */
        {
            "",
            {
                0xB3, 0x28, 0x11, 0x42, 0x33, 0x77, 0xF5, 0x2D,
                0x78, 0x62, 0x28, 0x6E, 0xE1, 0xA7, 0x2E, 0xE5,
                0x40, 0x52, 0x43, 0x80, 0xFD, 0xA1, 0x72, 0x4A,
                0x6F, 0x25, 0xD7, 0x97, 0x8C, 0x6F, 0xD3, 0x24,
                0x4A, 0x6C, 0xAF, 0x04, 0x98, 0x81, 0x26, 0x73,
                0xC5, 0xE0, 0x5E, 0xF5, 0x83, 0x82, 0x51, 0x00,
            },
        },
        {
            "abc",
            {
                0x6F, 0x56, 0xA8, 0x2C, 0x8E, 0x7E, 0xF5, 0x26,
                0xDF, 0xE1, 0x82, 0xEB, 0x52, 0x12, 0xF7, 0xDB,
                0x9D, 0xF1, 0x31, 0x7E, 0x57, 0x81, 0x5D, 0xBD,
                0xA4, 0x60, 0x83, 0xFC, 0x30, 0xF5, 0x4E, 0xE6,
                0xC6, 0x6B, 0xA8, 0x3B, 0xE6, 0x4B, 0x30, 0x2D,
                0x7C, 0xBA, 0x6C, 0xE1, 0x5B, 0xB5, 0x56, 0xF4,
            },
        },
        /* clang-format on */
    };

    blake2b_s ctx[1];

    for (unsigned int i = 0; i != sizeof(tests) / sizeof(*tests); ++i)
    {
        blake2b_384_init(ctx);
        blake2b_proc(ctx, tests[i].msg, strlen(tests[i].msg));
        blake2b_done(ctx, ctx->out);
        HASH_DIFF(ctx->out, tests[i].hash, BLAKE2B_384_OUTSIZ, "blake2b-384");
    }
}

static void test_blake2b_512(void)
{
    static const struct
    {
        const char *msg;
        unsigned char hash[BLAKE2B_512_OUTSIZ];
    } tests[] = {
        /* clang-format off */
        {
            "",
            {
                0x78, 0x6A, 0x02, 0xF7, 0x42, 0x01, 0x59, 0x03,
                0xC6, 0xC6, 0xFD, 0x85, 0x25, 0x52, 0xD2, 0x72,
                0x91, 0x2F, 0x47, 0x40, 0xE1, 0x58, 0x47, 0x61,
                0x8A, 0x86, 0xE2, 0x17, 0xF7, 0x1F, 0x54, 0x19,
                0xD2, 0x5E, 0x10, 0x31, 0xAF, 0xEE, 0x58, 0x53,
                0x13, 0x89, 0x64, 0x44, 0x93, 0x4E, 0xB0, 0x4B,
                0x90, 0x3A, 0x68, 0x5B, 0x14, 0x48, 0xB7, 0x55,
                0xD5, 0x6F, 0x70, 0x1A, 0xFE, 0x9B, 0xE2, 0xCE,
            },
        },
        {
            "abc",
            {
                0xBA, 0x80, 0xA5, 0x3F, 0x98, 0x1C, 0x4D, 0x0D,
                0x6A, 0x27, 0x97, 0xB6, 0x9F, 0x12, 0xF6, 0xE9,
                0x4C, 0x21, 0x2F, 0x14, 0x68, 0x5A, 0xC4, 0xB7,
                0x4B, 0x12, 0xBB, 0x6F, 0xDB, 0xFF, 0xA2, 0xD1,
                0x7D, 0x87, 0xC5, 0x39, 0x2A, 0xAB, 0x79, 0x2D,
                0xC2, 0x52, 0xD5, 0xDE, 0x45, 0x33, 0xCC, 0x95,
                0x18, 0xD3, 0x8A, 0xA8, 0xDB, 0xF1, 0x92, 0x5A,
                0xB9, 0x23, 0x86, 0xED, 0xD4, 0x00, 0x99, 0x23,
            },
        },
        /* clang-format on */
    };

    blake2b_s ctx[1];

    for (unsigned int i = 0; i != sizeof(tests) / sizeof(*tests); ++i)
    {
        blake2b_512_init(ctx);
        blake2b_proc(ctx, tests[i].msg, strlen(tests[i].msg));
        blake2b_done(ctx, ctx->out);
        HASH_DIFF(ctx->out, tests[i].hash, BLAKE2B_512_OUTSIZ, "blake2b-512");
    }
}

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif /* __GNUC__ || __clang__ */

int main(void)
{
    test_md5();

    test_sha1();

    test_sha256();
    test_sha224();
    test_sha384();
    test_sha512();
    test_sha512_224();
    test_sha512_256();

    test_sha3_224();
    test_sha3_256();
    test_sha3_384();
    test_sha3_512();
    test_sha3shake();
    test_keccak224();
    test_keccak256();
    test_keccak384();
    test_keccak512();

    test_blake2s_128();
    test_blake2s_160();
    test_blake2s_224();
    test_blake2s_256();

    test_blake2b_160();
    test_blake2b_256();
    test_blake2b_384();
    test_blake2b_512();

    return 0;
}
