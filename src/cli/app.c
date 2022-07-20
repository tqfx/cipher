/*!
 @file app.c
 @brief application
 @copyright Copyright (C) 2020 tqfx, All rights reserved.
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

static void app_log3(const char *obj, unsigned int color, const char *local, const char *info)
{
    app_log(3, TEXT_WHITE, obj, color, local, TEXT_TURQUOISE, info);
}

static void app_print_word(size_t index, const char *word)
{
    char buf[1 << 4];
    sprintf(buf, "%04zu", index);
    app_log(2, TEXT_TURQUOISE, buf, TEXT_DEFAULT, word);
}

static void app_print_info(size_t index, const cipher_s *info)
{
    char *text = info->text;
#if defined(_WIN32)
    code_utf8_to(&text, info->text);
#endif /* _WIN32 */
    char buf_i[1 << 4];
    sprintf(buf_i, "%04zu", index);
    char buf_t[1 << 2];
    sprintf(buf_t, "%u", info->type);
    char buf_z[1 << 2];
    sprintf(buf_z, "%02u", info->size);
    app_log(7, TEXT_TURQUOISE, buf_i,
            TEXT_GREEN, buf_z,
            TEXT_GREEN, buf_t,
            TEXT_DEFAULT, text,
            TEXT_DEFAULT, info->hash,
            TEXT_GREEN, info->blob,
            TEXT_DEFAULT, info->hint);
#if defined(_WIN32)
    free(text);
#endif /* _WIN32 */
}

void app_log(unsigned int num, ...)
{
    va_list ap;
    va_start(ap, num);
    console_vprint(" ", num, ap);
    printf("\n");
    va_end(ap);
}

int app_exec(const cipher_s *info, const char *word)
{
    assert(info);
    assert(word);

    if ((info->type == CIPHER_OTHER) && (info->blob == 0))
    {
        app_log(2, TEXT_RED, s_missing, TEXT_TURQUOISE, "blob");
        return ~0;
    }

    char *out = 0;
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
        return ~0;
    }

#if defined(_WIN32)
    clipboard_sets(out);
    char *text = 0;
    code_utf8_to(&text, info->text);
    app_log(2, TEXT_GREEN, "ok", TEXT_TURQUOISE, text);
    free(text);
#else /* !_WIN32 */
    app_log(2, TEXT_TURQUOISE, out, TEXT_DEFAULT, info->text);
#endif /* _WIN32 */

    free(out);

    return 0;
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
    const char *fname;
    cipher_word_s word[1];
    cipher_info_s info[1];
    int status;
} local[1] = {
    {
        .db = 0,
        .fname = 0,
        .status = STATUS_ZERO,
    },
};
#pragma pack(pop)

int app_init(const char *fname)
{
    assert(fname);

    if (STATUS_IS_SET(local->status, STATUS_INIT))
    {
        return ~0;
    }

    sqlite3_initialize();
    STATUS_CLR(local->status, STATUS_DONE);

    int ret = sqlite3_open(fname, &local->db);
    if (ret != SQLITE_OK)
    {
        fprintf(stderr, "%s\n", sqlite3_errmsg(local->db));
        exit(EXIT_FAILURE);
    }

    local->fname = fname;
    cipher_sqlite_init(local->db);
    STATUS_SET(local->status, STATUS_INIT);

    cipher_word_ctor(local->word);
    cipher_info_ctor(local->info);

    cipher_sqlite_out_word(local->db, local->word);
    cipher_sqlite_out_info(local->db, local->info);

    return ret;
}

int app_exit(void)
{
    if (STATUS_IS_CLR(local->status, STATUS_INIT))
    {
        return ~0;
    }

    if (STATUS_IS_SET(local->status, STATUS_MODP))
    {
        cipher_sqlite_delete_word(local->db);
        cipher_sqlite_create_word(local->db);
        cipher_sqlite_add_word(local->db, local->word);
        STATUS_CLR(local->status, STATUS_MODP);
    }
    if (STATUS_IS_SET(local->status, STATUS_MODK))
    {
        cipher_sqlite_delete_info(local->db);
        cipher_sqlite_create_info(local->db);
        cipher_sqlite_add_info(local->db, local->info);
        STATUS_CLR(local->status, STATUS_MODK);
    }

    cipher_sqlite_exit(local->db);
    sqlite3_close(local->db);
    STATUS_CLR(local->status, STATUS_INIT);

    cipher_word_dtor(local->word);
    cipher_info_dtor(local->info);
    STATUS_SET(local->status, STATUS_DONE);

    return sqlite3_shutdown();
}

void app_show_word(const void *word)
{
    printf("   I P\n");
    const char *str = word ? (const char *)word : "";
    str_s *it = 0;
    cipher_word_forboth(i, it, local->word)
    {
        if (str_len(it) && strstr(str_val(it), str))
        {
            app_print_word(i, str_val(it));
        }
    }
}

void app_show_info(const void *info)
{
    printf("   I  L T K\n");
    const char *str = info ? (const char *)info : "";
    cipher_s *it = 0;
    cipher_info_forboth(i, it, local->info)
    {
        if (cipher_get_text(it) && strstr(cipher_get_text(it), str))
        {
            app_print_info(i, it);
        }
    }
}

void app_show_word_idx(const cipher_word_s *word)
{
    assert(word);
    if (cipher_word_num(word))
    {
        printf("   I P\n");
    }
    cipher_word_foreach(it, word)
    {
        unsigned int idx;
        sscanf(str_val(it), "%u", &idx);
        if (idx < cipher_word_num(word))
        {
            app_print_word(idx, str_val(it));
        }
    }
}

void app_show_info_idx(const cipher_info_s *info)
{
    assert(info);
    if (cipher_info_num(info))
    {
        printf("   I  L T K\n");
    }
    cipher_info_foreach(it, info)
    {
        unsigned int idx;
        sscanf(cipher_get_text(it), "%u", &idx);
        if (idx < cipher_info_num(info))
        {
            app_print_info(idx, it);
        }
    }
}

int app_add_word(const cipher_word_s *word)
{
    assert(word);
    int ret = ~0;
    cipher_word_foreach(it, word)
    {
        if ((str_len(it) == 0) || (strlen(str_val(it)) == 0))
        {
            app_log3(local->fname, TEXT_RED, s_missing, "p");
            break;
        }
        ret = cipher_word_add(local->word, it);
        if (ret == 0)
        {
            STATUS_SET(local->status, STATUS_MODP);
            app_log3(local->fname, TEXT_GREEN, s_success, str_val(it));
        }
        else
        {
            app_log3(local->fname, TEXT_RED, s_failure, str_val(it));
        }
    }
    return ret;
}

int app_add_info(const cipher_info_s *info)
{
    assert(info);
    int ret = ~0;
    cipher_info_foreach(it, info)
    {
        if ((cipher_get_type(it) == CIPHER_OTHER) && (cipher_get_blob(it) == 0))
        {
            app_log3(local->fname, TEXT_RED, s_missing, "blob");
            break;
        }
        if ((cipher_get_text(it) == 0) || (strlen(cipher_get_text(it)) == 0))
        {
            app_log3(local->fname, TEXT_RED, s_missing, "k");
            break;
        }
        ret = cipher_info_add(local->info, it);
        if (ret == 0)
        {
            STATUS_SET(local->status, STATUS_MODK);
            app_generate_key(it);
        }
        else
        {
            app_log3(local->fname, TEXT_RED, s_failure, cipher_get_text(it));
        }
    }
    return ret;
}

int app_del_word(const cipher_word_s *word)
{
    assert(word);
    int ret = ~0;
    cipher_word_foreach(it, word)
    {
        ret = cipher_word_del(local->word, it);
        if (ret == 0)
        {
            STATUS_SET(local->status, STATUS_MODP);
            app_log3(local->fname, TEXT_GREEN, s_success, str_val(it));
        }
        else
        {
            app_log3(local->fname, TEXT_RED, s_failure, str_val(it));
        }
    }
    return ret;
}

int app_del_info(const cipher_info_s *info)
{
    assert(info);
    int ret = ~0;
    cipher_info_foreach(it, info)
    {
        ret = cipher_info_del(local->info, it);
        if (ret == 0)
        {
            STATUS_SET(local->status, STATUS_MODK);
            app_log3(local->fname, TEXT_GREEN, s_success, cipher_get_text(it));
        }
        else
        {
            app_log3(local->fname, TEXT_RED, s_failure, cipher_get_text(it));
        }
    }
    return ret;
}

int app_del_word_idx(const cipher_word_s *word)
{
    assert(word);
    int ret = ~0;
    cipher_word_foreach(it, word)
    {
        str_s *str = 0;
        unsigned int x = 0;
        sscanf(str_val(it), "%u", &x);
        if (x < cipher_word_num(local->word) &&
            ((void)(str = cipher_word_at(local->word, x)), str_len(str)))
        {
            STATUS_SET(local->status, STATUS_MODP);
            app_print_word(x, str_val(str));
            str_dtor(str);
            ret = 0;
        }
    }
    return ret;
}

int app_del_info_idx(const cipher_info_s *info)
{
    assert(info);
    int ret = ~0;
    cipher_info_foreach(it, info)
    {
        cipher_s *ctx = 0;
        unsigned int x = 0;
        sscanf(cipher_get_text(it), "%u", &x);
        if (x < cipher_info_num(local->info) &&
            ((void)(ctx = cipher_info_at(local->info, x)), cipher_get_text(ctx)))
        {
            STATUS_SET(local->status, STATUS_MODK);
            app_print_info(x, ctx);
            cipher_dtor(ctx);
            ret = 0;
        }
    }
    return ret;
}

int app_generate_idx(unsigned int word, unsigned int info)
{
    int ret = ~0;

    if (cipher_word_num(local->word) == 0)
    {
        app_log3(local->fname, TEXT_RED, s_missing, "p");
        goto label_exit;
    }
    else if (word >= cipher_word_num(local->word))
    {
        app_log3(local->fname, TEXT_RED, s_failure, "p");
        goto label_exit;
    }

    if (cipher_info_num(local->info) == 0)
    {
        app_log3(local->fname, TEXT_RED, s_missing, "k");
        goto label_exit;
    }
    else if (info >= cipher_info_num(local->info))
    {
        app_log3(local->fname, TEXT_RED, s_failure, "k");
        goto label_exit;
    }

    if (word)
    {
        str_s str[1];
        str_s *ptr = local->word->vec;
        str_move(str, ptr + word);
        str_move(ptr + word, ptr);
        str_move(ptr, str);
        STATUS_SET(local->status, STATUS_MODP);
    }

    str_s *str = local->word->vec;
    cipher_s *ptr = local->info->vec;
    ret = app_exec(ptr + info, str_val(str));

label_exit:
    return ret;
}

int app_generate_key(const cipher_s *ctx)
{
    assert(ctx);
    int ret = ~0;
    if (cipher_word_num(local->word))
    {
        ret = app_exec(ctx, str_val(local->word->vec));
    }
    else
    {
        app_log(2, TEXT_RED, s_missing, TEXT_TURQUOISE, "p");
    }
    return ret;
}

#include "cipher/json.h"
#include "cipher/stream.h"

static int app_import_(cipher_info_s *info, const char *in)
{
    assert(in);
    assert(info);
    int ret = ~0;
    cJSON *json = 0;
    ret = cipher_json_load(&json, in);
    if (json)
    {
        ret = cipher_json_export_info(json, info);
        cJSON_Delete(json);
        return ret;
    }
    sqlite3 *db = 0;
    ret = sqlite3_open(in, &db);
    if (ret == SQLITE_OK)
    {
        return cipher_sqlite_out_info(db, info);
    }
    fprintf(stderr, "%s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return ret;
}

static int app_export_(cipher_info_s *info, const char *out)
{
    assert(out);
    assert(info);

    if (strstr(out, ".db"))
    {
        sqlite3 *db = 0;
        int ret = sqlite3_open(out, &db);
        if (ret == SQLITE_OK)
        {
            cipher_sqlite_create_info(db);
            cipher_sqlite_begin(db);
            cipher_sqlite_add_info(db, info);
            cipher_sqlite_commit(db);
        }
        sqlite3_close(db);
        return ret;
    }

    cJSON *json = 0;
    cipher_json_import_info(&json, info);
    char *str = cJSON_PrintUnformatted(json);
    if (str)
    {
        stream_write(out, str, strlen(str));
        free(str);
    }
    cJSON_Delete(json);

    return 0;
}

int app_convert(const char *in, const char *out)
{
    assert(in);
    assert(out);
    int ret = ~0;
    cipher_info_s *info = cipher_info_new();
    ret = app_import_(info, in);
    if (ret)
    {
        goto label_exit;
    }
    ret = app_export_(info, out);
    if (ret)
    {
        goto label_exit;
    }
label_exit:
    cipher_info_die(info);
    return ret;
}

int app_import(const char *fname)
{
    assert(fname);
    cipher_info_s *info = cipher_info_new();
    int ret = app_import_(info, fname);
    if (cipher_info_num(info) && ret == 0)
    {
        STATUS_SET(local->status, STATUS_MODK);
        cipher_info_foreach(it, info)
        {
            cipher_info_add(local->info, it);
        }
        app_log3(local->fname, TEXT_GREEN, s_success, fname);
    }
    else
    {
        app_log3(local->fname, TEXT_RED, s_failure, fname);
    }
    cipher_info_die(info);
    return ret;
}

int app_export(const char *fname)
{
    assert(fname);
    return app_export_(local->info, fname);
}
