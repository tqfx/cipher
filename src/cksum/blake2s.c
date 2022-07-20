/*!
 @file blake2s.c
 @brief RFC 7693 compliant BLAKE implementation
 @details https://www.ietf.org/rfc/rfc7693.txt
 @copyright Copyright (C) 2020 tqfx, All rights reserved.
*/

#include "cksum/blake2s.h"

#include "hash.h"

static const uint32_t blake2s_IV[8] = {
    /* clang-format off */
    0x6A09E667, 0xBB67AE85, 0x3C6EF372, 0xA54FF53A,
    0x510E527F, 0x9B05688C, 0x1F83D9AB, 0x5BE0CD19,
    /* clang-format on */
};

static const unsigned char blake2s_sigma[10][0x10] = {
    /* clang-format off */
    {0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF},
    {0xE, 0xA, 0x4, 0x8, 0x9, 0xF, 0xD, 0x6, 0x1, 0xC, 0x0, 0x2, 0xB, 0x7, 0x5, 0x3},
    {0xB, 0x8, 0xC, 0x0, 0x5, 0x2, 0xF, 0xD, 0xA, 0xE, 0x3, 0x6, 0x7, 0x1, 0x9, 0x4},
    {0x7, 0x9, 0x3, 0x1, 0xD, 0xC, 0xB, 0xE, 0x2, 0x6, 0x5, 0xA, 0x4, 0x0, 0xF, 0x8},
    {0x9, 0x0, 0x5, 0x7, 0x2, 0x4, 0xA, 0xF, 0xE, 0x1, 0xB, 0xC, 0x6, 0x8, 0x3, 0xD},
    {0x2, 0xC, 0x6, 0xA, 0x0, 0xB, 0x8, 0x3, 0x4, 0xD, 0x7, 0x5, 0xF, 0xE, 0x1, 0x9},
    {0xC, 0x5, 0x1, 0xF, 0xE, 0xD, 0x4, 0xA, 0x0, 0x7, 0x6, 0x3, 0x9, 0x2, 0x8, 0xB},
    {0xD, 0xB, 0x7, 0xE, 0xC, 0x1, 0x3, 0x9, 0x5, 0x0, 0xF, 0x4, 0x8, 0x6, 0x2, 0xA},
    {0x6, 0xF, 0xE, 0x9, 0xB, 0x3, 0x0, 0x8, 0xC, 0x2, 0xD, 0x7, 0x1, 0x4, 0xA, 0x5},
    {0xA, 0x2, 0x8, 0x4, 0x7, 0x6, 0x1, 0x5, 0xF, 0xB, 0x9, 0xE, 0x3, 0xC, 0xD, 0x0},
    /* clang-format on */
};

/* param offsets */
enum
{
    O_DIGEST_LENGTH = 0,
    O_KEY_LENGTH = 1,
    O_FANOUT = 2,
    O_DEPTH = 3,
    O_LEAF_LENGTH = 4,
    O_NODE_OFFSET = 8,
    O_XOF_LENGTH = 12,
    O_NODE_DEPTH = 14,
    O_INNER_LENGTH = 15,
    O_SALT = 16,
    O_PERSONAL = 24,
    A_PARAM_SIZE = 32,
};

static inline void blake2s_set_lastnode(blake2s_s *ctx) { ctx->__f[1] = 0xFFFFFFFF; }

static inline int blake2s_is_lastblock(const blake2s_s *ctx) { return (ctx->__f[0] != 0); }

static inline void blake2s_set_lastblock(blake2s_s *ctx)
{
    if (ctx->__lastnode)
    {
        blake2s_set_lastnode(ctx);
    }
    ctx->__f[0] = 0xFFFFFFFF;
}

static inline void blake2s_increment_counter(blake2s_s *ctx, uint32_t inc)
{
    ctx->__t[0] += inc;
    if (ctx->__t[0] < inc)
    {
        ++ctx->__t[1];
    }
}

static void blake2s_compress(blake2s_s *ctx, const unsigned char *buf)
{
    uint32_t v[0x10];
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
#endif /* __GNUC__ || __clang__ */
    uint32_t *m = (uint32_t *)ctx->__buf;
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif /* __GNUC__ || __clang__ */
    if (ctx->__buf != buf)
    {
        for (unsigned int i = 0; i != 0x10; ++i)
        {
            LOAD32L(m[i], buf + sizeof(*ctx->__state) * i);
        }
    }

    /* copy state into v */
    for (unsigned int i = 0; i != sizeof(ctx->__state) / sizeof(*ctx->__state); ++i)
    {
        v[i] = ctx->__state[i];
    }

    v[8] = blake2s_IV[0];
    v[9] = blake2s_IV[1];
    v[10] = blake2s_IV[2];
    v[11] = blake2s_IV[3];
    v[12] = ctx->__t[0] ^ blake2s_IV[4];
    v[13] = ctx->__t[1] ^ blake2s_IV[5];
    v[14] = ctx->__f[0] ^ blake2s_IV[6];
    v[15] = ctx->__f[1] ^ blake2s_IV[7];

#undef G
#define G(r, i, a, b, c, d)                            \
    do                                                 \
    {                                                  \
        a = a + b + m[blake2s_sigma[r][(i << 1) + 0]]; \
        d = ROR(d ^ a, 0x10);                          \
        c = c + d;                                     \
        b = ROR(b ^ c, 0x0C);                          \
        a = a + b + m[blake2s_sigma[r][(i << 1) + 1]]; \
        d = ROR(d ^ a, 0x08);                          \
        c = c + d;                                     \
        b = ROR(b ^ c, 0x07);                          \
    } while (0)
#undef ROUND
#define ROUND(r)                                 \
    do                                           \
    {                                            \
        G(r, 0, v[0x0], v[0x4], v[0x8], v[0xC]); \
        G(r, 1, v[0x1], v[0x5], v[0x9], v[0xD]); \
        G(r, 2, v[0x2], v[0x6], v[0xA], v[0xE]); \
        G(r, 3, v[0x3], v[0x7], v[0xB], v[0xF]); \
        G(r, 4, v[0x0], v[0x5], v[0xA], v[0xF]); \
        G(r, 5, v[0x1], v[0x6], v[0xB], v[0xC]); \
        G(r, 6, v[0x2], v[0x7], v[0x8], v[0xD]); \
        G(r, 7, v[0x3], v[0x4], v[0x9], v[0xE]); \
    } while (0)
    ROUND(0);
    ROUND(1);
    ROUND(2);
    ROUND(3);
    ROUND(4);
    ROUND(5);
    ROUND(6);
    ROUND(7);
    ROUND(8);
    ROUND(9);
#undef ROUND
#undef G

    for (unsigned int i = 0; i != sizeof(ctx->__state) / sizeof(*ctx->__state); ++i)
    {
        ctx->__state[i] = ctx->__state[i] ^ v[i] ^ v[i + 8];
    }
}

int blake2s_init(blake2s_s *ctx, size_t siz, const void *pdata, size_t nbyte)
{
    assert(ctx);

    unsigned char ap[A_PARAM_SIZE] = {0};

    if ((siz == 0) || (sizeof(ctx->out) < siz))
    {
        return INVALID;
    }

    if ((pdata && !nbyte) || (nbyte && !pdata) || (sizeof(ctx->__buf) < nbyte))
    {
        return INVALID;
    }

    ap[O_DIGEST_LENGTH] = (unsigned char)siz;
    ap[O_KEY_LENGTH] = (unsigned char)nbyte;
    ap[O_FANOUT] = 1;
    ap[O_DEPTH] = 1;

    memset(ctx, 0, sizeof(*ctx));

    for (unsigned int i = 0; i != sizeof(ctx->__state) / sizeof(*ctx->__state); ++i)
    {
        ctx->__state[i] = blake2s_IV[i];
    }

    /* IV XOR ParamBlock */
    for (unsigned int i = 0; i != sizeof(ctx->__state) / sizeof(*ctx->__state); ++i)
    {
        uint32_t t;
        LOAD32L(t, ap + sizeof(*ctx->__state) * i);
        ctx->__state[i] ^= t;
    }

    ctx->outsiz = ap[O_DIGEST_LENGTH];

    if (pdata)
    {
        memcpy(ctx->__buf, pdata, nbyte);
        blake2s_proc(ctx, ctx->__buf, sizeof(ctx->__buf));
    }

    return SUCCESS;
}

#undef BLAKE2S_INIT
#define BLAKE2S_INIT(bit)                     \
    void blake2s_##bit##_init(blake2s_s *ctx) \
    {                                         \
        blake2s_init(ctx, (bit) >> 3, 0, 0);  \
    }
BLAKE2S_INIT(128)
BLAKE2S_INIT(160)
BLAKE2S_INIT(224)
BLAKE2S_INIT(256)
#undef BLAKE2S_INIT

int blake2s_proc(blake2s_s *ctx, const void *pdata, size_t nbyte)
{
    assert(ctx);
    assert(!nbyte || pdata);

    if (sizeof(ctx->__buf) < ctx->__cursiz)
    {
        return INVALID;
    }

    const unsigned char *p = (const unsigned char *)pdata;
    uint32_t n = (uint32_t)sizeof(ctx->__buf) - ctx->__cursiz;
    if (nbyte > n)
    {
        memcpy(ctx->__buf + ctx->__cursiz, p, n);
        blake2s_increment_counter(ctx, sizeof(ctx->__buf));
        blake2s_compress(ctx, ctx->__buf);
        ctx->__cursiz = 0;
        nbyte -= n;
        p += n;
        while (nbyte > sizeof(ctx->__buf) - 1)
        {
            blake2s_increment_counter(ctx, sizeof(ctx->__buf));
            blake2s_compress(ctx, p);
            nbyte -= sizeof(ctx->__buf);
            p += sizeof(ctx->__buf);
        }
    }
    if (nbyte)
    {
        memcpy(ctx->__buf + ctx->__cursiz, p, nbyte);
        ctx->__cursiz += (uint32_t)nbyte;
    }

    return SUCCESS;
}

unsigned char *blake2s_done(blake2s_s *ctx, void *out)
{
    assert(ctx);

    if (blake2s_is_lastblock(ctx))
    {
        return 0;
    }

    blake2s_increment_counter(ctx, ctx->__cursiz);
    blake2s_set_lastblock(ctx);

    /* padding */
    memset(ctx->__buf + ctx->__cursiz, 0, sizeof(ctx->__buf) - ctx->__cursiz);
    blake2s_compress(ctx, ctx->__buf);

    /* copy output */
    for (unsigned int i = 0; i != sizeof(ctx->__state) / sizeof(*ctx->__state); ++i)
    {
        STORE32L(ctx->__state[i], ctx->out + sizeof(*ctx->__state) * i);
    }

    if (out && (out != ctx->out))
    {
        memcpy(out, ctx->out, ctx->outsiz);
    }

    return ctx->out;
}
