/*!
 @file hmac.c
 @brief RFC 2104 compliant HMAC implementation
 @details https://www.ietf.org/rfc/rfc2104.txt
 @copyright Copyright (C) 2020-present tqfx, All rights reserved.
*/

#include "cksum/hmac.h"

#include "hash.h"

#include <assert.h>
#include <string.h>

#undef HMAC_IPAD
#undef HMAC_OPAD
#define HMAC_IPAD 0x36
#define HMAC_OPAD 0x5C

int hmac_init(hmac_s *ctx, const hash_s *hash, const void *pdata, size_t nbyte)
{
    assert(ctx);
    assert(hash);
    assert(!nbyte || pdata);

    if (sizeof(ctx->buf) < hash->bufsiz)
    {
        return OVERFLOW;
    }

    ctx->__hash = hash;
    ctx->outsiz = ctx->__hash->outsiz;

    if (ctx->__hash->bufsiz < nbyte)
    {
        ctx->__hash->init(ctx->__state);
        if (ctx->__hash->proc(ctx->__state, pdata, nbyte) != SUCCESS)
        {
            return FAILURE;
        }
        if (ctx->__hash->done(ctx->__state, ctx->buf) == 0)
        {
            return FAILURE;
        }
        nbyte = ctx->__hash->outsiz;
    }
    else if (nbyte)
    {
        memcpy(ctx->buf, pdata, nbyte);
    }

    if (nbyte < ctx->__hash->bufsiz)
    {
        memset(ctx->buf + nbyte, 0, sizeof(ctx->buf) - nbyte);
    }

    unsigned char buf[sizeof(ctx->buf)];
    for (unsigned int i = 0; i != ctx->__hash->bufsiz; ++i)
    {
        buf[i] = ctx->buf[i] ^ HMAC_IPAD;
    }

    ctx->__hash->init(ctx->__state);
    return ctx->__hash->proc(ctx->__state, buf, ctx->__hash->bufsiz);
}

int hmac_proc(hmac_s *ctx, const void *pdata, size_t nbyte)
{
    assert(ctx);
    assert(!nbyte || pdata);

    return ctx->__hash->proc(ctx->__state, pdata, nbyte);
}

unsigned char *hmac_done(hmac_s *ctx, void *out)
{
    assert(ctx);

    unsigned char buf[sizeof(ctx->buf)];

    if (ctx->__hash->done(ctx->__state, buf) == 0)
    {
        return 0;
    }

    for (unsigned int i = 0; i != ctx->__hash->bufsiz; ++i)
    {
        ctx->buf[i] ^= HMAC_OPAD;
    }

    ctx->__hash->init(ctx->__state);
    if (ctx->__hash->proc(ctx->__state, ctx->buf, ctx->__hash->bufsiz) != SUCCESS)
    {
        return 0;
    }
    if (ctx->__hash->proc(ctx->__state, buf, ctx->__hash->outsiz) != SUCCESS)
    {
        return 0;
    }
    if (ctx->__hash->done(ctx->__state, ctx->buf) == 0)
    {
        return 0;
    }

    if (out && out != ctx->buf)
    {
        memcpy(out, ctx->buf, ctx->__hash->outsiz);
    }

    return ctx->buf;
}

#undef HMAC_IPAD
#undef HMAC_OPAD
