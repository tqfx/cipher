/*!
 @file stream.h
 @copyright Copyright (C) 2020 tqfx, All rights reserved.
*/

#ifndef __CIPHER_STREAM_H__
#define __CIPHER_STREAM_H__

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

long stream_fsize(FILE *handle);
long stream_size(const char *fname);

int stream_fread(FILE *handle, void **pdata, size_t *nbyte);
int stream_read(const char *fname, void **pdata, size_t *nbyte);
int stream_fwrite(FILE *handle, const void *pdata, size_t nbyte);
int stream_write(const char *fname, const void *pdata, size_t nbyte);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* __CIPHER_STREAM_H__ */
