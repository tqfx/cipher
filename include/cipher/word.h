/*!
 @file word.h
 @brief cipher word
 @copyright Copyright (C) 2020-present tqfx, All rights reserved.
*/

#ifndef __CIPHER_WORD_H__
#define __CIPHER_WORD_H__

#include "a/word.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

int c_word_add(c_word_s *ctx, str_s *obj);
int c_word_del(c_word_s *ctx, str_s *obj);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* __CIPHER_WORD_H__ */
