/*!
 @file word.h
 @brief cipher word
 @copyright Copyright (C) 2020 tqfx, All rights reserved.
*/

#ifndef __CIPHER_WORD_H__
#define __CIPHER_WORD_H__

#include "str.h"

typedef struct cipher_word_s
{
    size_t mem;
    size_t num;
    str_s *vec;
} cipher_word_s;

#define cipher_word_num(ctx) (ctx)->num
#define cipher_word_at(ctx, index) ((ctx)->vec + index)
#define cipher_word_top(ctx) ((ctx)->num ? (ctx)->vec + (ctx)->num - 1 : 0)

#define cipher_word_forenum(i, ctx) for (size_t i = 0; i != (ctx)->num; ++i)
#define cipher_word_forenum_reverse(i, ctx) for (size_t i = (ctx)->num; i--;)

#define cipher_word_foreach(it, ctx) \
    for (str_s *it = (ctx)->vec, *it##_ = it + (ctx)->num; it != it##_; ++it)
#define cipher_word_foreach_reverse(it, ctx) \
    for (str_s *it##_ = (ctx)->vec - 1, *it = it##_ + (ctx)->num; it != it##_; --it)

#define cipher_word_forboth(i, it, ctx) \
    for (size_t i = 0; (it) = (ctx)->vec + i, i != (ctx)->num; ++i)
#define cipher_word_forboth_reverse(i, it, ctx) \
    for (size_t i = (ctx)->num; i ? ((it) = (ctx)->vec + --i) : 0;)

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

cipher_word_s *cipher_word_new(void);
void cipher_word_die(cipher_word_s *ctx);

void cipher_word_ctor(cipher_word_s *ctx);
void cipher_word_dtor(cipher_word_s *ctx);

str_s *cipher_word_pop(cipher_word_s *ctx);
str_s *cipher_word_push(cipher_word_s *ctx);

int cipher_word_add(cipher_word_s *ctx, str_s *obj);
int cipher_word_del(cipher_word_s *ctx, str_s *obj);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* __CIPHER_WORD_H__ */
