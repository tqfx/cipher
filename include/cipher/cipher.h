/*!
 @file cipher.h
 @brief cipher core
*/

#ifndef CIPHER_CIPHER_H
#define CIPHER_CIPHER_H

#include "a/a.h"
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
    char *text;
    char *hash;
    char *hint;
    char *misc;
    unsigned int type;
    unsigned int size;
} cipher_s;

#define cipher_get_time(ctx) (ctx)->time
#define cipher_get_text(ctx) (ctx)->text
#define cipher_get_hash(ctx) (ctx)->hash
#define cipher_get_hint(ctx) (ctx)->hint
#define cipher_get_misc(ctx) (ctx)->misc
#define cipher_get_type(ctx) (ctx)->type
#define cipher_get_size(ctx) (ctx)->size

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*!
 @brief convert hexadecimal to decimal.
 @param[in] x '0'-'9', 'a'-'f', 'A'-'F'
 @return 0 ~ 15
  @retval -1 failure
*/
int xdigit(int x);

/*!
 @brief convert a string to a lower string.
 @param[in] pdata points to data to convert.
 @param[in] nbyte length of data to convert.
 @param[in,out] out points to buffer that holds the string.
 @return a pointer to the string.
 @note When out is 0, you need to use free to release the memory.
*/
void *lower(void const *pdata, size_t nbyte, void *out);

/*!
 @brief convert a string to a upper string.
 @param[in] pdata points to data to convert.
 @param[in] nbyte length of data to convert.
 @param[in,out] out points to buffer that holds the string.
 @return a pointer to the string.
 @note When out is 0, you need to use free to release the memory.
*/
void *upper(void const *pdata, size_t nbyte, void *out);

/*!
 @brief convert a digest to a string.
 @param[in] pdata points to data to convert.
 @param[in] nbyte length of data to convert.
 @param[in] cases select the converted case.
  @arg 0 lower
  @arg 1 upper
 @param[in,out] out points to buffer that holds the string.
 @return a pointer to the string.
 @note When out is 0, you need to use free to release the memory.
*/
void *digest(void const *pdata, size_t nbyte, unsigned int cases, void *out);
void *digest_lower(void const *pdata, size_t nbyte, void *out);
void *digest_upper(void const *pdata, size_t nbyte, void *out);

cipher_s *cipher_new(void);
void cipher_die(cipher_s *ctx);

void cipher_ctor(cipher_s *ctx);
void cipher_dtor(cipher_s *ctx);

int cipher_set_hint(cipher_s *ctx, void const *hint);
int cipher_set_misc(cipher_s *ctx, void const *misc);
int cipher_set_text(cipher_s *ctx, void const *text);
int cipher_set_hash(cipher_s *ctx, void const *hash);
void cipher_set_type(cipher_s *ctx, unsigned int type);
void cipher_set_size(cipher_s *ctx, unsigned int size);

int cipher_copy(cipher_s *ctx, cipher_s const *obj);
cipher_s *cipher_move(cipher_s *ctx, cipher_s *obj);

void cipher_v2_init(void const *s0, void const *s1, void const *s2, void const *s3);

int cipher_v1(cipher_s const *ctx, char const *word, char **out);
int cipher_v2(cipher_s const *ctx, char const *word, char **out);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* __cplusplus */

#endif /* CIPHER_CIPHER_H */
