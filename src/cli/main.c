/*!
 @file main.c
 @brief main program
 @copyright Copyright (C) 2020-present tqfx, All rights reserved.
*/

#include "app.h"

#include "path.h"

#include <getopt.h>

#define OPTION_INDEX (1 << 0)
#define OPTION_SEARCH (1 << 1)
#define OPTION_CREATE (1 << 2)
#define OPTION_DELETE (1 << 3)

#define OPTION_SET(stat, mask) ((stat) |= (mask))
#define OPTION_CLR(stat, mask) ((stat) &= ~(mask))
#define OPTION_IS_SET(stat, mask) (((stat) & (mask)) == (mask))

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic ignored "-Wc++-compat"
#endif /* __GNUC__ || __clang__ */

#pragma pack(push, 4)
static struct
{
    char *file;
    char *import;
    char *export;
    cipher_s ctx[1];
    c_word_s word[1];
    c_info_s info[1];
    int option;
} local[1] = {
    {
        .file = 0,
        .import = 0,
        .export = 0,
        .option = 0,
    },
};
#pragma pack(pop)

static void main_help(void)
{
    static const char help[] = "option: --import > -d[i] > -c > -s[i] > -i\n\
  -i --index     using the index\n\
  -s --search    search something\n\
  -c --create    create something\n\
  -d --delete    delete something\n\
  -a --hash      hash algorithm\n\
  -k --text      string(\"*\"->\"\")\n\
  -t --type      number(0:email 1:digit 2:other)\n\
  -m --misc      string\n\
  -h --hint      string\n\
  -l --length    number(0~128)\n\
  -p --password  string(\"*\"->\"\")\n\
  -f --filename  filename\n\
     --import    filename\n\
     --export    filename\n\
hash algorithm: MD5(default)\n\
     SHA1  SHA256  SHA224  BLAKE2S\n\
     SHA3  SHA512  SHA384  BLAKE2B\n\
Copyright (C) 2020-present tqfx, All rights reserved.";
    str_t self = path_self();
    printf("%s\n%s\n", self, help);
    free(self);
}

static int main_app(void)
{
#if defined(_WIN32)
    c_info_foreach(it, local->info)
    {
        if (cipher_get_text(it))
        {
            char *text;
            code_to_utf8(&text, it->text);
            free(it->text);
            it->text = text;
        }
    }
#endif /* _WIN32 */

    if (local->file == 0)
    {
        char *exe = path_self();
#if defined(_WIN32)
        if (strstr(exe, ".exe"))
        {
            exe[strlen(exe) - 4] = 0;
        }
#endif /* _WIN32 */
        str_s str[1] = {STR_NIL};
        str_printf(str, "%s%s", exe, ".db");
        local->file = str_exit(str);
        free(exe);
    }

    app_init(local->file);

    if (local->import && local->export)
    {
        app_convert(local->import, local->export);
    }
    else if (local->import)
    {
        app_import(local->import);
    }
    else if (local->export)
    {
        app_export(local->export);
    }
    else if (OPTION_IS_SET(local->option, OPTION_DELETE))
    {
        OPTION_CLR(local->option, OPTION_DELETE);
        if (OPTION_IS_SET(local->option, OPTION_INDEX))
        {
            OPTION_CLR(local->option, OPTION_INDEX);
            app_delete_word_idx(local->word);
            app_delete_info_idx(local->info);
        }
        else
        {
            app_delete_word(local->word);
            app_delete_info(local->info);
        }
    }
    else if (OPTION_IS_SET(local->option, OPTION_CREATE))
    {
        OPTION_CLR(local->option, OPTION_CREATE);
        app_create_word(local->word);
        app_create_info(local->info);
    }
    else if (OPTION_IS_SET(local->option, OPTION_SEARCH | OPTION_INDEX))
    {
        OPTION_CLR(local->option, OPTION_SEARCH | OPTION_INDEX);
        app_search_word_idx(local->word);
        app_search_info_idx(local->info);
    }
    else if ((local->option & (OPTION_SEARCH | OPTION_INDEX)) == OPTION_SEARCH)
    {
        OPTION_CLR(local->option, OPTION_SEARCH);
        c_word_foreach(it, local->word)
        {
            app_search_word_str(str_val(it));
        }
        c_info_foreach(it, local->info)
        {
            app_search_info_str(cipher_get_text(it));
        }
    }
    else if (c_info_num(local->info) && OPTION_IS_SET(local->option, OPTION_INDEX))
    {
        unsigned long ip = 0;
        unsigned long ik = 0;
        OPTION_CLR(local->option, OPTION_INDEX);
        c_info_foreach(it, local->info)
        {
            ik = strtoul(cipher_get_text(it), 0, 0);
            if (c_word_num(local->word))
            {
                str_s *str = c_word_top(local->word);
                ip = strtoul(str_val(str), 0, 0);
            }
            app_exec_idx(ip, ik);
        }
    }
    else if (c_info_num(local->info))
    {
        c_info_foreach(it, local->info)
        {
            if (c_word_num(local->word))
            {
                str_s *str = c_word_top(local->word);
                app_exec(it, str_val(str));
            }
            else
            {
                app_exec_ctx(it);
            }
        }
    }

    return app_exit();
}

static inline char *text_optarg(char *arg) { return (*arg != '*') ? arg : (arg + 1); }

#if defined(_MSC_VER)
#define strdup(s) _strdup(s)
#endif /* _MSC_VER */

int main(int argc, char *argv[])
{
    int ok = SUCCESS;

    const char *shortopts = "Hiscda:k:h:m:t:l:p:f:";
    const struct option longopts[] = {
        {"help", no_argument, 0, 'H'},
        {"index", no_argument, 0, 'i'},
        {"search", no_argument, 0, 's'},
        {"create", no_argument, 0, 'c'},
        {"delete", no_argument, 0, 'd'},
        {"hash", required_argument, 0, 'a'},
        {"text", required_argument, 0, 'k'},
        {"hint", required_argument, 0, 'h'},
        {"misc", required_argument, 0, 'm'},
        {"type", required_argument, 0, 't'},
        {"import", required_argument, 0, 1},
        {"export", required_argument, 0, 2},
        {"length", required_argument, 0, 'l'},
        {"password", required_argument, 0, 'p'},
        {"filename", required_argument, 0, 'f'},
        {0, 0, 0, 0},
    };

    if (argc < 2)
    {
        main_help();
        exit(EXIT_SUCCESS);
    }

    /* init */
    {
        cipher_ctor(local->ctx);
        c_word_ctor(local->word);
        c_info_ctor(local->info);
    }

    while (((void)(ok = getopt_long(argc, argv, shortopts, longopts, &optind)), ok) != -1)
    {
        switch (ok)
        {
        case 'H':
        {
            main_help();
            exit(EXIT_SUCCESS);
        }
        case 'i':
        {
            OPTION_SET(local->option, OPTION_INDEX);
        }
        break;
        case 's':
        {
            OPTION_SET(local->option, OPTION_SEARCH);
        }
        break;
        case 'c':
        {
            OPTION_SET(local->option, OPTION_CREATE);
        }
        break;
        case 'd':
        {
            OPTION_SET(local->option, OPTION_DELETE);
        }
        break;
        case 'k':
        {
            if (cipher_get_hash(local->ctx) == 0)
            {
                cipher_get_hash(local->ctx) = "MD5";
            }
            if (cipher_get_misc(local->ctx) && cipher_get_type(local->ctx) != CIPHER_OTHER)
            {
                cipher_get_misc(local->ctx) = 0;
            }
            cipher_get_text(local->ctx) = text_optarg(optarg);
            if (c_info_add(local->info, local->ctx))
            {
                fprintf(stderr, "%s + %s!\n", cipher_get_text(local->ctx), s_failure);
            }
        }
        break;
        case 'a':
        {
            cipher_get_hash(local->ctx) = optarg;
        }
        break;
        case 'h':
        {
            cipher_get_hint(local->ctx) = optarg;
        }
        break;
        case 'm':
        {
            cipher_get_misc(local->ctx) = optarg;
        }
        break;
        case 't':
        {
            cipher_get_type(local->ctx) = (uint_t)strtoul(optarg, 0, 0);
        }
        break;
        case 'l':
        {
            cipher_get_size(local->ctx) = (uint_t)strtoul(optarg, 0, 0);
        }
        break;
        case 'p':
        {
            str_s *str = c_word_push(local->word);
            str_puts(str, text_optarg(optarg));
        }
        break;
        case 'f':
        {
            if (local->file)
            {
                free(local->file);
            }
            local->file = strdup(optarg);
        }
        break;
        case 1:
        {
            if (local->import)
            {
                free(local->import);
            }
            local->import = strdup(optarg);
        }
        break;
        case 2:
        {
            if (local->export)
            {
                free(local->export);
            }
            local->export = strdup(optarg);
        }
        break;
        default:
            break;
        }
    }

    if (c_info_num(local->info))
    {
        cipher_s *ctx = c_info_top(local->info);
        if (cipher_get_hash(local->ctx))
        {
            cipher_set_hash(ctx, cipher_get_hash(local->ctx));
        }
        cipher_set_size(ctx, cipher_get_size(local->ctx));
        cipher_set_type(ctx, cipher_get_type(local->ctx));
        if (cipher_get_misc(local->ctx) && cipher_get_type(local->ctx) == CIPHER_OTHER)
        {
            cipher_set_misc(ctx, cipher_get_misc(local->ctx));
        }
        if (cipher_get_hint(local->ctx))
        {
            cipher_set_hint(ctx, cipher_get_hint(local->ctx));
        }
    }

    ok = main_app();

    /* exit */
    {
        c_word_dtor(local->word);
        c_info_dtor(local->info);
        free(local->export);
        local->export = 0;
        free(local->import);
        local->import = 0;
        free(local->file);
        local->file = 0;
    }

    return ok;
}
