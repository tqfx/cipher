/*!
 @file c_word.h
 @brief cipher word
 @copyright Copyright (C) 2020-present tqfx, All rights reserved.
*/

#ifndef __C_WORD_H__
#define __C_WORD_H__

#include "output.h"
#include "str.h"

#define c_word_forenum(i, ctx) for (size_t i = 0; i != CAST(size_t, (ctx)->tail - (ctx)->head); ++i)

#define c_word_forenum_reverse(i, ctx) for (size_t i = CAST(size_t, (ctx)->tail - (ctx)->head); i--;)

#define c_word_foreach(it, ctx) for (str_s *it = (ctx)->head, *it##_ = (ctx)->tail; it != it##_; ++it)

#define c_word_foreach_reverse(it, ctx)                       \
    for (str_s *it##_ = (ctx)->head ? (ctx)->head - 1 : NULL, \
               *it = (ctx)->tail ? (ctx)->tail - 1 : NULL;    \
         it != it##_; --it)

typedef struct c_word_s
{
    str_s *head;
    str_s *tail;
    str_s *last;
} c_word_s;

static inline str_s *c_word_ptr(const c_word_s *ctx) { return ctx->head; }

static inline str_s *c_word_end(const c_word_s *ctx) { return ctx->tail; }

static inline str_s *c_word_at(const c_word_s *ctx, size_t idx)
{
    return ctx->head + idx;
}

static inline size_t c_word_num(const c_word_s *ctx)
{
    return CAST(size_t, ctx->tail - ctx->head);
}

static inline size_t c_word_mem(const c_word_s *ctx)
{
    return CAST(size_t, ctx->last - ctx->head);
}

static inline str_s *c_word_top(const c_word_s *ctx)
{
    return ctx->head != ctx->tail ? ctx->tail - 1 : NULL;
}

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

c_word_s *c_word_new(void);

void c_word_die(c_word_s *ctx);

void c_word_ctor(c_word_s *ctx);

void c_word_dtor(c_word_s *ctx);

int c_word_copy(c_word_s *ctx, const c_word_s *obj);

c_word_s *c_word_move(c_word_s *ctx, c_word_s *obj);

void c_word_drop(c_word_s *ctx);

int c_word_swap(c_word_s *ctx, size_t lhs, size_t rhs);

str_s *c_word_insert(c_word_s *ctx, size_t idx);

str_s *c_word_push_fore(c_word_s *ctx);

str_s *c_word_push_back(c_word_s *ctx);

str_s *c_word_remove(c_word_s *ctx, size_t idx);

str_s *c_word_pop_fore(c_word_s *ctx);

str_s *c_word_pop_back(c_word_s *ctx);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* __cplusplus */

static inline str_s *c_word_push(c_word_s *ctx)
{
    str_s *obj = c_word_push_back(ctx);
    str_ctor(obj);
    return obj;
}

static inline str_s *c_word_pop(c_word_s *ctx)
{
    return c_word_pop_back(ctx);
}

#endif /* __C_WORD_H__ */
