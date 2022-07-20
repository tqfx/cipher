/*!
 @file str.h
 @brief basic string library
 @copyright Copyright (C) 2020 tqfx, All rights reserved.
*/

#ifndef __STR_H__
#define __STR_H__

/* MinGW */
#if defined(__MINGW32__)
#ifndef __USE_MINGW_ANSI_STDIO
#define __USE_MINGW_ANSI_STDIO 1
#endif /* __USE_MINGW_ANSI_STDIO */
#ifndef __format__
#define __format__(...)
#endif /* __format__ */
#endif /* __MINGW32__ */

/* fallback for __has_attribute */
#ifndef __has_attribute
#define __has_attribute(...) 0
#define __attribute__(...)
#endif /* __has_attribute */

#include <stddef.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

/*!
 @addtogroup STR basic string library
 @{
*/

/*!
 @brief instance structure for basic string
*/
typedef struct str_s
{
    size_t __mem; /*!< memory */
    size_t __num; /*!< length */
    char *__str; /*!< string */
} str_s;

#ifndef STR_NIL
// clang-format off
#define STR_NIL {0, 0, 0}
// clang-format on
#endif /* STR_NIL */

/*!
 @brief memory for a pointer to string structure
 @param[in] ctx points to an instance of string structure
 @return size of memory
*/
static inline size_t str_mem(const str_s *ctx) { return ctx->__mem; }

/*!
 @brief length for a pointer to string structure
 @param[in] ctx points to an instance of string structure
 @return size of length
*/
static inline size_t str_len(const str_s *ctx) { return ctx->__num; }

/*!
 @brief string for a pointer to string structure
 @param[in] ctx points to an instance of string structure
 @return string
*/
static inline char *str_val(const str_s *ctx) { return ctx->__str; }

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*!
 @brief allocate a pointer to string structure from memory
*/
str_s *str_new(void);

/*!
 @brief deallocate a pointer to string structure
 @param[in] ctx points to an instance of string structure
*/
void str_die(str_s *ctx);

/*!
 @brief constructor for string structure
 @param[in] ctx points to an instance of string structure
*/
void str_ctor(str_s *ctx);

/*!
 @brief destructor for string structure
 @param[in] ctx points to an instance of string structure
*/
void str_dtor(str_s *ctx);

/*!
 @brief initialize a pointer to string structure
 @param[in] ctx points to an instance of string structure
 @param[in] pdata points to data to initialize
 @param[in] nbyte length of data to initialize
 @return the execution state of the function
  @retval -1 failure
  @retval 0 success
*/
int str_init(str_s *ctx, const void *pdata, size_t nbyte);

/*!
 @brief initialize a pointer to string structure by copying
 @param[in] ctx points to an instance of string structure
 @param[in] str input source pointing to an instance
 @return the execution state of the function
  @retval -1 failure
  @retval 0 success
*/
int str_copy(str_s *ctx, const str_s *str);

/*!
 @brief initialize a pointer to string structure by moving
 @param[in] ctx points to an instance of string structure
 @param[in] str input source pointing to an instance
*/
str_s *str_move(str_s *ctx, str_s *str);

/*!
 @brief terminate a pointer to string structure
 @param[in] ctx points to an instance of string structure
 @return string of string structure
 @note need to use free to release this memory
*/
char *str_exit(str_s *ctx);

/*!
 @brief resize memory for a pointer to string structure
 @param[in] ctx points to an instance of string structure
 @param[in] mem length of real memory
 @return the execution state of the function
  @retval -1 failure
  @retval 0 success
*/
int str_resize(str_s *ctx, size_t mem);
int str_resize_(str_s *ctx, size_t mem);

/*!
 @brief put character to an instance of string structure
 @param[in] ctx points to an instance of string structure
 @param[in] c character
 @return character
  @retval -1 failure
*/
int str_putc(str_s *ctx, int c);
int str_putc_(str_s *ctx, int c);

/*!
 @brief put data to an instance of string structure
 @param[in] ctx points to an instance of string structure
 @param[in] pdata points to data to put
 @param[in] nbyte length of data to put
 @return the execution state of the function
  @retval -1 failure
  @retval 0 success
*/
int str_putn(str_s *ctx, const void *pdata, size_t nbyte);
int str_putn_(str_s *ctx, const void *pdata, size_t nbyte);

/*!
 @brief put string to an instance of string structure
 @param[in] ctx points to an instance of string structure
 @param[in] str string terminated with a null character
 @return the execution state of the function
  @retval -1 failure
  @retval 0 success
*/
int str_puts(str_s *ctx, const void *str);

/*!
 @brief print string to a pointer to string structure
 @param[in] ctx points to an instance of string structure
 @param[in] fmt format of string to be printed
 @param[in] va instance of variable argument
 @return number of printed characters
  @retval -1 failure
*/
int str_vprintf(str_s *ctx, const char *fmt, va_list va) __attribute__((__format__(__printf__, 2, 0)));

/*!
 @brief print string to a pointer to string structure
 @param[in] ctx points to an instance of string structure
 @param[in] fmt format of string to be printed
 @return number of printed characters
  @retval -1 failure
*/
int str_printf(str_s *ctx, const char *fmt, ...) __attribute__((__format__(__printf__, 2, 3)));

#if defined(__cplusplus)
}
#endif /* __cplusplus */

/*! @} STR */

#endif /* __STR_H__ */
