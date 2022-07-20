/*!
 @file main.c
 @brief main program
 @copyright Copyright (C) 2020 tqfx, All rights reserved.
*/

#include "app.h"

#include "path.h"

#include <getopt.h>

#define OPTION_ADD (1 << 0)
#define OPTION_DEL (1 << 1)
#define OPTION_SHOW (1 << 2)
#define OPTION_INDEX (1 << 3)

#define OPTION_SET(stat, mask) ((stat) |= (mask))
#define OPTION_CLR(stat, mask) ((stat) &= ~(mask))
#define OPTION_IS_SET(stat, mask) (((stat) & (mask)) == (mask))

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wc++-compat"
#endif /* __GNUC__ || __clang__ */

#pragma pack(push, 4)
static struct
{
    char *file;
    char *import;
    char *export;
    cipher_s ctx[1];
    cipher_word_s word[1];
    cipher_info_s info[1];
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
    static const char help[] = "option: --import > -d[i] > -a > -s[i] > -i\n\
  -a --add       option for add\n\
  -d --del       option for del\n\
  -s --show      option for show\n\
  -i --index     option for index\n\
  -k --key       string(\"*\"->\"\")\n\
  -t --type      0:email 1:digit 2:custom\n\
  -b --blob      string\n\
  -A --hash      hash\n\
  -H --hint      hint\n\
  -l --length    1 ~ 32\n\
  -p --password  string(\"*\"->\"\")\n\
  -f --filename  filename\n\
     --import    filename\n\
     --export    filename\n\
Copyright (C) 2020 tqfx, All rights reserved.";
    char *self = path_self();
    printf("%s\n%s\n", self, help);
    free(self);
}

static int main_app(void)
{
#if defined(_WIN32)
    cipher_info_foreach(it, local->info)
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
    else if (OPTION_IS_SET(local->option, OPTION_DEL))
    {
        OPTION_CLR(local->option, OPTION_DEL);
        if (OPTION_IS_SET(local->option, OPTION_INDEX))
        {
            OPTION_CLR(local->option, OPTION_INDEX);
            app_del_word_idx(local->word);
            app_del_info_idx(local->info);
        }
        else
        {
            app_del_word(local->word);
            app_del_info(local->info);
        }
    }
    else if (OPTION_IS_SET(local->option, OPTION_ADD))
    {
        OPTION_CLR(local->option, OPTION_ADD);
        app_add_word(local->word);
        app_add_info(local->info);
    }
    else if (OPTION_IS_SET(local->option, OPTION_SHOW | OPTION_INDEX))
    {
        OPTION_CLR(local->option, OPTION_SHOW | OPTION_INDEX);
        app_show_word_idx(local->word);
        app_show_info_idx(local->info);
    }
    else if ((local->option & (OPTION_SHOW | OPTION_INDEX)) == OPTION_SHOW)
    {
        OPTION_CLR(local->option, OPTION_SHOW);
        cipher_word_foreach(it, local->word)
        {
            app_show_word(str_val(it));
        }
        cipher_info_foreach(it, local->info)
        {
            app_show_info(cipher_get_text(it));
        }
    }
    else if (cipher_info_num(local->info) && OPTION_IS_SET(local->option, OPTION_INDEX))
    {
        unsigned int ip = 0;
        unsigned int ik = 0;
        OPTION_CLR(local->option, OPTION_INDEX);
        cipher_info_foreach(it, local->info)
        {
            sscanf(cipher_get_text(it), "%u", &ik);
            if (cipher_word_num(local->word))
            {
                str_s *str = cipher_word_top(local->word);
                sscanf(str_val(str), "%u", &ip);
            }
            app_generate_idx(ip, ik);
        }
    }
    else if (cipher_info_num(local->info))
    {
        cipher_info_foreach(it, local->info)
        {
            if (cipher_word_num(local->word))
            {
                str_s *str = cipher_word_top(local->word);
                app_exec(it, str_val(str));
            }
            else
            {
                app_generate_key(it);
            }
        }
    }
    else
    {
        main_help();
    }

    return app_exit();
}

static inline char *text_optarg(char *arg) { return (*arg != '*') ? arg : (arg + 1); }

int main(int argc, char *argv[])
{
    int opt = 0;
    const char *shortopts = "adhsik:A:H:b:t:l:p:f:";
    struct option longopts[] = {
        {"add", no_argument, 0, 'a'},
        {"del", no_argument, 0, 'd'},
        {"help", no_argument, 0, 'h'},
        {"show", no_argument, 0, 's'},
        {"index", no_argument, 0, 'i'},
        {"key", required_argument, 0, 'k'},
        {"hash", required_argument, 0, 'A'},
        {"hint", required_argument, 0, 'H'},
        {"blob", required_argument, 0, 'b'},
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
        cipher_word_ctor(local->word);
        cipher_info_ctor(local->info);
    }

    while (((void)(opt = getopt_long(argc, argv, shortopts, longopts, &optind)), opt) != -1)
    {
        switch (opt)
        {
        case 'h':
        {
            main_help();
            exit(EXIT_SUCCESS);
        }
        case 'a':
        {
            OPTION_SET(local->option, OPTION_ADD);
        }
        break;
        case 'd':
        {
            OPTION_SET(local->option, OPTION_DEL);
        }
        break;
        case 's':
        {
            OPTION_SET(local->option, OPTION_SHOW);
        }
        break;
        case 'i':
        {
            OPTION_SET(local->option, OPTION_INDEX);
        }
        break;
        case 'k':
        {
            if (cipher_get_blob(local->ctx) && cipher_get_type(local->ctx) != CIPHER_OTHER)
            {
                cipher_get_blob(local->ctx) = 0;
            }
            if (cipher_get_hash(local->ctx) == 0)
            {
                cipher_get_hash(local->ctx) = "MD5";
            }
            cipher_get_text(local->ctx) = text_optarg(optarg);
            if (cipher_info_add(local->info, local->ctx))
            {
                fprintf(stderr, "%s + %s!\n", cipher_get_text(local->ctx), s_failure);
            }
            cipher_get_text(local->ctx) = 0;
            cipher_get_hash(local->ctx) = 0;
            cipher_get_hint(local->ctx) = 0;
            cipher_get_blob(local->ctx) = 0;
        }
        break;
        case 'A':
        {
            cipher_get_hash(local->ctx) = optarg;
        }
        break;
        case 'H':
        {
            cipher_get_hint(local->ctx) = optarg;
        }
        break;
        case 'b':
        {
            cipher_get_blob(local->ctx) = optarg;
        }
        break;
        case 't':
        {
            sscanf(optarg, "%u", &cipher_get_type(local->ctx));
        }
        break;
        case 'l':
        {
            sscanf(optarg, "%u", &cipher_get_size(local->ctx));
        }
        break;
        case 'p':
        {
            str_s *str = cipher_word_push(local->word);
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

    if (cipher_info_num(local->info))
    {
        cipher_s *ctx = cipher_info_top(local->info);
        if (cipher_get_hash(local->ctx))
        {
            cipher_set_hash(ctx, cipher_get_hash(local->ctx));
            cipher_get_hash(local->ctx) = 0;
        }
        cipher_set_size(ctx, cipher_get_size(local->ctx));
        cipher_set_type(ctx, cipher_get_type(local->ctx));
        if (cipher_get_blob(local->ctx))
        {
            if (cipher_get_type(local->ctx) == CIPHER_OTHER)
            {
                cipher_set_blob(ctx, cipher_get_blob(local->ctx));
            }
            cipher_get_blob(local->ctx) = 0;
        }
        if (cipher_get_hint(local->ctx))
        {
            cipher_set_hint(ctx, cipher_get_hint(local->ctx));
            cipher_get_hint(local->ctx) = 0;
        }
    }

    opt = main_app();

    /* exit */
    {
        cipher_word_dtor(local->word);
        cipher_info_dtor(local->info);
        cipher_dtor(local->ctx);
        free(local->export);
        local->export = 0;
        free(local->import);
        local->import = 0;
        free(local->file);
        local->file = 0;
    }

    return opt;
}

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif /* __GNUC__ || __clang__ */
