/*!
 @file rule.h
 @brief rule rule
 @copyright Copyright (C) 2020-present tqfx, All rights reserved.
*/

#ifndef __CIPHER_RULE_H__
#define __CIPHER_RULE_H__

#include "a/rule.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

int c_rule_add(c_rule_s *ctx, rule_s *obj);
int c_rule_del(c_rule_s *ctx, rule_s *obj);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* __CIPHER_RULE_H__ */
