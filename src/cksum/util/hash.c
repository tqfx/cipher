/*!
 @file hash.c
 @brief hash library utils
 @copyright Copyright (C) 2020 tqfx, All rights reserved.
*/

#include "cksum/util/hash.h"

#include "../hash.h"

#include <assert.h>
#include <stdarg.h>

int hash_memory(const hash_s *ctx, const void *pdata, size_t nbyte, void *out, size_t *siz)
{
    assert(ctx);
    assert(out);
    assert(siz);
    assert(!nbyte || pdata);

    if (*siz < ctx->outsiz)
    {
        *siz = ctx->outsiz;
        return OVERFLOW;
    }

    hash_u hash[1];

    ctx->init(hash);
    if (ctx->proc(hash, pdata, nbyte) != SUCCESS)
    {
        return FAILURE;
    }
    *siz = ctx->done(hash, out) ? ctx->outsiz : 0;

    return SUCCESS;
}

int hash_mmulti(const hash_s *ctx, void *out, size_t *siz, const void *pdata, size_t nbyte, ...)
{
    assert(ctx);
    assert(out);
    assert(siz);
    assert(!nbyte || pdata);

    if (*siz < ctx->outsiz)
    {
        *siz = ctx->outsiz;
        return OVERFLOW;
    }

    va_list arg;
    hash_u hash[1];
    int ret = SUCCESS;
    va_start(arg, nbyte);

    ctx->init(hash);
    for (;;)
    {
        ret = ctx->proc(hash, pdata, nbyte);
        if (ret != SUCCESS)
        {
            goto done;
        }
        pdata = va_arg(arg, const void *);
        if (pdata == 0)
        {
            break;
        }
        nbyte = va_arg(arg, size_t);
    }
    *siz = ctx->done(hash, out) ? ctx->outsiz : 0;

done:
    va_end(arg);
    return ret;
}

int hash_filehandle(const hash_s *ctx, FILE *in, void *out, size_t *siz)
{
    assert(in);
    assert(ctx);
    assert(out);
    assert(siz);

    if (*siz < ctx->outsiz)
    {
        *siz = ctx->outsiz;
        return OVERFLOW;
    }

    char buf[BUFSIZ];
    hash_u hash[1];

    ctx->init(hash);
    do
    {
        *siz = fread(buf, 1, BUFSIZ, in);
        ctx->proc(hash, buf, *siz);
    } while (*siz == BUFSIZ);
    *siz = ctx->done(hash, out) ? ctx->outsiz : 0;

    return SUCCESS;
}

int hash_file(const hash_s *ctx, const char *fname, void *out, size_t *siz)
{
    assert(ctx);
    assert(out);
    assert(siz);
    assert(fname);

    if (*siz < ctx->outsiz)
    {
        *siz = ctx->outsiz;
        return OVERFLOW;
    }

    FILE *in = fopen(fname, "rb");
    if (in == 0)
    {
        return NOTFOUND;
    }

    int ret = hash_filehandle(ctx, in, out, siz);

    if (fclose(in))
    {
        return FAILURE;
    }

    return ret;
}
