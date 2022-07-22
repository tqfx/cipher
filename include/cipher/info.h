/*!
 @file info.h
 @brief cipher infomation
 @copyright Copyright (C) 2020-present tqfx, All rights reserved.
*/

#ifndef __CIPHER_INFO_H__
#define __CIPHER_INFO_H__

#include "a/info.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

int c_info_add(c_info_s *ctx, cipher_s *obj);
int c_info_del(c_info_s *ctx, cipher_s *obj);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* __CIPHER_INFO_H__ */
