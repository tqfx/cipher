/*!
 @file cipher.h
 @brief cipher core
 @copyright Copyright (C) 2020 tqfx, All rights reserved.
*/

#ifndef __CIPHER_CIPHER_H__
#define __CIPHER_CIPHER_H__

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#define CIPHER_OUTSIZ 0x80

typedef enum cipher_e
{
    CIPHER_EMAIL,
    CIPHER_DIGIT,
    CIPHER_OTHER,
    CIPHER_TOTAL,
} cipher_e;

typedef struct cipher_s
{
    char *hint;
    char *blob;
    char *text;
    char *hash;
    unsigned int type;
    unsigned int size;
} cipher_s;

#define cipher_get_hint(ctx) (ctx)->hint
#define cipher_get_text(ctx) (ctx)->text
#define cipher_get_blob(ctx) (ctx)->blob
#define cipher_get_hash(ctx) (ctx)->hash
#define cipher_get_type(ctx) (ctx)->type
#define cipher_get_size(ctx) (ctx)->size

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

cipher_s *cipher_new(void);
void cipher_die(cipher_s *ctx);

void cipher_ctor(cipher_s *ctx);
void cipher_dtor(cipher_s *ctx);

int cipher_set_hint(cipher_s *ctx, const void *hint);
int cipher_set_blob(cipher_s *ctx, const void *blob);
int cipher_set_text(cipher_s *ctx, const void *text);
int cipher_set_hash(cipher_s *ctx, const void *hash);
void cipher_set_type(cipher_s *ctx, unsigned int type);
void cipher_set_size(cipher_s *ctx, unsigned int size);

int cipher_copy(cipher_s *ctx, const cipher_s *obj);
cipher_s *cipher_move(cipher_s *ctx, cipher_s *obj);

void cipher_v2_init(const void *s0, const void *s1, const void *s2, const void *s3);

int cipher_v1(const cipher_s *ctx, const char *word, char **out);
int cipher_v2(const cipher_s *ctx, const char *word, char **out);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* __CIPHER_CIPHER_H__ */
