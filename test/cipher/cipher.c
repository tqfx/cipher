/*!
 @file cipher.c
 @brief Testing cipher core
 @copyright Copyright (C) 2020-present tqfx, All rights reserved.
*/

#include "cipher/cipher.h"

#include <stdio.h>

static char word[] = "word";
static char text[] = "test";
static char misc[] = ".";

static void test_v1(void)
{
    char *out;

    cipher_s ctx[1];
    ctx->text = text;
    ctx->misc = misc;
    ctx->size = CIPHER_OUTSIZ;

    ctx->type = CIPHER_EMAIL;
    {
        ctx->hash = "MD5";
        cipher_v1(ctx, word, &out);
        printf("%8s: %s\n", ctx->hash, out);
        free(out);

        ctx->hash = "SHA1";
        cipher_v1(ctx, word, &out);
        printf("%8s: %s\n", ctx->hash, out);
        free(out);

        ctx->hash = "SHA256";
        cipher_v1(ctx, word, &out);
        printf("%8s: %s\n", ctx->hash, out);
        free(out);

        ctx->hash = "SHA224";
        cipher_v1(ctx, word, &out);
        printf("%8s: %s\n", ctx->hash, out);
        free(out);

        ctx->hash = "SHA512";
        cipher_v1(ctx, word, &out);
        printf("%8s: %s\n", ctx->hash, out);
        free(out);

        ctx->hash = "SHA384";
        cipher_v1(ctx, word, &out);
        printf("%8s: %s\n", ctx->hash, out);
        free(out);

        ctx->hash = "SHA3";
        cipher_v1(ctx, word, &out);
        printf("%8s: %s\n", ctx->hash, out);
        free(out);

        ctx->hash = "BLAKE2S";
        cipher_v1(ctx, word, &out);
        printf("%8s: %s\n", ctx->hash, out);
        free(out);

        ctx->hash = "BLAKE2B";
        cipher_v1(ctx, word, &out);
        printf("%8s: %s\n", ctx->hash, out);
        free(out);
    }

    ctx->type = CIPHER_DIGIT;
    {
        ctx->hash = "MD5";
        cipher_v1(ctx, word, &out);
        printf("%8s: %s\n", ctx->hash, out);
        free(out);

        ctx->hash = "SHA1";
        cipher_v1(ctx, word, &out);
        printf("%8s: %s\n", ctx->hash, out);
        free(out);

        ctx->hash = "SHA256";
        cipher_v1(ctx, word, &out);
        printf("%8s: %s\n", ctx->hash, out);
        free(out);

        ctx->hash = "SHA224";
        cipher_v1(ctx, word, &out);
        printf("%8s: %s\n", ctx->hash, out);
        free(out);

        ctx->hash = "SHA512";
        cipher_v1(ctx, word, &out);
        printf("%8s: %s\n", ctx->hash, out);
        free(out);

        ctx->hash = "SHA384";
        cipher_v1(ctx, word, &out);
        printf("%8s: %s\n", ctx->hash, out);
        free(out);

        ctx->hash = "SHA3";
        cipher_v1(ctx, word, &out);
        printf("%8s: %s\n", ctx->hash, out);
        free(out);

        ctx->hash = "BLAKE2S";
        cipher_v1(ctx, word, &out);
        printf("%8s: %s\n", ctx->hash, out);
        free(out);

        ctx->hash = "BLAKE2B";
        cipher_v1(ctx, word, &out);
        printf("%8s: %s\n", ctx->hash, out);
        free(out);
    }

    ctx->type = CIPHER_OTHER;
    {
        ctx->hash = "MD5";
        cipher_v1(ctx, word, &out);
        printf("%8s: %s\n", ctx->hash, out);
        free(out);

        ctx->hash = "SHA1";
        cipher_v1(ctx, word, &out);
        printf("%8s: %s\n", ctx->hash, out);
        free(out);

        ctx->hash = "SHA256";
        cipher_v1(ctx, word, &out);
        printf("%8s: %s\n", ctx->hash, out);
        free(out);

        ctx->hash = "SHA224";
        cipher_v1(ctx, word, &out);
        printf("%8s: %s\n", ctx->hash, out);
        free(out);

        ctx->hash = "SHA512";
        cipher_v1(ctx, word, &out);
        printf("%8s: %s\n", ctx->hash, out);
        free(out);

        ctx->hash = "SHA384";
        cipher_v1(ctx, word, &out);
        printf("%8s: %s\n", ctx->hash, out);
        free(out);

        ctx->hash = "SHA3";
        cipher_v1(ctx, word, &out);
        printf("%8s: %s\n", ctx->hash, out);
        free(out);

        ctx->hash = "BLAKE2S";
        cipher_v1(ctx, word, &out);
        printf("%8s: %s\n", ctx->hash, out);
        free(out);

        ctx->hash = "BLAKE2B";
        cipher_v1(ctx, word, &out);
        printf("%8s: %s\n", ctx->hash, out);
        free(out);
    }
}

static void test_v2(void)
{
    char *out;

    cipher_s ctx[1];
    ctx->text = text;
    ctx->misc = misc;
    ctx->size = CIPHER_OUTSIZ;

    ctx->type = CIPHER_EMAIL;
    {
        ctx->hash = "MD5";
        cipher_v2(ctx, word, &out);
        printf("%8s: %s\n", ctx->hash, out);
        free(out);

        ctx->hash = "SHA1";
        cipher_v2(ctx, word, &out);
        printf("%8s: %s\n", ctx->hash, out);
        free(out);

        ctx->hash = "SHA256";
        cipher_v2(ctx, word, &out);
        printf("%8s: %s\n", ctx->hash, out);
        free(out);

        ctx->hash = "SHA224";
        cipher_v2(ctx, word, &out);
        printf("%8s: %s\n", ctx->hash, out);
        free(out);

        ctx->hash = "SHA512";
        cipher_v2(ctx, word, &out);
        printf("%8s: %s\n", ctx->hash, out);
        free(out);

        ctx->hash = "SHA384";
        cipher_v2(ctx, word, &out);
        printf("%8s: %s\n", ctx->hash, out);
        free(out);

        ctx->hash = "SHA3";
        cipher_v2(ctx, word, &out);
        printf("%8s: %s\n", ctx->hash, out);
        free(out);

        ctx->hash = "BLAKE2S";
        cipher_v2(ctx, word, &out);
        printf("%8s: %s\n", ctx->hash, out);
        free(out);

        ctx->hash = "BLAKE2B";
        cipher_v2(ctx, word, &out);
        printf("%8s: %s\n", ctx->hash, out);
        free(out);
    }

    ctx->type = CIPHER_DIGIT;
    {
        ctx->hash = "MD5";
        cipher_v2(ctx, word, &out);
        printf("%8s: %s\n", ctx->hash, out);
        free(out);

        ctx->hash = "SHA1";
        cipher_v2(ctx, word, &out);
        printf("%8s: %s\n", ctx->hash, out);
        free(out);

        ctx->hash = "SHA256";
        cipher_v2(ctx, word, &out);
        printf("%8s: %s\n", ctx->hash, out);
        free(out);

        ctx->hash = "SHA224";
        cipher_v2(ctx, word, &out);
        printf("%8s: %s\n", ctx->hash, out);
        free(out);

        ctx->hash = "SHA512";
        cipher_v2(ctx, word, &out);
        printf("%8s: %s\n", ctx->hash, out);
        free(out);

        ctx->hash = "SHA384";
        cipher_v2(ctx, word, &out);
        printf("%8s: %s\n", ctx->hash, out);
        free(out);

        ctx->hash = "SHA3";
        cipher_v2(ctx, word, &out);
        printf("%8s: %s\n", ctx->hash, out);
        free(out);

        ctx->hash = "BLAKE2S";
        cipher_v2(ctx, word, &out);
        printf("%8s: %s\n", ctx->hash, out);
        free(out);

        ctx->hash = "BLAKE2B";
        cipher_v2(ctx, word, &out);
        printf("%8s: %s\n", ctx->hash, out);
        free(out);
    }

    ctx->type = CIPHER_OTHER;
    {
        ctx->hash = "MD5";
        cipher_v2(ctx, word, &out);
        printf("%8s: %s\n", ctx->hash, out);
        free(out);

        ctx->hash = "SHA1";
        cipher_v2(ctx, word, &out);
        printf("%8s: %s\n", ctx->hash, out);
        free(out);

        ctx->hash = "SHA256";
        cipher_v2(ctx, word, &out);
        printf("%8s: %s\n", ctx->hash, out);
        free(out);

        ctx->hash = "SHA224";
        cipher_v2(ctx, word, &out);
        printf("%8s: %s\n", ctx->hash, out);
        free(out);

        ctx->hash = "SHA512";
        cipher_v2(ctx, word, &out);
        printf("%8s: %s\n", ctx->hash, out);
        free(out);

        ctx->hash = "SHA384";
        cipher_v2(ctx, word, &out);
        printf("%8s: %s\n", ctx->hash, out);
        free(out);

        ctx->hash = "SHA3";
        cipher_v2(ctx, word, &out);
        printf("%8s: %s\n", ctx->hash, out);
        free(out);

        ctx->hash = "BLAKE2S";
        cipher_v2(ctx, word, &out);
        printf("%8s: %s\n", ctx->hash, out);
        free(out);

        ctx->hash = "BLAKE2B";
        cipher_v2(ctx, word, &out);
        printf("%8s: %s\n", ctx->hash, out);
        free(out);
    }
}

int main(void)
{
    test_v1();
    test_v2();
}
