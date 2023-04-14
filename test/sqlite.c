#include "cipher/sqlite.h"
#include <stdio.h>
#include <time.h>

int main(int argc, char *argv[])
{
    char const *fname = "sqlite.db";

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
    c_info_s info[1] = {C_INFO_INIT};
    c_sqlite_out_word(db, word);
    c_sqlite_out_info(db, info);

    printf("0x%zX 0x%zX\n", a_vec_num(word), info->count);

    size_t n = a_vec_num(word) + 0x10;

    for (size_t i = a_vec_num(word); i != n; ++i)
    {
        a_str_s *str = c_word_push(word);
        if (str == 0)
        {
            continue;
        }
        a_str_printf(str, "%zu", i);
        info_s *ctx = c_info_add(info, a_str_ptr(str));
        ctx->time = time(NULL) + LONG_MIN;
    }

    c_sqlite_begin(db);
    c_sqlite_add_word(db, word);
    c_sqlite_add_info(db, info);
    c_sqlite_commit(db);

    c_word_die(word);
    c_info_dtor(info);

    return sqlite3_close(db);
}
