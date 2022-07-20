/*!
 @file info.c
 @brief cipher infomation
 @copyright Copyright (C) 2020 tqfx, All rights reserved.
*/

#include "cipher/info.h"

#include <string.h>

void cipher_info_ctor(cipher_info_s *ctx)
{
    ctx->mem = 0;
    ctx->num = 0;
    ctx->vec = 0;
}

void cipher_info_dtor(cipher_info_s *ctx)
{
    if (ctx->vec)
    {
        while (ctx->num)
        {
            --ctx->num;
            cipher_dtor(ctx->vec + ctx->num);
        }
        free(ctx->vec);
        ctx->vec = 0;
    }
    ctx->num = 0;
    ctx->mem = 0;
}

cipher_info_s *cipher_info_new(void)
{
    cipher_info_s *ctx = (cipher_info_s *)malloc(sizeof(cipher_info_s));
    if (ctx)
    {
        cipher_info_ctor(ctx);
    }
    return ctx;
}

void cipher_info_die(cipher_info_s *ctx)
{
    if (ctx)
    {
        cipher_info_dtor(ctx);
        free(ctx);
    }
}

cipher_s *cipher_info_push(cipher_info_s *ctx)
{
    if (ctx->mem <= ctx->num)
    {
        size_t mem = ctx->mem + (ctx->mem >> 1) + 1;
        cipher_s *vec = (cipher_s *)realloc(ctx->vec, sizeof(cipher_s) * mem);
        if (vec == 0)
        {
            return 0;
        }
        ctx->mem = mem;
        ctx->vec = vec;
    }
    cipher_s *obj = ctx->vec + ctx->num++;
    cipher_ctor(obj);
    return obj;
}

cipher_s *cipher_info_pop(cipher_info_s *ctx) { return ctx->num ? ctx->vec + --ctx->num : 0; }

int cipher_info_add(cipher_info_s *ctx, cipher_s *obj)
{
    cipher_s *it = 0;
    cipher_info_forenum(i, ctx)
    {
        it = cipher_info_at(ctx, i);
        if (cipher_get_text(it) && strcmp(cipher_get_text(it), cipher_get_text(obj)) == 0)
        {
            cipher_dtor(it);
            break;
        }
        it = 0;
    }
    if (it == 0)
    {
        it = cipher_info_push(ctx);
        cipher_ctor(it);
        if (it == 0)
        {
            return ~0;
        }
    }
    return cipher_copy(it, obj);
}

int cipher_info_del(cipher_info_s *ctx, cipher_s *obj)
{
    int ret = ~0;
    cipher_info_foreach(it, ctx)
    {
        if (cipher_get_text(it) && strcmp(cipher_get_text(it), cipher_get_text(obj)) == 0)
        {
            cipher_dtor(it);
            ret = 0;
        }
    }
    return ret;
}
