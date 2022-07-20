/*!
 @file str.c
 @brief basic string library
 @copyright Copyright (C) 2020 tqfx, All rights reserved.
*/

#include "cipher/str.h"
#include <stdio.h>
#include <assert.h>

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
    ctx->__mem = 0;
    ctx->__num = 0;
    ctx->__str = 0;
}

void str_dtor(str_s *ctx)
{
    assert(ctx);
    ctx->__mem = 0;
    ctx->__num = 0;
    if (ctx->__str)
    {
        free(ctx->__str);
        ctx->__str = 0;
    }
}

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
    roundup32(ctx->__mem);
    ctx->__str = (char *)malloc(ctx->__mem);
    if (ctx->__str == 0)
    {
        return ~0;
    }
    if (pdata && nbyte)
    {
        memcpy(ctx->__str, pdata, nbyte);
    }
    ctx->__str[ctx->__num] = 0;
    return 0;
}

int str_copy(str_s *ctx, const str_s *str)
{
    assert(ctx);
    assert(str);
    return str_init(ctx, str->__str, str->__num);
}

str_s *str_move(str_s *ctx, str_s *str)
{
    assert(ctx);
    assert(str);
    memcpy(ctx, str, sizeof(str_s));
    memset(str, 0, sizeof(str_s));
    return ctx;
}

char *str_exit(str_s *ctx)
{
    assert(ctx);
    char *str = ctx->__str;
    if (ctx->__str)
    {
        ctx->__str[ctx->__num] = 0;
        ctx->__str = 0;
    }
    ctx->__mem = 0;
    ctx->__num = 0;
    return str;
}

int str_resize_(str_s *ctx, size_t mem)
{
    assert(ctx);
    roundup32(mem);
    char *str = (char *)realloc(ctx->__str, mem);
    if (!str && mem)
    {
        return ~0;
    }
    ctx->__str = str;
    ctx->__mem = mem;
    return 0;
}

int str_resize(str_s *ctx, size_t mem)
{
    assert(ctx);
    if (ctx->__mem < mem)
    {
        return str_resize_(ctx, mem);
    }
    return 0;
}

int str_putc_(str_s *ctx, int c)
{
    assert(ctx);
    if (str_resize(ctx, ctx->__num + 1))
    {
        return ~0;
    }
    ctx->__str[ctx->__num++] = (char)c;
    return c;
}

int str_putc(str_s *ctx, int c)
{
    assert(ctx);
    if (c == 0)
    {
        return str_putc_(ctx, c);
    }
    if (str_resize(ctx, ctx->__num + 2))
    {
        return ~0;
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
        if (str_resize(ctx, ctx->__num + nbyte))
        {
            return ~0;
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
        if (str_resize(ctx, ctx->__num + nbyte + 1))
        {
            return ~0;
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
    char *str = ctx->__str ? (ctx->__str + ctx->__num) : 0;
    int ret = vsnprintf(str, ctx->__mem - ctx->__num, fmt, ap);
    va_end(ap);
    size_t size = (size_t)ret + 1;
    if (ctx->__mem - ctx->__num < size)
    {
        if (str_resize_(ctx, ctx->__num + size))
        {
            return ~0;
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
