#ifndef APP_H
#define APP_H

#include "cipher/sqlite.h"

#include "console.h"
#include "convert.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

void app_log(unsigned int n, ...);
int app_exec(cipher_s const *info, char const *word);

int app_init(char const *fname);
int app_exit(void);

void app_search_word_str(void const *word);
void app_search_info_str(void const *info);
void app_search_word_idx(a_vec_s const *word);
void app_search_info_idx(a_vec_s const *info);

int app_create_word(a_vec_s const *word);
int app_create_info(a_vec_s const *info);

int app_delete_word(a_vec_s const *word);
int app_delete_info(a_vec_s const *info);
int app_delete_word_idx(a_vec_s const *word);
int app_delete_info_idx(a_vec_s const *info);

int app_exec_idx(size_t word, size_t info);
int app_exec_ctx(cipher_s const *ctx);

int app_convert(char const *in, char const *out);
int app_import(char const *fname);
int app_export(char const *fname);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* __cplusplus */

static const char s_missing[] = "missing";
static const char s_failure[] = "failure";
static const char s_success[] = "success";
static const char s_warning[] = "warning";

#endif /* APP_H */
