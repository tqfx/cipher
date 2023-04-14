/*!
 @file info.h
 @brief cipher infomation
*/

#ifndef CIPHER_INFO_H
#define CIPHER_INFO_H

#include "a/host/a.h"
#include "cipher.h"
#include "a/avl.h"

// clang-format off
#define C_INFO_INIT {{A_AVL_ROOT}, 0}
// clang-format on

typedef struct c_info_s
{
    a_avl_u root[1];
    a_size_t count;
} c_info_s;

typedef struct info_s
{
    a_avl_s node[1];
    a_i64_t time;
    cipher_s cipher[1];
} info_s;

#define c_info_num(ctx) (ctx)->count

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

void c_info_ctor(c_info_s *ctx);
void c_info_dtor(c_info_s *ctx);

info_s *c_info_at(c_info_s *ctx, size_t idx);
info_s *c_info_add(c_info_s *ctx, void const *text);
info_s *c_info_del(c_info_s *ctx, void const *text);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* __cplusplus */

#endif /* CIPHER_INFO_H */
