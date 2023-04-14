#ifndef CLIPBOARD_H
#define CLIPBOARD_H

#include <stdlib.h>

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*!
 @brief Set data to clipboard
 @param[in] pdata points to data
 @param[in] nbyte length of data
 @return the execution state of the function
  @retval -1 failure
  @retval 0 success
*/
int clipboard_set(void const *pdata, size_t nbyte);

/*!
 @brief Set string to clipboard
 @param[in] in points to string
 @return the execution state of the function
  @retval -1 failure
  @retval 0 success
*/
int clipboard_sets(void const *in);

/*!
 @brief Get data of clipboard
 @param[out] out points to data address
 @return the execution state of the function
  @retval -1 failure
  @retval 0 success
*/
int clipboard_get(char **out);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* __cplusplus */

#endif /* CLIPBOARD_H */
