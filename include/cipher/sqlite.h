/*!
 @file sqlite.h
 @brief cipher sqlite
 @copyright Copyright (C) 2020 tqfx, All rights reserved.
*/

#ifndef __CIPHER_SQLITE_H__
#define __CIPHER_SQLITE_H__

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#if defined(__clang__)
#pragma GCC diagnostic ignored "-Wlanguage-extension-token"
#endif /* __clang__ */
#endif /* __GNUC__ || __clang__ */
#include "sqlite3.h"
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif /* __GNUC__ || __clang__ */

#include "info.h"
#include "word.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

int cipher_sqlite_begin(sqlite3 *db);
int cipher_sqlite_commit(sqlite3 *db);

int cipher_sqlite_create_rule(sqlite3 *db);
int cipher_sqlite_create_word(sqlite3 *db);
int cipher_sqlite_create_info(sqlite3 *db);
int cipher_sqlite_delete_rule(sqlite3 *db);
int cipher_sqlite_delete_word(sqlite3 *db);
int cipher_sqlite_delete_info(sqlite3 *db);

int cipher_sqlite_init(sqlite3 *db);
int cipher_sqlite_exit(sqlite3 *db);

int cipher_sqlite_out_word(sqlite3 *db, cipher_word_s *out);
int cipher_sqlite_out_info(sqlite3 *db, cipher_info_s *out);

int cipher_sqlite_add_word(sqlite3 *db, const cipher_word_s *in);
int cipher_sqlite_add_info(sqlite3 *db, const cipher_info_s *in);
int cipher_sqlite_del_word(sqlite3 *db, const cipher_word_s *in);
int cipher_sqlite_del_info(sqlite3 *db, const cipher_info_s *in);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* __CIPHER_SQLITE_H__ */
