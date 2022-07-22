/*!
 @file app.h
 @brief application
 @copyright Copyright (C) 2020-present tqfx, All rights reserved.
*/

#ifndef __APP_H__
#define __APP_H__

#include "cipher/sqlite.h"

#include "console.h"
#include "convert.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

void app_log(uint_t n, ...);
int app_exec(const cipher_s *info, cstr_t word);

int app_init(cstr_t fname);
int app_exit(void);

void app_search_word_str(cptr_t word);
void app_search_info_str(cptr_t info);
void app_search_word_idx(const c_word_s *word);
void app_search_info_idx(const c_info_s *info);

int app_create_word(const c_word_s *word);
int app_create_info(const c_info_s *info);

int app_delete_word(const c_word_s *word);
int app_delete_info(const c_info_s *info);
int app_delete_word_idx(const c_word_s *word);
int app_delete_info_idx(const c_info_s *info);

int app_exec_idx(size_t word, size_t info);
int app_exec_ctx(const cipher_s *ctx);

int app_convert(cstr_t in, cstr_t out);
int app_import(cstr_t fname);
int app_export(cstr_t fname);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

static const char s_missing[] = "missing";
static const char s_failure[] = "failure";
static const char s_success[] = "success";
static const char s_warning[] = "warning";

#endif /* __APP_H__ */
