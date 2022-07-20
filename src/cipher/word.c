/*!
 @file word.c
 @brief cipher word
 @copyright Copyright (C) 2020 tqfx, All rights reserved.
*/

#include "cipher/word.h"

void cipher_word_ctor(cipher_word_s *ctx)
{
    ctx->mem = 0;
    ctx->num = 0;
    ctx->vec = 0;
}

void cipher_word_dtor(cipher_word_s *ctx)
{
    if (ctx->vec)
    {
        while (ctx->num)
        {
            --ctx->num;
            str_dtor(ctx->vec + ctx->num);
        }
        free(ctx->vec);
        ctx->vec = 0;
    }
    ctx->num = 0;
    ctx->mem = 0;
}

cipher_word_s *cipher_word_new(void)
{
    cipher_word_s *ctx = (cipher_word_s *)malloc(sizeof(cipher_word_s));
    if (ctx)
    {
        cipher_word_ctor(ctx);
    }
    return ctx;
}

void cipher_word_die(cipher_word_s *ctx)
{
    if (ctx)
    {
        cipher_word_dtor(ctx);
        free(ctx);
    }
}

str_s *cipher_word_push(cipher_word_s *ctx)
{
    if (ctx->mem <= ctx->num)
    {
        size_t mem = ctx->mem + (ctx->mem >> 1) + 1;
        str_s *vec = (str_s *)realloc(ctx->vec, sizeof(str_s) * mem);
        if (vec == 0)
        {
            return 0;
        }
        ctx->mem = mem;
        ctx->vec = vec;
    }
    str_s *obj = ctx->vec + ctx->num++;
    str_ctor(obj);
    return obj;
}

str_s *cipher_word_pop(cipher_word_s *ctx) { return ctx->num ? ctx->vec + --ctx->num : 0; }

int cipher_word_add(cipher_word_s *ctx, str_s *obj)
{
    str_s *it = 0;
    cipher_word_forenum(i, ctx)
    {
        it = cipher_word_at(ctx, i);
        if (str_val(it) && strcmp(str_val(it), str_val(obj)) == 0)
        {
            str_dtor(it);
            break;
        }
        it = 0;
    }
    if (it == 0)
    {
        it = cipher_word_push(ctx);
        str_ctor(it);
        if (it == 0)
        {
            return ~0;
        }
    }
    return str_copy(it, obj);
}

int cipher_word_del(cipher_word_s *ctx, str_s *obj)
{
    int ret = ~0;
    cipher_word_foreach(it, ctx)
    {
        if (str_val(it) && strcmp(str_val(it), str_val(obj)) == 0)
        {
            str_dtor(it);
            ret = 0;
        }
    }
    return ret;
}
