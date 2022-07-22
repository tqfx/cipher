/*!
 @file info.c
 @brief cipher infomation
 @copyright Copyright (C) 2020-present tqfx, All rights reserved.
*/

#include "cipher/a/info.h"

static inline cipher_s *c_info_inc_(c_info_s *ctx) { return ctx->tail++; }
static inline cipher_s *c_info_dec_(c_info_s *ctx) { return --ctx->tail; }

static inline size_t c_info_num_(const c_info_s *ctx)
{
    return (size_t)(ctx->tail - ctx->head);
}
static inline size_t c_info_mem_(const c_info_s *ctx)
{
    return (size_t)(ctx->last - ctx->head);
}

c_info_s *c_info_new(void)
{
    c_info_s *ctx = (c_info_s *)malloc(sizeof(c_info_s));
    if (ctx)
    {
        c_info_ctor(ctx);
    }
    return ctx;
}

void c_info_die(c_info_s *ctx)
{
    if (ctx)
    {
        c_info_dtor(ctx);
        free(ctx);
    }
}

void c_info_ctor(c_info_s *ctx)
{
    ctx->head = NULL;
    ctx->tail = NULL;
    ctx->last = NULL;
}

void c_info_dtor(c_info_s *ctx)
{
    if (ctx->head)
    {
        c_info_drop(ctx);
        free(ctx->head);
        ctx->head = NULL;
    }
    ctx->tail = NULL;
    ctx->last = NULL;
}

int c_info_copy(c_info_s *ctx, const c_info_s *obj)
{
    size_t num = c_info_num_(obj);
    size_t mem = c_info_mem_(obj);
    ctx->head = (cipher_s *)malloc(sizeof(cipher_s) * mem);
    if (ctx->head == NULL)
    {
        return FAILURE;
    }
    ctx->tail = ctx->head + num;
    ctx->last = ctx->head + mem;
    cipher_s *dst = ctx->head;
    cipher_s *src = obj->head;
    for (; src != obj->last; ++dst, ++src)
    {
        cipher_copy(dst, src);
    }
    return SUCCESS;
}

c_info_s *c_info_move(c_info_s *ctx, c_info_s *obj)
{
    memcpy(ctx, obj, sizeof(c_info_s));
    memset(obj, 000, sizeof(c_info_s));
    return ctx;
}

void c_info_drop(c_info_s *ctx)
{
    while (ctx->head != ctx->tail)
    {
        --ctx->tail;
        cipher_dtor(ctx->tail);
    }
}

#undef align
#define align(x) ((sizeof(cipher_s) * (x) + sizeof(void *) - 1) & ~(sizeof(void *) - 1))

static int c_info_alloc(c_info_s *ctx, size_t num)
{
    size_t mem = c_info_mem_(ctx);
    if (mem <= num)
    {
        size_t n = c_info_num_(ctx);
        do
        {
            mem += (mem >> 1) + 1;
        } while (mem < num);
        cipher_s *head = (cipher_s *)realloc(ctx->head, align(mem));
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

int c_info_swap(c_info_s *ctx, size_t lhs, size_t rhs)
{
    const size_t num = c_info_num_(ctx);
    lhs = lhs < num ? lhs : num - 1;
    rhs = rhs < num ? rhs : num - 1;
    if (lhs == rhs)
    {
        return SUCCESS;
    }
    if (c_info_alloc(ctx, num))
    {
        return FAILURE;
    }
    memcpy(ctx->head + num, ctx->head + lhs, sizeof(cipher_s));
    memcpy(ctx->head + lhs, ctx->head + rhs, sizeof(cipher_s));
    memcpy(ctx->head + rhs, ctx->head + num, sizeof(cipher_s));
    return SUCCESS;
}

cipher_s *c_info_insert(c_info_s *ctx, size_t idx)
{
    size_t num = c_info_num_(ctx);
    if (c_info_alloc(ctx, num))
    {
        return NULL;
    }
    if (idx < num)
    {
        cipher_s *ptr = ctx->tail;
        cipher_s *src = ctx->head + idx + 0;
        cipher_s *dst = ctx->head + idx + 1;
        memmove(dst, src, sizeof(cipher_s) * (size_t)(ptr - src));
        c_info_inc_(ctx);
        return src;
    }
    return c_info_inc_(ctx);
}

cipher_s *c_info_push_front(c_info_s *ctx)
{
    return c_info_insert(ctx, 0);
}

cipher_s *c_info_push_back(c_info_s *ctx)
{
    size_t num = c_info_num_(ctx);
    if (c_info_alloc(ctx, num))
    {
        return NULL;
    }
    return c_info_inc_(ctx);
}

cipher_s *c_info_remove(c_info_s *ctx, size_t idx)
{
    size_t num = c_info_num_(ctx);
    if (idx + 1 < num)
    {
        if (c_info_alloc(ctx, num + 1))
        {
            return NULL;
        }
        cipher_s *ptr = ctx->tail;
        cipher_s *dst = ctx->head + idx + 0;
        cipher_s *src = ctx->head + idx + 1;
        memcpy(ptr, dst, sizeof(cipher_s));
        memmove(dst, src, sizeof(cipher_s) * (size_t)(ptr - src));
        c_info_dec_(ctx);
        return ptr;
    }
    return ctx->head != ctx->tail ? c_info_inc_(ctx) : NULL;
}

cipher_s *c_info_pop_front(c_info_s *ctx)
{
    return c_info_remove(ctx, 0);
}

cipher_s *c_info_pop_back(c_info_s *ctx)
{
    return ctx->head != ctx->tail ? c_info_dec_(ctx) : NULL;
}
