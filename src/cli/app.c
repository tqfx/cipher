/*!
 @file app.c
 @brief application
 @copyright Copyright (C) 2020-present tqfx, All rights reserved.
*/

#include "app.h"

#include "clipboard.h"

#include <assert.h>

// clang-format off
#define TEXT_RED       CONSOLE_TEXT_RED
#define TEXT_GREEN     CONSOLE_TEXT_GREEN
#define TEXT_TURQUOISE CONSOLE_TEXT_TURQUOISE
#define TEXT_WHITE     CONSOLE_TEXT_WHITE
#define TEXT_DEFAULT   CONSOLE_TEXT_DEFAULT
// clang-format on

static void app_log3(cstr_t obj, uint_t color, cstr_t local, cstr_t info)
{
    app_log(3, TEXT_WHITE, obj, color, local, TEXT_TURQUOISE, info);
}

#define TITLE_WORD "INDEX P\n"
static void app_print_word(size_t index, cstr_t word)
{
    char buf[1 << 4];
    sprintf(buf, "%05zu", index);
    app_log(2, TEXT_TURQUOISE, buf, TEXT_DEFAULT, word);
}

#define TITLE_INFO "INDEX T LEN HASHMAC K\n"
static void app_print_info(size_t index, const cipher_s *info)
{
    str_t text = info->text;
#if defined(_WIN32)
    code_utf8_to(&text, info->text);
#endif /* _WIN32 */
    char buf_i[1 << 4];
    sprintf(buf_i, "%05zu", index);
    char buf_t[1 << 2];
    sprintf(buf_t, "%u", info->type);
    char buf_z[1 << 2];
    sprintf(buf_z, "%3u", info->size);
    char buf_h[1 << 3];
    snprintf(buf_h, 1 << 3, "%-7s", info->hash);
    app_log(7, TEXT_TURQUOISE, buf_i, TEXT_GREEN, buf_t, TEXT_GREEN, buf_z, TEXT_GREEN, buf_h,
            TEXT_DEFAULT, text, TEXT_GREEN, info->misc, TEXT_DEFAULT, info->hint);
#if defined(_WIN32)
    free(text);
#endif /* _WIN32 */
}

void app_log(uint_t num, ...)
{
    va_list ap;
    va_start(ap, num);
    console_vprint(" ", num, ap);
    printf("\n");
    va_end(ap);
}

int app_exec(const cipher_s *info, cstr_t word)
{
    assert(info);
    assert(word);

    if ((info->type == CIPHER_OTHER) && (info->misc == 0))
    {
        app_log(2, TEXT_RED, s_missing, TEXT_TURQUOISE, "misc");
        return FAILURE;
    }

    str_t out = 0;
    if (cipher_v1(info, word, &out))
    {
        if ((out == 0) || (strlen(out) == 0))
        {
            app_log(2, TEXT_RED, s_missing, TEXT_TURQUOISE, "p");
        }
        else if ((info->text == 0) || (strlen(info->text) == 0))
        {
            app_log(2, TEXT_RED, s_missing, TEXT_TURQUOISE, "k");
        }
        else
        {
            app_log(2, TEXT_RED, s_failure, TEXT_TURQUOISE, "exec");
        }
        return FAILURE;
    }

#if defined(_WIN32)
    clipboard_sets(out);
    str_t text = 0;
    code_utf8_to(&text, info->text);
    app_log(2, TEXT_GREEN, "ok", TEXT_TURQUOISE, text);
    free(text);
#else /* !_WIN32 */
    app_log(2, TEXT_TURQUOISE, out, TEXT_DEFAULT, info->text);
#endif /* _WIN32 */

    free(out);

    return SUCCESS;
}

#define STATUS_ZERO 0
#define STATUS_INIT (1 << 0)
#define STATUS_DONE (1 << 1)
#define STATUS_MODP (1 << 8)
#define STATUS_MODK (1 << 9)

#define STATUS_SET(stat, mask) ((stat) |= (mask))
#define STATUS_CLR(stat, mask) ((stat) &= ~(mask))
#define STATUS_IS_SET(stat, mask) (((stat) & (mask)) == (mask))
#define STATUS_IS_CLR(stat, mask) (((stat) & (mask)) != (mask))

#pragma pack(push, 4)
static struct
{
    sqlite3 *db;
    cstr_t fname;
    c_word_s word[1];
    c_info_s info[1];
    int status;
} local[1] = {
    {
        .db = 0,
        .fname = 0,
        .status = STATUS_ZERO,
    },
};
#pragma pack(pop)

int app_init(cstr_t fname)
{
    assert(fname);

    if (STATUS_IS_SET(local->status, STATUS_INIT))
    {
        return FAILURE;
    }

    sqlite3_initialize();
    STATUS_CLR(local->status, STATUS_DONE);

    int ok = sqlite3_open(fname, &local->db);
    if (ok != SQLITE_OK)
    {
        fprintf(stderr, "%s\n", sqlite3_errmsg(local->db));
        exit(EXIT_FAILURE);
    }

    local->fname = fname;
    c_sqlite_init(local->db);
    STATUS_SET(local->status, STATUS_INIT);

    c_word_ctor(local->word);
    c_info_ctor(local->info);

    c_sqlite_out_word(local->db, local->word);
    c_sqlite_out_info(local->db, local->info);

    return ok;
}

int app_exit(void)
{
    if (STATUS_IS_CLR(local->status, STATUS_INIT))
    {
        return FAILURE;
    }

    if (STATUS_IS_SET(local->status, STATUS_MODP))
    {
        c_sqlite_delete_word(local->db);
        c_sqlite_create_word(local->db);
        c_sqlite_add_word(local->db, local->word);
        STATUS_CLR(local->status, STATUS_MODP);
    }
    if (STATUS_IS_SET(local->status, STATUS_MODK))
    {
        c_sqlite_delete_info(local->db);
        c_sqlite_create_info(local->db);
        c_sqlite_add_info(local->db, local->info);
        STATUS_CLR(local->status, STATUS_MODK);
    }

    c_sqlite_exit(local->db);
    sqlite3_close(local->db);
    STATUS_CLR(local->status, STATUS_INIT);

    c_word_dtor(local->word);
    c_info_dtor(local->info);
    STATUS_SET(local->status, STATUS_DONE);

    return sqlite3_shutdown();
}

void app_search_word_str(cptr_t word)
{
    printf(TITLE_WORD);
    cstr_t str = word ? (cstr_t)word : "";
    c_word_forenum(i, local->word)
    {
        str_s *it = c_word_at(local->word, i);
        if (str_len(it) && strstr(str_val(it), str))
        {
            app_print_word(i, str_val(it));
        }
    }
}

void app_search_info_str(cptr_t info)
{
    printf(TITLE_INFO);
    cstr_t str = info ? (cstr_t)info : "";
    c_info_forenum(i, local->info)
    {
        cipher_s *it = c_info_at(local->info, i);
        if (cipher_get_text(it) && strstr(cipher_get_text(it), str))
        {
            app_print_info(i, it);
        }
    }
}

void app_search_word_idx(const c_word_s *word)
{
    assert(word);
    if (c_word_num(word))
    {
        printf(TITLE_WORD);
    }
    c_word_foreach(it, word)
    {
        unsigned long idx = strtoul(str_val(it), 0, 0);
        if (idx < c_word_num(local->word))
        {
            app_print_word(idx, str_val(it));
        }
    }
}

void app_search_info_idx(const c_info_s *info)
{
    assert(info);
    if (c_info_num(info))
    {
        printf(TITLE_INFO);
    }
    c_info_foreach(it, info)
    {
        unsigned long idx = strtoul(cipher_get_text(it), 0, 0);
        if (idx < c_info_num(local->info))
        {
            app_print_info(idx, it);
        }
    }
}

int app_create_word(const c_word_s *word)
{
    assert(word);
    int ok = FAILURE;
    c_word_foreach(it, word)
    {
        if ((str_len(it) == 0) || (strlen(str_val(it)) == 0))
        {
            app_log3(local->fname, TEXT_RED, s_missing, "p");
            break;
        }
        ok = c_word_add(local->word, it);
        if (ok == SUCCESS)
        {
            STATUS_SET(local->status, STATUS_MODP);
            app_log3(local->fname, TEXT_GREEN, s_success, str_val(it));
        }
        else
        {
            app_log3(local->fname, TEXT_RED, s_failure, str_val(it));
        }
    }
    return ok;
}

int app_create_info(const c_info_s *info)
{
    assert(info);
    int ok = FAILURE;
    c_info_foreach(it, info)
    {
        if ((cipher_get_type(it) == CIPHER_OTHER) && (cipher_get_misc(it) == 0))
        {
            app_log3(local->fname, TEXT_RED, s_missing, "misc");
            break;
        }
        if ((cipher_get_text(it) == 0) || (strlen(cipher_get_text(it)) == 0))
        {
            app_log3(local->fname, TEXT_RED, s_missing, "k");
            break;
        }
        ok = c_info_add(local->info, it);
        if (ok == SUCCESS)
        {
            STATUS_SET(local->status, STATUS_MODK);
            app_exec_ctx(it);
        }
        else
        {
            app_log3(local->fname, TEXT_RED, s_failure, cipher_get_text(it));
        }
    }
    return ok;
}

int app_delete_word(const c_word_s *word)
{
    assert(word);
    int ok = FAILURE;
    c_word_foreach(it, word)
    {
        ok = c_word_del(local->word, it);
        if (ok == SUCCESS)
        {
            STATUS_SET(local->status, STATUS_MODP);
            app_log3(local->fname, TEXT_GREEN, s_success, str_val(it));
        }
        else
        {
            app_log3(local->fname, TEXT_RED, s_failure, str_val(it));
        }
    }
    return ok;
}

int app_delete_info(const c_info_s *info)
{
    assert(info);
    int ok = FAILURE;
    c_info_foreach(it, info)
    {
        ok = c_info_del(local->info, it);
        if (ok == SUCCESS)
        {
            STATUS_SET(local->status, STATUS_MODK);
            app_log3(local->fname, TEXT_GREEN, s_success, cipher_get_text(it));
        }
        else
        {
            app_log3(local->fname, TEXT_RED, s_failure, cipher_get_text(it));
        }
    }
    return ok;
}

int app_delete_word_idx(const c_word_s *word)
{
    assert(word);
    int ok = FAILURE;
    c_word_foreach(it, word)
    {
        str_s *str = 0;
        unsigned long x = strtoul(str_val(it), 0, 0);
        if (x < c_word_num(local->word) &&
            ((void)(str = c_word_at(local->word, x)), str_len(str)))
        {
            STATUS_SET(local->status, STATUS_MODP);
            app_print_word(x, str_val(str));
            str_dtor(str);
            ok = SUCCESS;
        }
    }
    return ok;
}

int app_delete_info_idx(const c_info_s *info)
{
    assert(info);
    int ok = FAILURE;
    c_info_foreach(it, info)
    {
        cipher_s *ctx = 0;
        unsigned long x = strtoul(cipher_get_text(it), 0, 0);
        if (x < c_info_num(local->info) &&
            ((void)(ctx = c_info_at(local->info, x)), cipher_get_text(ctx)))
        {
            STATUS_SET(local->status, STATUS_MODK);
            app_print_info(x, ctx);
            cipher_dtor(ctx);
            ok = SUCCESS;
        }
    }
    return ok;
}

int app_exec_ctx(const cipher_s *ctx)
{
    assert(ctx);
    int ok = FAILURE;
    if (c_word_num(local->word))
    {
        ok = app_exec(ctx, str_val(c_word_ptr(local->word)));
    }
    else
    {
        app_log(2, TEXT_RED, s_missing, TEXT_TURQUOISE, "p");
    }
    return ok;
}

int app_exec_idx(size_t word, size_t info)
{
    int ok = FAILURE;

    if (c_word_num(local->word) == 0)
    {
        app_log3(local->fname, TEXT_RED, s_missing, "p");
        goto exit;
    }
    else if (word >= c_word_num(local->word))
    {
        app_log3(local->fname, TEXT_RED, s_failure, "p");
        goto exit;
    }

    if (c_info_num(local->info) == 0)
    {
        app_log3(local->fname, TEXT_RED, s_missing, "k");
        goto exit;
    }
    else if (info >= c_info_num(local->info))
    {
        app_log3(local->fname, TEXT_RED, s_failure, "k");
        goto exit;
    }

    if (word)
    {
        c_word_swap(local->word, word, 0);
        STATUS_SET(local->status, STATUS_MODP);
    }

    ok = app_exec(c_info_at(local->info, info),
                  str_val(c_word_ptr(local->word)));

exit:
    return ok;
}

#include "cipher/json.h"
#include "cipher/stream.h"

static int app_import_(c_info_s *info, cstr_t in)
{
    int ok = FAILURE;

    cJSON *json = 0;
    ok = c_json_load(&json, in);
    if (json)
    {
        ok = c_json_export_info(json, info);
        cJSON_Delete(json);
        return ok;
    }

    sqlite3 *db = 0;
    ok = sqlite3_open(in, &db);
    if (ok == SQLITE_OK)
    {
        return c_sqlite_out_info(db, info);
    }
    fprintf(stderr, "%s\n", sqlite3_errmsg(db));
    sqlite3_close(db);

    return ok;
}

static int app_export_(c_info_s *info, cstr_t out)
{
    if (strstr(out, ".db"))
    {
        sqlite3 *db = 0;
        int ok = sqlite3_open(out, &db);
        if (ok == SQLITE_OK)
        {
            c_sqlite_create_info(db);
            c_sqlite_begin(db);
            c_sqlite_add_info(db, info);
            c_sqlite_commit(db);
        }
        sqlite3_close(db);
        return ok;
    }

    cJSON *json = 0;
    c_json_import_info(&json, info);
    str_t str = cJSON_PrintUnformatted(json);
    if (str)
    {
        stream_write(out, str, strlen(str));
        free(str);
    }
    cJSON_Delete(json);

    return SUCCESS;
}

int app_convert(cstr_t in, cstr_t out)
{
    assert(in);
    assert(out);
    int ok = FAILURE;
    c_info_s *info = c_info_new();
    ok = app_import_(info, in);
    if (ok)
    {
        goto exit;
    }
    ok = app_export_(info, out);
    if (ok)
    {
        goto exit;
    }
exit:
    c_info_die(info);
    return ok;
}

int app_import(cstr_t fname)
{
    assert(fname);
    c_info_s *info = c_info_new();
    int ok = app_import_(info, fname);
    if (c_info_num(info) && ok == SUCCESS)
    {
        STATUS_SET(local->status, STATUS_MODK);
        c_info_foreach(it, info)
        {
            c_info_add(local->info, it);
        }
        app_log3(local->fname, TEXT_GREEN, s_success, fname);
    }
    else
    {
        app_log3(local->fname, TEXT_RED, s_failure, fname);
    }
    c_info_die(info);
    return ok;
}

int app_export(cstr_t fname)
{
    assert(fname);
    return app_export_(local->info, fname);
}
