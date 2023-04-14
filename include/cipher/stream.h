/*!
 @file stream.h
*/

#ifndef CIPHER_STREAM_H
#define CIPHER_STREAM_H

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

long stream_fsize(FILE *handle);
long stream_size(char const *fname);

int stream_fread(FILE *handle, void **pdata, size_t *nbyte);
int stream_read(char const *fname, void **pdata, size_t *nbyte);
int stream_fwrite(FILE *handle, void const *pdata, size_t nbyte);
int stream_write(char const *fname, void const *pdata, size_t nbyte);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* __cplusplus */

#endif /* CIPHER_STREAM_H */
