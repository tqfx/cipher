/*!
 @file info.h
 @brief cipher infomation
 @copyright Copyright (C) 2020 tqfx, All rights reserved.
*/

#ifndef __CIPHER_INFO_H__
#define __CIPHER_INFO_H__

#include "cipher.h"

typedef struct cipher_info_s
{
    size_t mem;
    size_t num;
    cipher_s *vec;
} cipher_info_s;

#define cipher_info_num(ctx) (ctx)->num
#define cipher_info_at(ctx, index) ((ctx)->vec + index)
#define cipher_info_top(ctx) ((ctx)->num ? (ctx)->vec + (ctx)->num - 1 : 0)

#define cipher_info_forenum(i, ctx) for (size_t i = 0; i != (ctx)->num; ++i)
#define cipher_info_forenum_reverse(i, ctx) for (size_t i = (ctx)->num; i--;)

#define cipher_info_foreach(it, ctx) \
    for (cipher_s *it = (ctx)->vec, *it##_ = it + (ctx)->num; it != it##_; ++it)
#define cipher_info_foreach_reverse(it, ctx) \
    for (cipher_s *it##_ = (ctx)->vec - 1, *it = it##_ + (ctx)->num; it != it##_; --it)

#define cipher_info_forboth(i, it, ctx) \
    for (size_t i = 0; (it) = (ctx)->vec + i, i != (ctx)->num; ++i)
#define cipher_info_forboth_reverse(i, it, ctx) \
    for (size_t i = (ctx)->num; i ? ((it) = (ctx)->vec + --i) : 0;)

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

cipher_info_s *cipher_info_new(void);
void cipher_info_die(cipher_info_s *ctx);

void cipher_info_ctor(cipher_info_s *ctx);
void cipher_info_dtor(cipher_info_s *ctx);

cipher_s *cipher_info_pop(cipher_info_s *ctx);
cipher_s *cipher_info_push(cipher_info_s *ctx);

int cipher_info_add(cipher_info_s *ctx, cipher_s *obj);
int cipher_info_del(cipher_info_s *ctx, cipher_s *obj);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* __CIPHER_INFO_H__ */
