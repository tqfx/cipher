/*!
 @file cipher.c
 @brief cipher core
 @copyright Copyright (C) 2020-present tqfx, All rights reserved.
*/

#include "cipher/cipher.h"

#include "cksum/util/hmac.h"
#include "cksum/util/conv.h"

#include <assert.h>
#include <string.h>
#include <ctype.h>

static const hash_s *cipher_hash(cstr_t text)
{
    if (text == 0)
    {
        return &hash_md5;
    }
    switch (*text)
    {
    case 'm':
    {
        if (strcmp(text, "md5") == 0)
        {
            return &hash_md5;
        }
    }
    break;
    case 'M':
    {
        if (strcmp(text, "MD5") == 0)
        {
            return &hash_md5;
        }
    }
    break;
    case 's':
    {
        if (strcmp(text, "sha1") == 0)
        {
            return &hash_sha1;
        }
        if (strcmp(text, "sha256") == 0)
        {
            return &hash_sha256;
        }
        if (strcmp(text, "sha224") == 0)
        {
            return &hash_sha224;
        }
        if (strcmp(text, "sha512") == 0)
        {
            return &hash_sha512;
        }
        if (strcmp(text, "sha384") == 0)
        {
            return &hash_sha384;
        }
        if (strcmp(text, "sha3") == 0)
        {
            return &hash_sha3_512;
        }
    }
    break;
    case 'S':
    {
        if (strcmp(text, "SHA1") == 0)
        {
            return &hash_sha1;
        }
        if (strcmp(text, "SHA256") == 0)
        {
            return &hash_sha256;
        }
        if (strcmp(text, "SHA224") == 0)
        {
            return &hash_sha224;
        }
        if (strcmp(text, "SHA512") == 0)
        {
            return &hash_sha512;
        }
        if (strcmp(text, "SHA384") == 0)
        {
            return &hash_sha384;
        }
        if (strcmp(text, "SHA3") == 0)
        {
            return &hash_sha3_512;
        }
    }
    break;
    case 'b':
    {
        if (strcmp(text, "blake2s") == 0)
        {
            return &hash_blake2s_256;
        }
        if (strcmp(text, "blake2b") == 0)
        {
            return &hash_blake2b_512;
        }
    }
    break;
    case 'B':
    {
        if (strcmp(text, "BLAKE2S") == 0)
        {
            return &hash_blake2s_256;
        }
        if (strcmp(text, "BLAKE2B") == 0)
        {
            return &hash_blake2b_512;
        }
    }
    break;
    default:
        break;
    }
    return &hash_md5;
}

static str_t hmac(cptr_t key, size_t keysiz, cptr_t msg, size_t msgsiz, const hash_s *hash, vptr_t out)
{
    assert(!keysiz || key);
    assert(!msgsiz || msg);

    hmac_s ctx[1];

    hmac_init(ctx, hash, key, keysiz);
    hmac_proc(ctx, msg, msgsiz);
    hmac_done(ctx, ctx->buf);

    return (str_t)digest_lower(ctx->buf, ctx->outsiz, out);
}

int cipher_v1(const cipher_s *ctx, cstr_t word, str_t *out)
{
    assert(ctx);
    assert(out);
    assert(word);

    if (ctx->text == 0)
    {
        return -3;
    }
    if (ctx->type == CIPHER_OTHER && ctx->misc == 0)
    {
        return -2;
    }
    const hash_s *hash = cipher_hash(ctx->hash);
    uint_t lword = (uint_t)strlen(word);
    uint_t ltext = (uint_t)strlen(ctx->text);
    if ((ctx->size == 0) || (lword == 0) || (ltext == 0))
    {
        return -1;
    }

    byte_t count = 0;
    byte_t num[10] = {0};
    uint_t outsiz = hash->outsiz << 1;
    uint_t length = ctx->size < outsiz ? ctx->size : outsiz;
    byte_t *msg = (byte_t *)hmac(ctx->text, ltext, word, lword, hash, 0);

    str_t buf0 = hmac("kise", 4, msg, outsiz, hash, 0);
    str_t buf1 = hmac("snow", 4, msg, outsiz, hash, 0);

    *out = (str_t)calloc(length + 1, sizeof(char));
    for (uint_t i = 0; i != length; ++i)
    {
        int x = xdigit(buf0[i]) + xdigit(buf1[i]);
        msg[i] = (byte_t)x;

        switch (ctx->type)
        {
        case CIPHER_EMAIL:
        case CIPHER_OTHER:
        {
            (*out)[i] = buf1[i];
            if (!isdigit((int)(*out)[i]))
            {
                if (strchr("sunlovesnow1990090127xykab", buf0[i]))
                {
                    (*out)[i] = (char)toupper((*out)[i]);
                }
            }
        }
        break;

        case CIPHER_DIGIT:
        {
            x %= 10;

            int m = x;
            while (num[x] > count)
            {
                if (++x == 10)
                {
                    x = 0;
                }
                if (x == m)
                {
                    ++count;
                }
            }
            ++num[x];

            (*out)[i] = (char)('0' + x);
        }
        break;

        default:
            break;
        }
    }

    if (ctx->type != CIPHER_DIGIT)
    {
        if (isdigit((int)(*out)[0]))
        {
            (*out)[0] = 'K';
        }
        if (ctx->type == CIPHER_OTHER)
        {
            uint_t lmisc = (uint_t)strlen(ctx->misc);
            for (uint_t i = 0; i != lmisc; ++i)
            {
                (*out)[msg[i % outsiz] % length] = ctx->misc[i];
            }
        }
    }

    free(buf1);
    free(buf0);
    free(msg);

    return 0;
}

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
#endif /* __GNUC__ || __clang__ */

#undef CH
#define CH 61

static struct
{
    /* points to rule data */
    cstr_t s0;
    cstr_t s1;
    cstr_t s2;
    cstr_t s3;
    /* length for rule data */
    uint_t l0;
    uint_t l1;
    uint_t l2;
    uint_t l3;
    /* character table */
    const char ch[CH];
} stat[1] = {
    {
        .s0 = 0,
        .s1 = 0,
        .s2 = 0,
        .s3 = 0,
        .l0 = 0,
        .l1 = 0,
        .l2 = 0,
        .l3 = 0,
        .ch = {
            /* clang-format off */
            'a', 'A', 'b', 'B', 'c', 'C', 'd', 'D', 'e', 'E', 'f', 'F', 'g', 'G', 'h',
            'H', 'i', 'j', 'J', 'k', 'K', 'l', 'L', 'm', 'M', '0', '1', '2', '3', '4', 'I',
            '5', '6', '7', '8', '9', 'n', 'N', 'o', 'p', 'P', 'q', 'Q', 'r', 'R', 's',
            'S', 't', 'T', 'u', 'U', 'v', 'V', 'w', 'W', 'x', 'X', 'y', 'Y', 'z', 'Z',
            /* clang-format on */
        },
    },
};

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif /* __GNUC__ || __clang__ */

void cipher_v2_init(cptr_t s0, cptr_t s1, cptr_t s2, cptr_t s3)
{
    /* Set the rules */
    stat->s0 = (cstr_t)s0;
    stat->s1 = (cstr_t)s1;
    stat->s2 = (cstr_t)s2;
    stat->s3 = (cstr_t)s3;
    /* Set the rule length */
    stat->l0 = s0 ? (uint_t)strlen(stat->s0) : 0;
    stat->l1 = s1 ? (uint_t)strlen(stat->s1) : 0;
    stat->l2 = s2 ? (uint_t)strlen(stat->s2) : 0;
    stat->l3 = s3 ? (uint_t)strlen(stat->s3) : 0;
}

int cipher_v2(const cipher_s *ctx, cstr_t word, str_t *out)
{
    assert(ctx);
    assert(out);
    assert(word);

    if (ctx->text == 0)
    {
        return -3;
    }
    if (ctx->type == CIPHER_OTHER && ctx->misc == 0)
    {
        return -2;
    }
    const hash_s *hash = cipher_hash(ctx->hash);
    uint_t lword = (uint_t)strlen(word);
    uint_t ltext = (uint_t)strlen(ctx->text);
    if ((ctx->size == 0) || (lword == 0) || (ltext == 0))
    {
        return -1;
    }

    byte_t count = 0;
    byte_t num[CH] = {0};
    uint_t outsiz = hash->outsiz << 1;
    uint_t length = ctx->size < outsiz ? ctx->size : outsiz;
    byte_t *msg = (byte_t *)hmac(ctx->text, ltext, word, lword, hash, 0);

    str_t buf0 = hmac(stat->s0, stat->l0, msg, outsiz, hash, 0);
    str_t buf1 = hmac(stat->s1, stat->l1, msg, outsiz, hash, 0);
    str_t buf2 = hmac(stat->s2, stat->l2, msg, outsiz, hash, 0);
    str_t buf3 = hmac(stat->s3, stat->l3, msg, outsiz, hash, 0);

    *out = (str_t)calloc(length + 1, sizeof(char));
    for (uint_t i = 0; i != length; ++i)
    {
        int x = xdigit(buf0[i]) + xdigit(buf1[i]) + xdigit(buf2[i]) + xdigit(buf3[i]);
        msg[i] = (byte_t)x;

        switch (ctx->type)
        {
        case CIPHER_EMAIL:
        case CIPHER_OTHER:
        {
            int m = x;
            while (num[x] > count)
            {
                if ((m % 2 == 0) && (CH == ++x))
                {
                    x = 0;
                }
                if ((m % 2 == 1) && (0 == x--))
                {
                    x = CH - 1;
                }
                if (x == m)
                {
                    ++count;
                }
            }
            ++num[x];

            (*out)[i] = stat->ch[x];
        }
        break;
#undef CH
#define CH 10
        case CIPHER_DIGIT:
        {
            x %= CH;

            int m = x;
            while (num[x] > count)
            {
                if ((m % 2 == 0) && (CH == ++x))
                {
                    x = 0;
                }
                if ((m % 2 == 1) && (0 == x--))
                {
                    x = CH - 1;
                }
                if (x == m)
                {
                    ++count;
                }
            }
            ++num[x];

            (*out)[i] = (char)('0' + x);
        }
        break;
#undef CH
        default:
            break;
        }
    }

    if (ctx->type == CIPHER_OTHER)
    {
        uint_t lmisc = (uint_t)strlen(ctx->misc);
        for (uint_t i = 0; i != lmisc; ++i)
        {
            (*out)[msg[i % outsiz] % length] = ctx->misc[i];
        }
    }

    free(buf3);
    free(buf2);
    free(buf1);
    free(buf0);
    free(msg);

    return 0;
}

void cipher_ctor(cipher_s *ctx)
{
    assert(ctx);
    ctx->hint = 0;
    ctx->misc = 0;
    ctx->text = 0;
    ctx->hash = 0;
    ctx->type = CIPHER_EMAIL;
    ctx->size = 16;
}

void cipher_dtor(cipher_s *ctx)
{
    assert(ctx);
    ctx->hint ? ((void)(free(ctx->hint)), ctx->hint = 0) : 0;
    ctx->misc ? ((void)(free(ctx->misc)), ctx->misc = 0) : 0;
    ctx->text ? ((void)(free(ctx->text)), ctx->text = 0) : 0;
    ctx->hash ? ((void)(free(ctx->hash)), ctx->hash = 0) : 0;
    ctx->type = CIPHER_EMAIL;
    ctx->size = 16;
}

cipher_s *cipher_new(void)
{
    cipher_s *ctx = (cipher_s *)malloc(sizeof(cipher_s));
    if (ctx)
    {
        cipher_ctor(ctx);
    }
    return ctx;
}

void cipher_die(cipher_s *ctx)
{
    if (ctx)
    {
        cipher_dtor(ctx);
        free(ctx);
    }
}

#undef CIPHER_SET_FIELD
#define CIPHER_SET_FIELD(field)                             \
    int cipher_set_##field(cipher_s *ctx, cptr_t field)     \
    {                                                       \
        assert(ctx);                                        \
        str_t str = 0;                                      \
        if (field)                                          \
        {                                                   \
            str = (str_t)malloc(strlen((cstr_t)field) + 1); \
            if (str == 0)                                   \
            {                                               \
                return ~0;                                  \
            }                                               \
            strcpy(str, (cstr_t)field);                     \
        }                                                   \
        if (ctx->field)                                     \
        {                                                   \
            free(ctx->field);                               \
        }                                                   \
        ctx->field = str;                                   \
        return 0;                                           \
    }
CIPHER_SET_FIELD(hint)
CIPHER_SET_FIELD(misc)
CIPHER_SET_FIELD(text)
CIPHER_SET_FIELD(hash)
#undef CIPHER_SET_FIELD

void cipher_set_type(cipher_s *ctx, uint_t type)
{
    assert(ctx);
    ctx->type = type % CIPHER_TOTAL;
}

void cipher_set_size(cipher_s *ctx, uint_t size)
{
    assert(ctx);
    uint_t outsiz = cipher_hash(ctx->hash)->outsiz << 1;
    ctx->size = size < outsiz ? size : outsiz;
}

int cipher_copy(cipher_s *ctx, const cipher_s *obj)
{
    assert(ctx);
    assert(obj);
    if (cipher_set_hint(ctx, cipher_get_hint(obj)))
    {
        return ~0;
    }
    if (cipher_set_text(ctx, cipher_get_text(obj)))
    {
        return ~0;
    }
    if (cipher_set_misc(ctx, cipher_get_misc(obj)))
    {
        return ~0;
    }
    if (cipher_set_hash(ctx, cipher_get_hash(obj)))
    {
        return ~0;
    }
    cipher_set_type(ctx, cipher_get_type(obj));
    cipher_set_size(ctx, cipher_get_size(obj));
    return 0;
}

cipher_s *cipher_move(cipher_s *ctx, cipher_s *obj)
{
    assert(ctx);
    assert(obj);
    memcpy(ctx, obj, sizeof(cipher_s));
    memset(obj, 000, sizeof(cipher_s));
    return ctx;
}
