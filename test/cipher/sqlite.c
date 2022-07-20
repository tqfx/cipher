/*!
 @file sqlite.c
 @brief Testing cipher sqlite
 @copyright Copyright (C) 2020 tqfx, All rights reserved.
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

    cipher_sqlite_init(db);
    cipher_word_s *word = cipher_word_new();
    cipher_info_s *info = cipher_info_new();
    cipher_sqlite_out_word(db, word);
    cipher_sqlite_out_info(db, info);

    printf("0x%zX 0x%zX\n", cipher_word_num(word), cipher_info_num(info));

    size_t n = cipher_word_num(word) + 0x10;

    for (size_t i = cipher_word_num(word); i != n; ++i)
    {
        str_s *str = cipher_word_push(word);
        if (str == 0)
        {
            continue;
        }
        str_printf(str, "%zu", i);
        cipher_s *obj = cipher_info_push(info);
        if (obj == 0)
        {
            continue;
        }
        cipher_set_text(obj, str_val(str));
    }

    cipher_sqlite_begin(db);
    cipher_sqlite_add_word(db, word);
    cipher_sqlite_add_info(db, info);
    cipher_sqlite_commit(db);

    cipher_word_die(word);
    cipher_info_die(info);

    return sqlite3_close(db);
}
