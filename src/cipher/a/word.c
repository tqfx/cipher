/*!
 @file word.c
 @brief cipher word
 @copyright Copyright (C) 2020-present tqfx, All rights reserved.
*/

#include "cipher/a/word.h"

static inline str_s *c_word_inc_(c_word_s *ctx) { return ctx->tail++; }
static inline str_s *c_word_dec_(c_word_s *ctx) { return --ctx->tail; }

static inline size_t c_word_num_(const c_word_s *ctx)
{
    return (size_t)(ctx->tail - ctx->head);
}
static inline size_t c_word_mem_(const c_word_s *ctx)
{
    return (size_t)(ctx->last - ctx->head);
}

c_word_s *c_word_new(void)
{
    c_word_s *ctx = (c_word_s *)malloc(sizeof(c_word_s));
    if (ctx)
    {
        c_word_ctor(ctx);
    }
    return ctx;
}

void c_word_die(c_word_s *ctx)
{
    if (ctx)
    {
        c_word_dtor(ctx);
        free(ctx);
    }
}

void c_word_ctor(c_word_s *ctx)
{
    ctx->head = NULL;
    ctx->tail = NULL;
    ctx->last = NULL;
}

void c_word_dtor(c_word_s *ctx)
{
    if (ctx->head)
    {
        c_word_drop(ctx);
        free(ctx->head);
        ctx->head = NULL;
    }
    ctx->tail = NULL;
    ctx->last = NULL;
}

int c_word_copy(c_word_s *ctx, const c_word_s *obj)
{
    size_t num = c_word_num_(obj);
    size_t mem = c_word_mem_(obj);
    ctx->head = (str_s *)malloc(sizeof(str_s) * mem);
    if (ctx->head == NULL)
    {
        return FAILURE;
    }
    ctx->tail = ctx->head + num;
    ctx->last = ctx->head + mem;
    str_s *dst = ctx->head;
    str_s *src = obj->head;
    for (; src != obj->last; ++dst, ++src)
    {
        str_copy(dst, src);
    }
    return SUCCESS;
}

c_word_s *c_word_move(c_word_s *ctx, c_word_s *obj)
{
    memcpy(ctx, obj, sizeof(c_word_s));
    memset(obj, 000, sizeof(c_word_s));
    return ctx;
}

void c_word_drop(c_word_s *ctx)
{
    while (ctx->head != ctx->tail)
    {
        --ctx->tail;
        str_dtor(ctx->tail);
    }
}

#undef align
#define align(x) ((sizeof(str_s) * (x) + sizeof(void *) - 1) & ~(sizeof(void *) - 1))

static int c_word_alloc(c_word_s *ctx, size_t num)
{
    size_t mem = c_word_mem_(ctx);
    if (mem <= num)
    {
        size_t n = c_word_num_(ctx);
        do
        {
            mem += (mem >> 1) + 1;
        } while (mem < num);
        str_s *head = (str_s *)realloc(ctx->head, align(mem));
        if (head == NULL)
        {
            return FAILURE;
        }
        ctx->head = head;
        ctx->tail = head + n;
        ctx->last = head + mem;
    }
    return SUCCESS;
}

#undef align

int c_word_swap(c_word_s *ctx, size_t lhs, size_t rhs)
{
    const size_t num = c_word_num_(ctx);
    lhs = lhs < num ? lhs : num - 1;
    rhs = rhs < num ? rhs : num - 1;
    if (lhs == rhs)
    {
        return SUCCESS;
    }
    if (c_word_alloc(ctx, num))
    {
        return FAILURE;
    }
    memcpy(ctx->head + num, ctx->head + lhs, sizeof(str_s));
    memcpy(ctx->head + lhs, ctx->head + rhs, sizeof(str_s));
    memcpy(ctx->head + rhs, ctx->head + num, sizeof(str_s));
    return SUCCESS;
}

str_s *c_word_insert(c_word_s *ctx, size_t idx)
{
    size_t num = c_word_num_(ctx);
    if (c_word_alloc(ctx, num))
    {
        return NULL;
    }
    if (idx < num)
    {
        str_s *ptr = ctx->tail;
        str_s *src = ctx->head + idx + 0;
        str_s *dst = ctx->head + idx + 1;
        memmove(dst, src, sizeof(str_s) * (size_t)(ptr - src));
        c_word_inc_(ctx);
        return src;
    }
    return c_word_inc_(ctx);
}

str_s *c_word_push_front(c_word_s *ctx)
{
    return c_word_insert(ctx, 0);
}

str_s *c_word_push_back(c_word_s *ctx)
{
    size_t num = c_word_num_(ctx);
    if (c_word_alloc(ctx, num))
    {
        return NULL;
    }
    return c_word_inc_(ctx);
}

str_s *c_word_remove(c_word_s *ctx, size_t idx)
{
    size_t num = c_word_num_(ctx);
    if (idx + 1 < num)
    {
        if (c_word_alloc(ctx, num + 1))
        {
            return NULL;
        }
        str_s *ptr = ctx->tail;
        str_s *dst = ctx->head + idx + 0;
        str_s *src = ctx->head + idx + 1;
        memcpy(ptr, dst, sizeof(str_s));
        memmove(dst, src, sizeof(str_s) * (size_t)(ptr - src));
        c_word_dec_(ctx);
        return ptr;
    }
    return ctx->head != ctx->tail ? c_word_inc_(ctx) : NULL;
}

str_s *c_word_pop_front(c_word_s *ctx)
{
    return c_word_remove(ctx, 0);
}

str_s *c_word_pop_back(c_word_s *ctx)
{
    return ctx->head != ctx->tail ? c_word_dec_(ctx) : NULL;
}
