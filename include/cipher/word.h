/*!
 @file word.h
 @brief cipher word
*/

#ifndef CIPHER_WORD_H
#define CIPHER_WORD_H

#include "a/host/vec.h"
#include "a/host/str.h"

typedef a_vec_s c_word_s;

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

c_word_s *c_word_new(void);
void c_word_die(c_word_s *ctx);
void c_word_ctor(c_word_s *ctx);
void c_word_dtor(c_word_s *ctx);

int c_word_add(c_word_s *ctx, a_str_s *obj);
int c_word_del(c_word_s *ctx, a_str_s *obj);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* __cplusplus */

static inline a_str_s *c_word_push(c_word_s *ctx)
{
    a_str_s *obj = a_vec_push(a_str_s, ctx);
    a_str_ctor(obj);
    return obj;
}

static inline a_str_s *c_word_pull(c_word_s *ctx)
{
    return a_vec_pull(a_str_s, ctx);
}

#endif /* CIPHER_WORD_H */
