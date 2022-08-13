/*!
 @file info.h
 @brief cipher infomation
 @copyright Copyright (C) 2020-present tqfx, All rights reserved.
*/

#ifndef __C_INFO_H__
#define __C_INFO_H__

#include "../cipher.h"

#include <string.h>

#define c_info_forenum(i, ctx) for (size_t i = 0; i != CAST(size_t, (ctx)->tail - (ctx)->head); ++i)

#define c_info_forenum_reverse(i, ctx) for (size_t i = CAST(size_t, (ctx)->tail - (ctx)->head); i--;)

#define c_info_foreach(it, ctx) for (cipher_s *it = (ctx)->head, *it##_ = (ctx)->tail; it != it##_; ++it)

#define c_info_foreach_reverse(it, ctx)                          \
    for (cipher_s *it##_ = (ctx)->head ? (ctx)->head - 1 : NULL, \
                  *it = (ctx)->tail ? (ctx)->tail - 1 : NULL;    \
         it != it##_; --it)

typedef struct c_info_s
{
    cipher_s *head;
    cipher_s *tail;
    cipher_s *last;
} c_info_s;

static inline cipher_s *c_info_ptr(const c_info_s *ctx) { return ctx->head; }

static inline cipher_s *c_info_end(const c_info_s *ctx) { return ctx->tail; }

static inline cipher_s *c_info_at(const c_info_s *ctx, size_t idx)
{
    return ctx->head + idx;
}

static inline size_t c_info_num(const c_info_s *ctx)
{
    return CAST(size_t, ctx->tail - ctx->head);
}

static inline size_t c_info_mem(const c_info_s *ctx)
{
    return CAST(size_t, ctx->last - ctx->head);
}

static inline cipher_s *c_info_top(const c_info_s *ctx)
{
    return ctx->head != ctx->tail ? ctx->tail - 1 : NULL;
}

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

c_info_s *c_info_new(void);

void c_info_die(c_info_s *ctx);

void c_info_ctor(c_info_s *ctx);

void c_info_dtor(c_info_s *ctx);

int c_info_copy(c_info_s *ctx, const c_info_s *obj);

c_info_s *c_info_move(c_info_s *ctx, c_info_s *obj);

void c_info_drop(c_info_s *ctx);

int c_info_swap(c_info_s *ctx, size_t lhs, size_t rhs);

cipher_s *c_info_insert(c_info_s *ctx, size_t idx);

cipher_s *c_info_push_fore(c_info_s *ctx);

cipher_s *c_info_push_back(c_info_s *ctx);

cipher_s *c_info_remove(c_info_s *ctx, size_t idx);

cipher_s *c_info_pop_fore(c_info_s *ctx);

cipher_s *c_info_pop_back(c_info_s *ctx);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* __cplusplus */

static inline cipher_s *c_info_push(c_info_s *ctx)
{
    cipher_s *obj = c_info_push_back(ctx);
    cipher_ctor(obj);
    return obj;
}

static inline cipher_s *c_info_pop(c_info_s *ctx)
{
    return c_info_pop_back(ctx);
}

#endif /* __C_INFO_H__ */
