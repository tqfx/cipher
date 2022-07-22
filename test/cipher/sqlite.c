/*!
 @file sqlite.c
 @brief Testing cipher sqlite
 @copyright Copyright (C) 2020-present tqfx, All rights reserved.
*/

#include "cipher/sqlite.h"

#include <stdio.h>

int main(int argc, char *argv[])
{
    const char *fname = "sqlite.db";

    if (argc > 1)
    {
        fname = argv[argc - 1];
    }

    sqlite3 *db;
    int ret = sqlite3_open(fname, &db);
    if (ret)
    {
        fprintf(stderr, "%s\n", sqlite3_errmsg(db));
        exit(EXIT_FAILURE);
    }

    c_sqlite_init(db);
    c_word_s *word = c_word_new();
    c_info_s *info = c_info_new();
    c_sqlite_out_word(db, word);
    c_sqlite_out_info(db, info);

    printf("0x%zX 0x%zX\n", c_word_num(word), c_info_num(info));

    size_t n = c_word_num(word) + 0x10;

    for (size_t i = c_word_num(word); i != n; ++i)
    {
        str_s *str = c_word_push(word);
        if (str == 0)
        {
            continue;
        }
        str_printf(str, "%zu", i);
        cipher_s *obj = c_info_push(info);
        if (obj == 0)
        {
            continue;
        }
        cipher_set_text(obj, str_val(str));
    }

    c_sqlite_begin(db);
    c_sqlite_add_word(db, word);
    c_sqlite_add_info(db, info);
    c_sqlite_commit(db);

    c_word_die(word);
    c_info_die(info);

    return sqlite3_close(db);
}
