#include "cipher/cipher.h"
#include "cipher/hmac.h"

#include <assert.h>
#include <string.h>
#include <ctype.h>

static hash_s const *cipher_hash(char const *text)
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

static char *hmac(void const *key, size_t keysiz, void const *msg, size_t msgsiz, hash_s const *hash, void *out)
{
    assert(!keysiz || key);
    assert(!msgsiz || msg);

    hmac_s ctx[1];

    hmac_init(ctx, hash, key, keysiz);
    hmac_proc(ctx, msg, msgsiz);
    hmac_done(ctx, ctx->buf);

    return (char *)digest_lower(ctx->buf, ctx->outsiz, out);
}

int cipher_v1(cipher_s const *ctx, char const *word, char **out)
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
    hash_s const *hash = cipher_hash(ctx->hash);
    unsigned int lword = (unsigned int)strlen(word);
    unsigned int ltext = (unsigned int)strlen(ctx->text);
    if ((ctx->size == 0) || (lword == 0) || (ltext == 0))
    {
        return -1;
    }

    unsigned char count = 0;
    unsigned char num[10] = {0};
    unsigned int outsiz = hash->outsiz << 1;
    unsigned int length = ctx->size < outsiz ? ctx->size : outsiz;
    unsigned char *msg = (unsigned char *)hmac(ctx->text, ltext, word, lword, hash, 0);

    char *buf0 = hmac("kise", 4, msg, outsiz, hash, 0);
    char *buf1 = hmac("snow", 4, msg, outsiz, hash, 0);

    *out = (char *)calloc(length + 1, sizeof(char));
    for (unsigned int i = 0; i != length; ++i)
    {
        int x = xdigit(buf0[i]) + xdigit(buf1[i]);
        msg[i] = (unsigned char)x;

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
            unsigned int lmisc = (unsigned int)strlen(ctx->misc);
            for (unsigned int i = 0; i != lmisc; ++i)
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
    char const *s0;
    char const *s1;
    char const *s2;
    char const *s3;
    /* length for rule data */
    unsigned int l0;
    unsigned int l1;
    unsigned int l2;
    unsigned int l3;
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

void cipher_v2_init(void const *s0, void const *s1, void const *s2, void const *s3)
{
    /* Set the rules */
    stat->s0 = (char const *)s0;
    stat->s1 = (char const *)s1;
    stat->s2 = (char const *)s2;
    stat->s3 = (char const *)s3;
    /* Set the rule length */
    stat->l0 = s0 ? (unsigned int)strlen(stat->s0) : 0;
    stat->l1 = s1 ? (unsigned int)strlen(stat->s1) : 0;
    stat->l2 = s2 ? (unsigned int)strlen(stat->s2) : 0;
    stat->l3 = s3 ? (unsigned int)strlen(stat->s3) : 0;
}

int cipher_v2(cipher_s const *ctx, char const *word, char **out)
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
    hash_s const *hash = cipher_hash(ctx->hash);
    unsigned int lword = (unsigned int)strlen(word);
    unsigned int ltext = (unsigned int)strlen(ctx->text);
    if ((ctx->size == 0) || (lword == 0) || (ltext == 0))
    {
        return -1;
    }

    unsigned char count = 0;
    unsigned char num[CH] = {0};
    unsigned int outsiz = hash->outsiz << 1;
    unsigned int length = ctx->size < outsiz ? ctx->size : outsiz;
    unsigned char *msg = (unsigned char *)hmac(ctx->text, ltext, word, lword, hash, 0);

    char *buf0 = hmac(stat->s0, stat->l0, msg, outsiz, hash, 0);
    char *buf1 = hmac(stat->s1, stat->l1, msg, outsiz, hash, 0);
    char *buf2 = hmac(stat->s2, stat->l2, msg, outsiz, hash, 0);
    char *buf3 = hmac(stat->s3, stat->l3, msg, outsiz, hash, 0);

    *out = (char *)calloc(length + 1, sizeof(char));
    for (unsigned int i = 0; i != length; ++i)
    {
        int x = xdigit(buf0[i]) + xdigit(buf1[i]) + xdigit(buf2[i]) + xdigit(buf3[i]);
        msg[i] = (unsigned char)x;

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
        unsigned int lmisc = (unsigned int)strlen(ctx->misc);
        for (unsigned int i = 0; i != lmisc; ++i)
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
#define CIPHER_SET_FIELD(field)                                    \
    int cipher_set_##field(cipher_s *ctx, void const *field)       \
    {                                                              \
        assert(ctx);                                               \
        char *str = 0;                                             \
        if (field)                                                 \
        {                                                          \
            str = (char *)malloc(strlen((char const *)field) + 1); \
            if (str == 0)                                          \
            {                                                      \
                return ~0;                                         \
            }                                                      \
            strcpy(str, (char const *)field);                      \
        }                                                          \
        if (ctx->field)                                            \
        {                                                          \
            free(ctx->field);                                      \
        }                                                          \
        ctx->field = str;                                          \
        return 0;                                                  \
    }
CIPHER_SET_FIELD(hint)
CIPHER_SET_FIELD(misc)
CIPHER_SET_FIELD(text)
CIPHER_SET_FIELD(hash)
#undef CIPHER_SET_FIELD

void cipher_set_type(cipher_s *ctx, unsigned int type)
{
    assert(ctx);
    ctx->type = type % CIPHER_TOTAL;
}

void cipher_set_size(cipher_s *ctx, unsigned int size)
{
    assert(ctx);
    unsigned int outsiz = cipher_hash(ctx->hash)->outsiz << 1;
    ctx->size = size < outsiz ? size : outsiz;
}

int cipher_copy(cipher_s *ctx, cipher_s const *obj)
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

int xdigit(int x)
{
    int ret = ~0;
    switch (x)
    {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    {
        /* 0 ~ 9 */
        ret = x - '0';
    }
    break;
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
    {
        /* A ~ F */
        ret = x - 'A' + 10;
    }
    break;
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
    {
        /* a ~ f */
        ret = x - 'a' + 10;
    }
    break;
    default:
        break;
    }
    return ret;
}

#undef TO
#define TO(func, to)                                       \
    void *func(void const *pdata, size_t nbyte, void *out) \
    {                                                      \
        assert(!nbyte || pdata);                           \
        char const *p = (char const *)pdata;               \
        if (out || ((void)(out = malloc(nbyte + 1)), out)) \
        {                                                  \
            char *o = (char *)out;                         \
            pdata = (void const *)(p + nbyte);             \
            for (; p != pdata; ++p)                        \
            {                                              \
                *o++ = (char)to(*p);                       \
            }                                              \
            *o = 0;                                        \
        }                                                  \
        return out;                                        \
    }
TO(lower, tolower)
TO(upper, toupper)
#undef TO

void *digest(void const *pdata, size_t nbyte, unsigned int cases, void *out)
{
    static char const hexits[2][0x10] = {
        /* clang-format off */
        {
            '0', '1', '2', '3', '4', '5', '6', '7',
            '8', '9', 'a', 'b', 'c', 'd', 'e', 'f',
        },
        {
            '0', '1', '2', '3', '4', '5', '6', '7',
            '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
        },
        /* clang-format on */
    };

    assert(!nbyte || pdata);

    char const *hexit = hexits[cases % 2];
    unsigned char const *p = (unsigned char const *)pdata;
    if (out || ((void)(out = malloc((nbyte << 1) + 1)), out))
    {
        char *o = (char *)out;
        pdata = (void const *)(p + nbyte);
        while (p != pdata)
        {
            *o++ = hexit[*p >> 0x4];
            *o++ = hexit[*p & 0x0F];
            ++p;
        }
        *o = 0;
    }

    return out;
}

void *digest_lower(void const *pdata, size_t nbyte, void *out)
{
    return digest(pdata, nbyte, 0, out);
}

void *digest_upper(void const *pdata, size_t nbyte, void *out)
{
    return digest(pdata, nbyte, 1, out);
}