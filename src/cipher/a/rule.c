/*!
 @file rule.c
 @brief cipher rule
 @copyright Copyright (C) 2020-present tqfx, All rights reserved.
*/

#include "cipher/a/rule.h"

static inline rule_s *c_rule_inc_(c_rule_s *ctx) { return ctx->tail++; }
static inline rule_s *c_rule_dec_(c_rule_s *ctx) { return --ctx->tail; }

static inline size_t c_rule_num_(const c_rule_s *ctx)
{
    return (size_t)(ctx->tail - ctx->head);
}
static inline size_t c_rule_mem_(const c_rule_s *ctx)
{
    return (size_t)(ctx->last - ctx->head);
}

c_rule_s *c_rule_new(void)
{
    c_rule_s *ctx = (c_rule_s *)malloc(sizeof(c_rule_s));
    if (ctx)
    {
        c_rule_ctor(ctx);
    }
    return ctx;
}

void c_rule_die(c_rule_s *ctx)
{
    if (ctx)
    {
        c_rule_dtor(ctx);
        free(ctx);
    }
}

void c_rule_ctor(c_rule_s *ctx)
{
    ctx->head = NULL;
    ctx->tail = NULL;
    ctx->last = NULL;
}

void c_rule_dtor(c_rule_s *ctx)
{
    if (ctx->head)
    {
        c_rule_drop(ctx);
        free(ctx->head);
        ctx->head = NULL;
    }
    ctx->tail = NULL;
    ctx->last = NULL;
}

int c_rule_copy(c_rule_s *ctx, const c_rule_s *obj)
{
    size_t num = c_rule_num_(obj);
    size_t mem = c_rule_mem_(obj);
    ctx->head = (rule_s *)malloc(sizeof(rule_s) * mem);
    if (ctx->head == NULL)
    {
        return FAILURE;
    }
    ctx->tail = ctx->head + num;
    ctx->last = ctx->head + mem;
#if defined(rule_copy)
    rule_s *dst = ctx->head;
    rule_s *src = obj->head;
    for (; src != obj->last; ++dst, ++src)
    {
        rule_copy(dst, src);
    }
#else /* !rule_copy */
    memcpy(ctx->head, obj->head, sizeof(rule_s) * mem);
#endif /* rule_copy */
    return SUCCESS;
}

c_rule_s *c_rule_move(c_rule_s *ctx, c_rule_s *obj)
{
    memcpy(ctx, obj, sizeof(c_rule_s));
    memset(obj, 000, sizeof(c_rule_s));
    return ctx;
}

void c_rule_drop(c_rule_s *ctx)
{
#if defined(rule_dtor)
    while (ctx->head != ctx->tail)
    {
        --ctx->tail;
        rule_dtor(ctx->tail);
    }
#else /* !rule_dtor */
    ctx->tail = ctx->head;
#endif /* rule_dtor */
}

#undef align
#define align(x) ((sizeof(rule_s) * (x) + sizeof(void *) - 1) & ~(sizeof(void *) - 1))

static int c_rule_alloc(c_rule_s *ctx, size_t num)
{
    size_t mem = c_rule_mem_(ctx);
    if (mem <= num)
    {
        size_t n = c_rule_num_(ctx);
        do
        {
            mem += (mem >> 1) + 1;
        } while (mem < num);
        rule_s *head = (rule_s *)realloc(ctx->head, align(mem));
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

int c_rule_swap(c_rule_s *ctx, size_t lhs, size_t rhs)
{
    const size_t num = c_rule_num_(ctx);
    lhs = lhs < num ? lhs : num - 1;
    rhs = rhs < num ? rhs : num - 1;
    if (lhs == rhs)
    {
        return SUCCESS;
    }
    if (c_rule_alloc(ctx, num))
    {
        return FAILURE;
    }
    memcpy(ctx->head + num, ctx->head + lhs, sizeof(rule_s));
    memcpy(ctx->head + lhs, ctx->head + rhs, sizeof(rule_s));
    memcpy(ctx->head + rhs, ctx->head + num, sizeof(rule_s));
    return SUCCESS;
}

rule_s *c_rule_insert(c_rule_s *ctx, size_t idx)
{
    size_t num = c_rule_num_(ctx);
    if (c_rule_alloc(ctx, num))
    {
        return NULL;
    }
    if (idx < num)
    {
        rule_s *ptr = ctx->tail;
        rule_s *src = ctx->head + idx + 0;
        rule_s *dst = ctx->head + idx + 1;
        memmove(dst, src, sizeof(rule_s) * (size_t)(ptr - src));
        c_rule_inc_(ctx);
        return src;
    }
    return c_rule_inc_(ctx);
}

rule_s *c_rule_push_front(c_rule_s *ctx)
{
    return c_rule_insert(ctx, 0);
}

rule_s *c_rule_push_back(c_rule_s *ctx)
{
    size_t num = c_rule_num_(ctx);
    if (c_rule_alloc(ctx, num))
    {
        return NULL;
    }
    return c_rule_inc_(ctx);
}

rule_s *c_rule_remove(c_rule_s *ctx, size_t idx)
{
    size_t num = c_rule_num_(ctx);
    if (idx + 1 < num)
    {
        if (c_rule_alloc(ctx, num + 1))
        {
            return NULL;
        }
        rule_s *ptr = ctx->tail;
        rule_s *dst = ctx->head + idx + 0;
        rule_s *src = ctx->head + idx + 1;
        memcpy(ptr, dst, sizeof(rule_s));
        memmove(dst, src, sizeof(rule_s) * (size_t)(ptr - src));
        c_rule_dec_(ctx);
        return ptr;
    }
    return ctx->head != ctx->tail ? c_rule_inc_(ctx) : NULL;
}

rule_s *c_rule_pop_front(c_rule_s *ctx)
{
    return c_rule_remove(ctx, 0);
}

rule_s *c_rule_pop_back(c_rule_s *ctx)
{
    return ctx->head != ctx->tail ? c_rule_dec_(ctx) : NULL;
}
