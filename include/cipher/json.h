/*!
 @file json.h
 @brief cipher json
*/

#ifndef CIPHER_JSON_H
#define CIPHER_JSON_H

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

int c_json_load(cJSON **out, char const *in);
int c_json_export_info(cJSON const *in, c_info_s *out);
int c_json_import_info(cJSON **out, c_info_s const *in);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* __cplusplus */

#endif /* CIPHER_JSON_H */
