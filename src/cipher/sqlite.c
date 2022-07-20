/*!
 @file sqlite.c
 @brief cipher sqlite
 @copyright Copyright (C) 2020 tqfx, All rights reserved.
*/

#include "cipher/sqlite.h"

#include <assert.h>
#include <string.h>

static struct
{
    const char *rule;
    const char *rule_0;
    const char *rule_1;
    const char *rule_2;
    const char *rule_3;
    const char *word;
    const char *word_text;
    const char *info;
    const char *info_hint;
    const char *info_text;
    const char *info_blob;
    const char *info_hash;
    const char *info_size;
    const char *info_type;
} local[1] = {
    {
        .rule = "rule",
        .rule_0 = "0",
        .rule_1 = "1",
        .rule_2 = "2",
        .rule_3 = "3",
        .word = "word",
        .word_text = "text",
        .info = "info",
        .info_hint = "hint",
        .info_text = "text",
        .info_blob = "blob",
        .info_size = "size",
        .info_hash = "hash",
        .info_type = "type",
    },
};

int cipher_sqlite_begin(sqlite3 *db)
{
    assert(db);
    sqlite3_stmt *stmt = 0;
    sqlite3_prepare(db, "begin;", -1, &stmt, 0);
    sqlite3_step(stmt);
    return sqlite3_finalize(stmt);
}

int cipher_sqlite_commit(sqlite3 *db)
{
    assert(db);
    sqlite3_stmt *stmt = 0;
    sqlite3_prepare(db, "commit;", -1, &stmt, 0);
    sqlite3_step(stmt);
    return sqlite3_finalize(stmt);
}

int cipher_sqlite_create_rule(sqlite3 *db)
{
    assert(db);
    sqlite3_stmt *stmt = 0;

    sqlite3_str *str = sqlite3_str_new(db);
    {
        const char *sql = "create table if not exists %s(%s text,%s text,%s text,%s text);";
        sqlite3_str_appendf(str, sql, local->rule,
                            local->rule_0, local->rule_1, local->rule_2, local->rule_3);
    }
    char *sql = sqlite3_str_finish(str);
    sqlite3_prepare(db, sql, -1, &stmt, 0);
    sqlite3_free(sql);
    sqlite3_step(stmt);

    return sqlite3_finalize(stmt);
}

int cipher_sqlite_create_word(sqlite3 *db)
{
    assert(db);
    sqlite3_stmt *stmt = 0;

    sqlite3_str *str = sqlite3_str_new(db);
    {
        const char *sql = "create table if not exists %s(%s text primary key);";
        sqlite3_str_appendf(str, sql, local->word, local->word_text);
    }
    char *sql = sqlite3_str_finish(str);
    sqlite3_prepare(db, sql, -1, &stmt, 0);
    sqlite3_free(sql);
    sqlite3_step(stmt);

    return sqlite3_finalize(stmt);
}

int cipher_sqlite_create_info(sqlite3 *db)
{
    assert(db);
    sqlite3_stmt *stmt = 0;

    sqlite3_str *str = sqlite3_str_new(db);
    {
        const char *sql = "create table if not exists %s("
                          "%s text primary key,"
                          "%s text default MD5,"
                          "%s integer default 16,"
                          "%s integer default 0,"
                          "%s text,%s text);";
        sqlite3_str_appendf(str, sql, local->info, local->info_text, local->info_hash,
                            local->info_size, local->info_type, local->info_blob, local->info_hint);
    }
    char *sql = sqlite3_str_finish(str);
    sqlite3_prepare(db, sql, -1, &stmt, 0);
    sqlite3_free(sql);
    sqlite3_step(stmt);

    return sqlite3_finalize(stmt);
}

int cipher_sqlite_delete_rule(sqlite3 *db)
{
    assert(db);
    sqlite3_stmt *stmt = 0;

    sqlite3_str *str = sqlite3_str_new(db);
    {
        const char *sql = "drop table if exists %s;";
        sqlite3_str_appendf(str, sql, local->rule);
    }
    char *sql = sqlite3_str_finish(str);
    sqlite3_prepare(db, sql, -1, &stmt, 0);
    sqlite3_free(sql);
    sqlite3_step(stmt);

    return sqlite3_finalize(stmt);
}

int cipher_sqlite_delete_word(sqlite3 *db)
{
    assert(db);
    sqlite3_stmt *stmt = 0;

    sqlite3_str *str = sqlite3_str_new(db);
    {
        const char *sql = "drop table if exists %s;";
        sqlite3_str_appendf(str, sql, local->word);
    }
    char *sql = sqlite3_str_finish(str);
    sqlite3_prepare(db, sql, -1, &stmt, 0);
    sqlite3_free(sql);
    sqlite3_step(stmt);

    return sqlite3_finalize(stmt);
}

int cipher_sqlite_delete_info(sqlite3 *db)
{
    assert(db);
    sqlite3_stmt *stmt = 0;

    sqlite3_str *str = sqlite3_str_new(db);
    {
        const char *sql = "drop table if exists %s;";
        sqlite3_str_appendf(str, sql, local->info);
    }
    char *sql = sqlite3_str_finish(str);
    sqlite3_prepare(db, sql, -1, &stmt, 0);
    sqlite3_free(sql);
    sqlite3_step(stmt);

    return sqlite3_finalize(stmt);
}

int cipher_sqlite_init(sqlite3 *db)
{
    assert(db);
    cipher_sqlite_create_rule(db);
    cipher_sqlite_create_word(db);
    cipher_sqlite_create_info(db);
    return cipher_sqlite_begin(db);
}

int cipher_sqlite_exit(sqlite3 *db)
{
    assert(db);
    return cipher_sqlite_commit(db);
}

int cipher_sqlite_out_word(sqlite3 *db, cipher_word_s *out)
{
    assert(db);
    assert(out);
    sqlite3_stmt *stmt = 0;

    sqlite3_str *str = sqlite3_str_new(db);
    {
        const char *sql = "select * from %s;";
        sqlite3_str_appendf(str, sql, local->word);
    }
    char *sql = sqlite3_str_finish(str);
    sqlite3_prepare(db, sql, -1, &stmt, 0);
    sqlite3_free(sql);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        const unsigned char *text = sqlite3_column_text(stmt, 0);
        if (text == 0)
        {
            continue;
        }
        str_s *ctx = cipher_word_push(out);
        str_puts(ctx, text);
    }

    return sqlite3_finalize(stmt);
}

int cipher_sqlite_out_info(sqlite3 *db, cipher_info_s *out)
{
    assert(db);
    assert(out);
    sqlite3_stmt *stmt = 0;

    sqlite3_str *str = sqlite3_str_new(db);
    {
        const char *sql = "select * from %s order by %s asc;";
        sqlite3_str_appendf(str, sql, local->info, local->info_text);
    }
    char *sql = sqlite3_str_finish(str);
    sqlite3_prepare(db, sql, -1, &stmt, 0);
    sqlite3_free(sql);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        const unsigned char *text = sqlite3_column_text(stmt, 0);
        if (text == 0)
        {
            continue;
        }
        cipher_s *ctx = cipher_info_push(out);
        cipher_set_text(ctx, text);
        if (((void)(text = sqlite3_column_text(stmt, 1)), text))
        {
            cipher_set_hash(ctx, text);
        }
        cipher_set_size(ctx, (unsigned int)sqlite3_column_int(stmt, 2));
        cipher_set_type(ctx, (unsigned int)sqlite3_column_int(stmt, 3));
        if (((void)(text = sqlite3_column_text(stmt, 5)), text) &&
            cipher_get_type(ctx) == CIPHER_OTHER)
        {
            cipher_set_blob(ctx, text);
        }
        if (((void)(text = sqlite3_column_text(stmt, 5)), text))
        {
            cipher_set_hint(ctx, text);
        }
    }

    return sqlite3_finalize(stmt);
}

int cipher_sqlite_add_word(sqlite3 *db, const cipher_word_s *in)
{
    assert(db);
    assert(in);
    sqlite3_stmt *stmt = 0;

    sqlite3_str *str = sqlite3_str_new(db);
    {
        const char *sql = "insert into %s values(?);";
        sqlite3_str_appendf(str, sql, local->word);
    }
    char *sql = sqlite3_str_finish(str);
    sqlite3_prepare(db, sql, -1, &stmt, 0);
    sqlite3_free(sql);

    cipher_word_foreach(it, in)
    {
        if (str_len(it))
        {
            sqlite3_reset(stmt);
            sqlite3_bind_text(stmt, 1, str_val(it), (int)str_len(it), SQLITE_STATIC);
            sqlite3_step(stmt);
        }
    }

    return sqlite3_finalize(stmt);
}

int cipher_sqlite_add_info(sqlite3 *db, const cipher_info_s *in)
{
    assert(db);
    assert(in);
    sqlite3_stmt *stmt = 0;

    sqlite3_str *str = sqlite3_str_new(db);
    {
        const char *sql = "insert into %s values(?,?,?,?,?,?);";
        sqlite3_str_appendf(str, sql, local->info);
    }
    char *sql = sqlite3_str_finish(str);
    sqlite3_prepare(db, sql, -1, &stmt, 0);
    sqlite3_free(sql);

    cipher_info_foreach(it, in)
    {
        if (cipher_get_text(it))
        {
            sqlite3_reset(stmt);
            sqlite3_bind_int(stmt, 3, (int)cipher_get_size(it));
            sqlite3_bind_int(stmt, 4, (int)cipher_get_type(it));
            {
                int size = (int)strlen(cipher_get_text(it));
                sqlite3_bind_text(stmt, 1, cipher_get_text(it), size, SQLITE_STATIC);
            }
            {
                int size = cipher_get_hash(it)
                               ? (int)strlen(cipher_get_hash(it))
                               : 0;
                sqlite3_bind_text(stmt, 2, cipher_get_hash(it), size, SQLITE_STATIC);
            }
            {
                int size = (cipher_get_blob(it) && cipher_get_type(it) == CIPHER_OTHER)
                               ? (int)strlen(cipher_get_blob(it))
                               : 0;
                sqlite3_bind_text(stmt, 5, cipher_get_blob(it), size, SQLITE_STATIC);
            }
            {
                int size = cipher_get_hint(it)
                               ? (int)strlen(cipher_get_hint(it))
                               : 0;
                sqlite3_bind_text(stmt, 6, cipher_get_hint(it), size, SQLITE_STATIC);
            }
            sqlite3_step(stmt);
        }
    }

    return sqlite3_finalize(stmt);
}

int cipher_sqlite_del_word(sqlite3 *db, const cipher_word_s *in)
{
    assert(db);
    assert(in);
    sqlite3_stmt *stmt = 0;

    sqlite3_str *str = sqlite3_str_new(db);
    {
        const char *sql = "delete from %s where %s = ?;";
        sqlite3_str_appendf(str, sql, local->word, local->word_text);
    }
    char *sql = sqlite3_str_finish(str);
    sqlite3_prepare(db, sql, -1, &stmt, 0);
    sqlite3_free(sql);

    cipher_word_foreach(it, in)
    {
        if (str_len(it))
        {
            sqlite3_reset(stmt);
            sqlite3_bind_text(stmt, 1, str_val(it), (int)str_len(it), SQLITE_STATIC);
            sqlite3_step(stmt);
        }
    }

    return sqlite3_finalize(stmt);
}

int cipher_sqlite_del_info(sqlite3 *db, const cipher_info_s *in)
{
    assert(db);
    assert(in);
    sqlite3_stmt *stmt = 0;

    sqlite3_str *str = sqlite3_str_new(db);
    {
        const char *sql = "delete from %s where %s = ?;";
        sqlite3_str_appendf(str, sql, local->info, local->info_text);
    }
    char *sql = sqlite3_str_finish(str);
    sqlite3_prepare(db, sql, -1, &stmt, 0);
    sqlite3_free(sql);

    cipher_info_foreach(it, in)
    {
        if (cipher_get_text(it))
        {
            sqlite3_reset(stmt);
            sqlite3_bind_text(stmt, 1, cipher_get_text(it), (int)strlen(cipher_get_text(it)), SQLITE_STATIC);
            sqlite3_step(stmt);
        }
    }

    return sqlite3_finalize(stmt);
}
