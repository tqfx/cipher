/*!
 @file rule.h
 @brief cipher rule
 @copyright Copyright (C) 2020-present tqfx, All rights reserved.
*/

#ifndef __C_RULE_H__
#define __C_RULE_H__

#include "output.h"
#include "str.h"

#include <stdlib.h>
#include <string.h>

#define c_rule_forenum(i, ctx) for (size_t i = 0; i != CAST(size_t, (ctx)->tail - (ctx)->head); ++i)

#define c_rule_forenum_reverse(i, ctx) for (size_t i = CAST(size_t, (ctx)->tail - (ctx)->head); i--;)

#define c_rule_foreach(it, ctx) for (rule_s *it = (ctx)->head, *it##_ = (ctx)->tail; it != it##_; ++it)

#define c_rule_foreach_reverse(it, ctx)                        \
    for (rule_s *it##_ = (ctx)->head ? (ctx)->head - 1 : NULL, \
                *it = (ctx)->tail ? (ctx)->tail - 1 : NULL;    \
         it != it##_; --it)

typedef struct rule_s
{
    str_s r[4];
} rule_s;

typedef struct c_rule_s
{
    rule_s *head;
    rule_s *tail;
    rule_s *last;
} c_rule_s;

static inline rule_s *c_rule_ptr(const c_rule_s *ctx) { return ctx->head; }

static inline rule_s *c_rule_end(const c_rule_s *ctx) { return ctx->tail; }

static inline rule_s *c_rule_at(const c_rule_s *ctx, size_t idx)
{
    return ctx->head + idx;
}

static inline size_t c_rule_num(const c_rule_s *ctx)
{
    return CAST(size_t, ctx->tail - ctx->head);
}

static inline size_t c_rule_mem(const c_rule_s *ctx)
{
    return CAST(size_t, ctx->last - ctx->head);
}

static inline rule_s *c_rule_top(const c_rule_s *ctx)
{
    return ctx->head != ctx->tail ? ctx->tail - 1 : NULL;
}

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

rule_s *rule_new(void);
void rule_die(rule_s *ctx);
void rule_ctor(rule_s *ctx);
void rule_dtor(rule_s *ctx);

int rule_copy(rule_s *ctx, const rule_s *obj);
rule_s *rule_move(rule_s *ctx, rule_s *obj);

c_rule_s *c_rule_new(void);

void c_rule_die(c_rule_s *ctx);

void c_rule_ctor(c_rule_s *ctx);

void c_rule_dtor(c_rule_s *ctx);

int c_rule_copy(c_rule_s *ctx, const c_rule_s *obj);

c_rule_s *c_rule_move(c_rule_s *ctx, c_rule_s *obj);

void c_rule_drop(c_rule_s *ctx);

int c_rule_swap(c_rule_s *ctx, size_t lhs, size_t rhs);

rule_s *c_rule_insert(c_rule_s *ctx, size_t idx);

rule_s *c_rule_push_front(c_rule_s *ctx);

rule_s *c_rule_push_back(c_rule_s *ctx);

rule_s *c_rule_remove(c_rule_s *ctx, size_t idx);

rule_s *c_rule_pop_front(c_rule_s *ctx);

rule_s *c_rule_pop_back(c_rule_s *ctx);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* __cplusplus */

static inline rule_s *c_rule_push(c_rule_s *ctx)
{
    rule_s *obj = c_rule_push_back(ctx);
    rule_ctor(obj);
    return obj;
}

static inline rule_s *c_rule_pop(c_rule_s *ctx)
{
    return c_rule_pop_back(ctx);
}

#endif /* __C_RULE_H__ */
