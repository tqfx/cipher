#include "cipher/sqlite.h"
#include <assert.h>
#include <string.h>

int c_sqlite_begin(sqlite3 *db)
{
    assert(db);
    sqlite3_stmt *stmt = 0;
    sqlite3_prepare(db, "BEGIN;", -1, &stmt, 0);
    sqlite3_step(stmt);
    return sqlite3_finalize(stmt);
}

int c_sqlite_commit(sqlite3 *db)
{
    assert(db);
    sqlite3_stmt *stmt = 0;
    sqlite3_prepare(db, "COMMIT;", -1, &stmt, 0);
    sqlite3_step(stmt);
    return sqlite3_finalize(stmt);
}

int c_sqlite_create_rule(sqlite3 *db)
{
    assert(db);
    sqlite3_stmt *stmt = 0;

    sqlite3_str *str = sqlite3_str_new(db);
    sqlite3_str_appendf(str, "CREATE TABLE IF NOT EXISTS %s(%s TEXT,%s TEXT,%s TEXT,%s TEXT);",
                        "rule", "0", "1", "2", "3");
    char *sql = sqlite3_str_finish(str);
    sqlite3_prepare(db, sql, -1, &stmt, 0);
    sqlite3_free(sql);
    sqlite3_step(stmt);

    return sqlite3_finalize(stmt);
}

int c_sqlite_create_word(sqlite3 *db)
{
    assert(db);
    sqlite3_stmt *stmt = 0;

    sqlite3_str *str = sqlite3_str_new(db);
    sqlite3_str_appendf(str, "CREATE TABLE IF NOT EXISTS %s(%s TEXT PRIMARY KEY);", "word", "text");
    char *sql = sqlite3_str_finish(str);
    sqlite3_prepare(db, sql, -1, &stmt, 0);
    sqlite3_free(sql);
    sqlite3_step(stmt);

    return sqlite3_finalize(stmt);
}

int c_sqlite_create_info(sqlite3 *db)
{
    assert(db);
    sqlite3_stmt *stmt = 0;

    sqlite3_str *str = sqlite3_str_new(db);
    sqlite3_str_appendf(str, "CREATE TABLE IF NOT EXISTS %s("
                             "%s TEXT PRIMARY KEY ASC,"
                             "%s TEXT DEFAULT MD5,"
                             "%s INTEGER DEFAULT 16,"
                             "%s INTEGER DEFAULT 0,"
                             "%s TEXT,%s TEXT,%s INTEGER DEFAULT -2147483648);",
                        "info", "text", "hash", "size", "type", "misc", "hint", "time");
    char *sql = sqlite3_str_finish(str);
    sqlite3_prepare(db, sql, -1, &stmt, 0);
    sqlite3_free(sql);
    sqlite3_step(stmt);

    return sqlite3_finalize(stmt);
}

int c_sqlite_delete_rule(sqlite3 *db)
{
    assert(db);
    sqlite3_stmt *stmt = 0;

    sqlite3_str *str = sqlite3_str_new(db);
    sqlite3_str_appendf(str, "DROP TABLE IF EXISTS %s;", "rule");
    char *sql = sqlite3_str_finish(str);
    sqlite3_prepare(db, sql, -1, &stmt, 0);
    sqlite3_free(sql);
    sqlite3_step(stmt);

    return sqlite3_finalize(stmt);
}

int c_sqlite_delete_word(sqlite3 *db)
{
    assert(db);
    sqlite3_stmt *stmt = 0;

    sqlite3_str *str = sqlite3_str_new(db);
    sqlite3_str_appendf(str, "DROP TABLE IF EXISTS %s;", "word");
    char *sql = sqlite3_str_finish(str);
    sqlite3_prepare(db, sql, -1, &stmt, 0);
    sqlite3_free(sql);
    sqlite3_step(stmt);

    return sqlite3_finalize(stmt);
}

int c_sqlite_delete_info(sqlite3 *db)
{
    assert(db);
    sqlite3_stmt *stmt = 0;

    sqlite3_str *str = sqlite3_str_new(db);
    sqlite3_str_appendf(str, "DROP TABLE IF EXISTS %s;", "info");
    char *sql = sqlite3_str_finish(str);
    sqlite3_prepare(db, sql, -1, &stmt, 0);
    sqlite3_free(sql);
    sqlite3_step(stmt);

    return sqlite3_finalize(stmt);
}

int c_sqlite_init(sqlite3 *db)
{
    assert(db);
    c_sqlite_create_rule(db);
    c_sqlite_create_word(db);
    c_sqlite_create_info(db);
    return c_sqlite_begin(db);
}

int c_sqlite_exit(sqlite3 *db)
{
    assert(db);
    return c_sqlite_commit(db);
}

int c_sqlite_out_word(sqlite3 *db, c_word_s *out)
{
    assert(db);
    assert(out);
    sqlite3_stmt *stmt = 0;

    sqlite3_str *str = sqlite3_str_new(db);
    sqlite3_str_appendf(str, "SELECT * FROM %s;", "word");
    char *sql = sqlite3_str_finish(str);
    sqlite3_prepare(db, sql, -1, &stmt, 0);
    sqlite3_free(sql);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        unsigned char const *text = sqlite3_column_text(stmt, 0);
        if (text == 0)
        {
            continue;
        }
        a_str_s *ctx = c_word_push(out);
        a_str_puts(ctx, text);
    }

    return sqlite3_finalize(stmt);
}

int c_sqlite_out_info(sqlite3 *db, c_info_s *out)
{
    assert(db);
    assert(out);
    sqlite3_stmt *stmt = 0;

    sqlite3_str *str = sqlite3_str_new(db);
    sqlite3_str_appendf(str, "SELECT * FROM %s ORDER BY %s ASC;", "info", "text");
    char *sql = sqlite3_str_finish(str);
    sqlite3_prepare(db, sql, -1, &stmt, 0);
    sqlite3_free(sql);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        unsigned char const *text = sqlite3_column_text(stmt, 0);
        if (text == 0)
        {
            continue;
        }
        info_s *info = c_info_add(out, text);
        cipher_s *cipher = info->cipher;
        if (((void)(text = sqlite3_column_text(stmt, 1)), text))
        {
            cipher_set_hash(cipher, text);
        }
        else
        {
            cipher_set_hash(cipher, "MD5");
        }
        cipher_set_size(cipher, (unsigned int)sqlite3_column_int(stmt, 2));
        cipher_set_type(cipher, (unsigned int)sqlite3_column_int(stmt, 3));
        if (((void)(text = sqlite3_column_text(stmt, 4)), text) &&
            cipher_get_type(cipher) == CIPHER_OTHER)
        {
            cipher_set_misc(cipher, text);
        }
        if (((void)(text = sqlite3_column_text(stmt, 5)), text))
        {
            cipher_set_hint(cipher, text);
        }
        info->time = sqlite3_column_int64(stmt, 6);
    }

    return sqlite3_finalize(stmt);
}

int c_sqlite_add_word(sqlite3 *db, c_word_s const *in)
{
    assert(db);
    assert(in);
    sqlite3_stmt *stmt = 0;

    sqlite3_str *str = sqlite3_str_new(db);
    sqlite3_str_appendf(str, "INSERT INTO %s VALUES(?);", "word");
    char *sql = sqlite3_str_finish(str);
    sqlite3_prepare(db, sql, -1, &stmt, 0);
    sqlite3_free(sql);

    a_vec_foreach(a_str_s, it, in)
    {
        if (a_str_len(it))
        {
            sqlite3_reset(stmt);
            sqlite3_bind_text(stmt, 1, a_str_ptr(it), (int)a_str_len(it), SQLITE_STATIC);
            sqlite3_step(stmt);
        }
    }

    return sqlite3_finalize(stmt);
}

int c_sqlite_add_info(sqlite3 *db, c_info_s const *in)
{
    assert(db);
    assert(in);
    sqlite3_stmt *stmt = 0;

    sqlite3_str *str = sqlite3_str_new(db);
    sqlite3_str_appendf(str, "INSERT INTO %s VALUES(?,?,?,?,?,?,?);", "info");
    char *sql = sqlite3_str_finish(str);
    sqlite3_prepare(db, sql, -1, &stmt, 0);
    sqlite3_free(sql);

    a_avl_foreach(cur, in->root)
    {
        cipher_s *it = a_avl_entry(cur, info_s, node)->cipher;
        if (cipher_get_text(it))
        {
            sqlite3_reset(stmt);
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
            sqlite3_bind_int(stmt, 3, (int)cipher_get_size(it));
            sqlite3_bind_int(stmt, 4, (int)cipher_get_type(it));
            {
                int size = (cipher_get_misc(it) && cipher_get_type(it) == CIPHER_OTHER)
                               ? (int)strlen(cipher_get_misc(it))
                               : 0;
                sqlite3_bind_text(stmt, 5, cipher_get_misc(it), size, SQLITE_STATIC);
            }
            {
                int size = cipher_get_hint(it)
                               ? (int)strlen(cipher_get_hint(it))
                               : 0;
                sqlite3_bind_text(stmt, 6, cipher_get_hint(it), size, SQLITE_STATIC);
            }
            sqlite3_bind_int64(stmt, 7, a_avl_entry(cur, info_s, node)->time);
            sqlite3_step(stmt);
        }
    }

    return sqlite3_finalize(stmt);
}

int c_sqlite_del_word(sqlite3 *db, c_word_s const *in)
{
    assert(db);
    assert(in);
    sqlite3_stmt *stmt = 0;

    sqlite3_str *str = sqlite3_str_new(db);
    sqlite3_str_appendf(str, "DELETE FROM %s WHERE %s = ?;", "word", "text");
    char *sql = sqlite3_str_finish(str);
    sqlite3_prepare(db, sql, -1, &stmt, 0);
    sqlite3_free(sql);

    a_vec_foreach(a_str_s, it, in)
    {
        if (a_str_len(it))
        {
            sqlite3_reset(stmt);
            sqlite3_bind_text(stmt, 1, a_str_ptr(it), (int)a_str_len(it), SQLITE_STATIC);
            sqlite3_step(stmt);
        }
    }

    return sqlite3_finalize(stmt);
}

int c_sqlite_del_info(sqlite3 *db, c_info_s const *in)
{
    assert(db);
    assert(in);
    sqlite3_stmt *stmt = 0;

    sqlite3_str *str = sqlite3_str_new(db);
    sqlite3_str_appendf(str, "DELETE FROM %s WHERE %s = ?;", "info", "text");
    char *sql = sqlite3_str_finish(str);
    sqlite3_prepare(db, sql, -1, &stmt, 0);
    sqlite3_free(sql);

    a_avl_foreach(cur, in->root)
    {
        cipher_s *it = a_avl_entry(cur, info_s, node)->cipher;
        if (cipher_get_text(it))
        {
            sqlite3_reset(stmt);
            sqlite3_bind_text(stmt, 1, cipher_get_text(it), (int)strlen(cipher_get_text(it)), SQLITE_STATIC);
            sqlite3_step(stmt);
        }
    }

    return sqlite3_finalize(stmt);
}
