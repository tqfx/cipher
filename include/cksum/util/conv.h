/*!
 @file conv.h
 @brief convert library
 @copyright Copyright (C) 2020 tqfx, All rights reserved.
*/

#ifndef __CKSUM_UTIL_CONV_H__
#define __CKSUM_UTIL_CONV_H__

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

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
void *lower(const void *pdata, size_t nbyte, void *out);

/*!
 @brief convert a string to a upper string.
 @param[in] pdata points to data to convert.
 @param[in] nbyte length of data to convert.
 @param[in,out] out points to buffer that holds the string.
 @return a pointer to the string.
 @note When out is 0, you need to use free to release the memory.
*/
void *upper(const void *pdata, size_t nbyte, void *out);

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
void *digest(const void *pdata, size_t nbyte, unsigned int cases, void *out);
void *digest_lower(const void *pdata, size_t nbyte, void *out);
void *digest_upper(const void *pdata, size_t nbyte, void *out);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* __CKSUM_UTIL_CONV_H__ */
