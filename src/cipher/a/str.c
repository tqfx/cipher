/*!
 @file str.c
 @brief basic string library
 @copyright Copyright (C) 2020-present tqfx, All rights reserved.
*/

#include "cipher/a/str.h"

#include <assert.h>
#include <stdio.h>

str_s *str_new(void)
{
    str_s *ctx = (str_s *)malloc(sizeof(str_s));
    if (ctx)
    {
        str_ctor(ctx);
    }
    return ctx;
}

void str_die(str_s *ctx)
{
    if (ctx)
    {
        str_dtor(ctx);
        free(ctx);
    }
}

void str_ctor(str_s *ctx)
{
    assert(ctx);
    ctx->__str = NULL;
    ctx->__num = 0;
    ctx->__mem = 0;
}

void str_dtor(str_s *ctx)
{
    assert(ctx);
    if (ctx->__str)
    {
        free(ctx->__str);
        ctx->__str = NULL;
    }
    ctx->__num = 0;
    ctx->__mem = 0;
}

#if defined(__clang__)
#pragma GCC diagnostic ignored "-Wcomma"
#endif /* __clang__ */

#ifndef roundup32
#define roundup32(x)     \
    (--(x),              \
     (x) |= (x) >> 0x01, \
     (x) |= (x) >> 0x02, \
     (x) |= (x) >> 0x04, \
     (x) |= (x) >> 0x08, \
     (x) |= (x) >> 0x10, \
     ++(x))
#endif /* roundup32 */

int str_init(str_s *ctx, const void *pdata, size_t nbyte)
{
    assert(ctx);
    ctx->__num = nbyte;
    ctx->__mem = nbyte + 1;
    ctx->__str = (char *)malloc(roundup32(ctx->__mem));
    if (unlikely(ctx->__str == NULL))
    {
        return 1;
    }
    if (pdata && nbyte)
    {
        memcpy(ctx->__str, pdata, nbyte);
    }
    ctx->__str[ctx->__num] = 0;
    return 0;
}

int str_copy(str_s *ctx, const str_s *obj)
{
    assert(ctx);
    assert(obj);
    return str_init(ctx, obj->__str, obj->__num);
}

str_s *str_move(str_s *ctx, str_s *obj)
{
    assert(ctx);
    assert(obj);
    memcpy(ctx, obj, sizeof(str_s));
    memset(obj, 000, sizeof(str_s));
    return ctx;
}

char *str_exit(str_s *ctx)
{
    assert(ctx);
    char *str = ctx->__str;
    if (ctx->__str)
    {
        ctx->__str[ctx->__num] = 0;
        ctx->__str = NULL;
    }
    ctx->__mem = 0;
    ctx->__num = 0;
    return str;
}

int str_cmp(const str_s *lhs, const str_s *rhs)
{
    assert(lhs);
    assert(rhs);
    int ok = 0;
    if (lhs->__str && rhs->__str)
    {
        size_t num = lhs->__num < rhs->__num ? lhs->__num : rhs->__num;
        ok = memcmp(lhs->__str, rhs->__str, num);
    }
    if (ok)
    {
        return ok;
    }
    if (lhs->__num == rhs->__num)
    {
        return 0;
    }
    return lhs->__num < rhs->__num ? -1 : 1;
}

int str_resize_(str_s *ctx, size_t mem)
{
    assert(ctx);
    char *str = (char *)realloc(ctx->__str, roundup32(mem));
    if (unlikely(!str && mem))
    {
        return 1;
    }
    ctx->__str = str;
    ctx->__mem = mem;
    return 0;
}

int str_resize(str_s *ctx, size_t mem)
{
    assert(ctx);
    return ctx->__mem < mem ? str_resize_(ctx, mem) : 0;
}

int str_putc_(str_s *ctx, int c)
{
    assert(ctx);
    if (unlikely(str_resize(ctx, ctx->__num + 1)))
    {
        return EOF;
    }
    ctx->__str[ctx->__num++] = (char)c;
    return c;
}

int str_putc(str_s *ctx, int c)
{
    assert(ctx);
    if (unlikely(c == 0))
    {
        return str_putc_(ctx, c);
    }
    if (unlikely(str_resize(ctx, ctx->__num + 2)))
    {
        return EOF;
    }
    ctx->__str[ctx->__num++] = (char)c;
    ctx->__str[ctx->__num] = 0;
    return c;
}

int str_putn_(str_s *ctx, const void *pdata, size_t nbyte)
{
    assert(ctx);
    if (pdata && nbyte)
    {
        if (unlikely(str_resize(ctx, ctx->__num + nbyte)))
        {
            return 1;
        }
        memcpy(ctx->__str + ctx->__num, pdata, nbyte);
        ctx->__num += nbyte;
    }
    return 0;
}

int str_putn(str_s *ctx, const void *pdata, size_t nbyte)
{
    assert(ctx);
    if (pdata)
    {
        if (unlikely(str_resize(ctx, ctx->__num + nbyte + 1)))
        {
            return 1;
        }
        if (nbyte)
        {
            memcpy(ctx->__str + ctx->__num, pdata, nbyte);
            ctx->__num += nbyte;
        }
        ctx->__str[ctx->__num] = 0;
    }
    return 0;
}

int str_puts(str_s *ctx, const void *str)
{
    assert(ctx);
    assert(str);
    return str_putn(ctx, str, strlen((const char *)str));
}

int str_vprintf(str_s *ctx, const char *fmt, va_list va)
{
    assert(ctx);
    assert(fmt);
    va_list ap;
    va_copy(ap, va);
    char *str = ctx->__str ? ctx->__str + ctx->__num : NULL;
    int ret = vsnprintf(str, ctx->__mem - ctx->__num, fmt, ap);
    va_end(ap);
    size_t size = (size_t)ret + 1;
    if (ctx->__mem - ctx->__num < size)
    {
        if (unlikely(str_resize_(ctx, ctx->__num + size)))
        {
            return EOF;
        }
        va_copy(ap, va);
        str = ctx->__str + ctx->__num;
        ret = vsnprintf(str, ctx->__mem - ctx->__num, fmt, ap);
        va_end(ap);
    }
    ctx->__num += (size_t)ret;
    return ret;
}

int str_printf(str_s *ctx, const char *fmt, ...)
{
    assert(ctx);
    assert(fmt);
    va_list ap;
    va_start(ap, fmt);
    int ret = str_vprintf(ctx, fmt, ap);
    va_end(ap);
    return ret;
}
