/*!
 @file json.h
 @brief cipher json
 @copyright Copyright (C) 2020 tqfx, All rights reserved.
*/

#ifndef __CIPHER_JSON_H__
#define __CIPHER_JSON_H__

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
#endif /* __GNUC__ || __clang__ */
#include "cjson/cJSON.h"
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif /* __GNUC__ || __clang__ */

#include "info.h"
#include "word.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

int cipher_json_load(cJSON **out, const char *in);
int cipher_json_export_info(const cJSON *in, cipher_info_s *out);
int cipher_json_import_info(cJSON **out, const cipher_info_s *in);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* __CIPHER_JSON_H__ */
