#include "app.h"
#include "utf8.h"
#include "clipboard.h"
#include <assert.h>
#include <time.h>

// clang-format off
#define TEXT_RED       CONSOLE_TEXT_RED
#define TEXT_GREEN     CONSOLE_TEXT_GREEN
#define TEXT_TURQUOISE CONSOLE_TEXT_TURQUOISE
#define TEXT_WHITE     CONSOLE_TEXT_WHITE
#define TEXT_DEFAULT   CONSOLE_TEXT_DEFAULT
// clang-format on

static void app_log3(char const *obj, unsigned int color, char const *local, char const *info)
{
    app_log(3, TEXT_WHITE, obj, color, local, TEXT_TURQUOISE, info);
}

#define TITLE_WORD "INDEX P\n"
static void app_print_word(size_t index, char const *word)
{
    char buf[1 << 4];
    sprintf(buf, "%05zu", index);
    app_log(2, TEXT_TURQUOISE, buf, TEXT_DEFAULT, word);
}

#define TITLE_INFO "INDEX T LEN HASHMAC K\n"
static void app_print_info(size_t index, cipher_s const *info)
{
    char *text = info->text;
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

void app_log(unsigned int num, ...)
{
    va_list ap;
    va_start(ap, num);
    console_vprint(" ", num, ap);
    printf("\n");
    va_end(ap);
}

int app_exec(cipher_s const *info, char const *word)
{
    assert(info);
    assert(word);

    if ((info->type == CIPHER_OTHER) && (info->misc == 0))
    {
        app_log(2, TEXT_RED, s_missing, TEXT_TURQUOISE, "misc");
        return A_FAILURE;
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
        return A_FAILURE;
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

    return A_SUCCESS;
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
    char const *fname;
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

int app_init(char const *fname)
{
    assert(fname);

    if (STATUS_IS_SET(local->status, STATUS_INIT))
    {
        return A_FAILURE;
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
        return A_FAILURE;
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

void app_search_word_str(void const *word)
{
    printf(TITLE_WORD);
    char const *str = word ? (char const *)word : "";
    a_vec_forenum(i, local->word)
    {
        a_str_s *it = a_vec_at_(a_str_s, local->word, i);
        if (a_str_len(it) && utf8casestr(a_str_ptr(it), str))
        {
            app_print_word(i, a_str_ptr(it));
        }
    }
}

void app_search_info_str(void const *info)
{
    size_t idx = 0;
    printf(TITLE_INFO);
    char const *str = info ? (char const *)info : "";
    a_avl_foreach(cur, local->info->root)
    {
        cipher_s *it = a_avl_entry(cur, info_s, node)->cipher;
        if (cipher_get_text(it) && utf8casestr(cipher_get_text(it), str))
        {
            app_print_info(idx, it);
        }
        ++idx;
    }
}

void app_search_word_idx(a_vec_s const *word)
{
    assert(word);
    if (a_vec_num(word))
    {
        printf(TITLE_WORD);
    }
    a_vec_foreach(a_str_s, it, word)
    {
        unsigned long idx = strtoul(a_str_ptr(it), 0, 0);
        if (idx < a_vec_num(local->word))
        {
            app_print_word(idx, a_str_ptr(it));
        }
    }
}

void app_search_info_idx(a_vec_s const *info)
{
    assert(info);
    if (a_vec_num(info))
    {
        printf(TITLE_INFO);
    }
    a_vec_foreach(cipher_s, it, info)
    {
        unsigned long idx = strtoul(cipher_get_text(it), 0, 0);
        if (idx < c_info_num(local->info))
        {
            app_print_info(idx, it);
        }
    }
}

int app_create_word(a_vec_s const *word)
{
    assert(word);
    int ok = A_FAILURE;
    a_vec_foreach(a_str_s, it, word)
    {
        if ((a_str_len(it) == 0) || (strlen(a_str_ptr(it)) == 0))
        {
            app_log3(local->fname, TEXT_RED, s_missing, "p");
            break;
        }
        ok = c_word_add(local->word, it);
        if (ok == A_SUCCESS)
        {
            STATUS_SET(local->status, STATUS_MODP);
            app_log3(local->fname, TEXT_GREEN, s_success, a_str_ptr(it));
        }
        else
        {
            app_log3(local->fname, TEXT_RED, s_failure, a_str_ptr(it));
        }
    }
    return ok;
}

int app_create_info(a_vec_s const *info)
{
    assert(info);
    int ok = A_FAILURE;
    a_vec_foreach(cipher_s, it, info)
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
        info_s *ctx = c_info_add(local->info, it->text);
        if (ctx)
        {
            STATUS_SET(local->status, STATUS_MODK);
            cipher_copy(ctx->cipher, it);
            ctx->time = time(NULL) + A_I32_MIN;
            app_exec_ctx(it);
        }
        else
        {
            app_log3(local->fname, TEXT_RED, s_failure, cipher_get_text(it));
        }
    }
    return ok;
}

int app_delete_word(a_vec_s const *word)
{
    assert(word);
    int ok = A_FAILURE;
    a_vec_foreach(a_str_s, it, word)
    {
        ok = c_word_del(local->word, it);
        if (ok == A_SUCCESS)
        {
            STATUS_SET(local->status, STATUS_MODP);
            app_log3(local->fname, TEXT_GREEN, s_success, a_str_ptr(it));
        }
        else
        {
            app_log3(local->fname, TEXT_RED, s_failure, a_str_ptr(it));
        }
    }
    return ok;
}

int app_delete_info(a_vec_s const *info)
{
    assert(info);
    int ok = A_FAILURE;
    a_vec_foreach(cipher_s, it, info)
    {
        info_s *ctx = c_info_del(local->info, it->text);
        if (ctx)
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

int app_delete_word_idx(a_vec_s const *word)
{
    assert(word);
    int ok = A_FAILURE;
    a_vec_foreach(a_str_s, it, word)
    {
        a_str_s *str = 0;
        unsigned long x = strtoul(a_str_ptr(it), 0, 0);
        if (x < a_vec_num(local->word) &&
            ((void)(str = a_vec_at_(a_str_s, local->word, x)), a_str_len(str)))
        {
            STATUS_SET(local->status, STATUS_MODP);
            app_print_word(x, a_str_ptr(str));
            a_str_dtor(str);
            ok = A_SUCCESS;
        }
    }
    return ok;
}

int app_delete_info_idx(a_vec_s const *info)
{
    assert(info);
    int ok = A_FAILURE;
    a_vec_foreach(cipher_s, it, info)
    {
        cipher_s *ctx = 0;
        unsigned long x = strtoul(cipher_get_text(it), 0, 0);
        if (x < c_info_num(local->info) &&
            ((void)(ctx = c_info_at(local->info, x)->cipher), cipher_get_text(ctx)))
        {
            STATUS_SET(local->status, STATUS_MODK);
            app_print_info(x, ctx);
            cipher_dtor(ctx);
            ok = A_SUCCESS;
        }
    }
    return ok;
}

int app_exec_ctx(cipher_s const *ctx)
{
    assert(ctx);
    int ok = A_FAILURE;
    if (a_vec_num(local->word))
    {
        ok = app_exec(ctx, a_str_ptr(a_vec_ptr(a_str_s, local->word)));
    }
    else
    {
        app_log(2, TEXT_RED, s_missing, TEXT_TURQUOISE, "p");
    }
    return ok;
}

int app_exec_idx(size_t word, size_t info)
{
    int ok = A_FAILURE;

    if (a_vec_num(local->word) == 0)
    {
        app_log3(local->fname, TEXT_RED, s_missing, "p");
        goto exit;
    }
    else if (word >= a_vec_num(local->word))
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
        a_vec_swap(local->word, word, 0);
        STATUS_SET(local->status, STATUS_MODP);
    }

    ok = app_exec(c_info_at(local->info, info)->cipher,
                  a_str_ptr(a_vec_ptr(a_str_s, local->word)));

exit:
    return ok;
}

#include "cipher/json.h"
#include "cipher/stream.h"

static int app_import_(c_info_s *info, char const *in)
{
    int ok = A_FAILURE;

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

static int app_export_(c_info_s *info, char const *out)
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
    char *str = cJSON_PrintUnformatted(json);
    if (str)
    {
        stream_write(out, str, strlen(str));
        free(str);
    }
    cJSON_Delete(json);

    return A_SUCCESS;
}

int app_convert(char const *in, char const *out)
{
    assert(in);
    assert(out);
    int ok = A_FAILURE;
    c_info_s info[1] = {C_INFO_INIT};
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
    c_info_dtor(info);
    return ok;
}

int app_import(char const *fname)
{
    assert(fname);
    c_info_s info[1] = {C_INFO_INIT};
    int ok = app_import_(info, fname);
    if (c_info_num(info) && ok == A_SUCCESS)
    {
        STATUS_SET(local->status, STATUS_MODK);
        a_avl_foreach(cur, info->root)
        {
            info_s *it = a_avl_entry(cur, info_s, node);
            info_s *item = c_info_add(local->info, it->cipher->text);
            if (it->time >= item->time)
            {
                cipher_copy(item->cipher, it->cipher);
                item->time = it->time;
            }
        }
        app_log3(local->fname, TEXT_GREEN, s_success, fname);
    }
    else
    {
        app_log3(local->fname, TEXT_RED, s_failure, fname);
    }
    c_info_dtor(info);
    return ok;
}

int app_export(char const *fname)
{
    assert(fname);
    return app_export_(local->info, fname);
}
