/*!
 @file stream.c
 @copyright Copyright (C) 2020 tqfx, All rights reserved.
*/

#include "cipher/stream.h"

#include <assert.h>

long stream_fsize(FILE *handle)
{
    assert(handle);
    long ret = 0;
    long seek = ftell(handle);
    if (seek < 0)
    {
        return seek;
    }
    if (((void)(ret = fseek(handle, seek, SEEK_END)), ret))
    {
        return ret;
    }
    long size = ftell(handle);
    if (size < 0)
    {
        return size;
    }
    if (((void)(ret = fseek(handle, seek, SEEK_SET)), ret))
    {
        return ret;
    }
    return size;
}

long stream_size(const char *fname)
{
    assert(fname);
    FILE *handle = fopen(fname, "rb");
    if (handle == 0)
    {
        return ~0;
    }
    long size = stream_fsize(handle);
    fclose(handle);
    return size;
}

int stream_fread(FILE *handle, void **pdata, size_t *nbyte)
{
    assert(pdata);
    assert(nbyte);
    long size = stream_fsize(handle);
    if (size < 0)
    {
        return ~0;
    }
    *pdata = malloc((size_t)size);
    if (*pdata == 0)
    {
        return ~0;
    }
    *nbyte = fread(*pdata, 1, (size_t)size, handle);
    return 0;
}

int stream_read(const char *fname, void **pdata, size_t *nbyte)
{
    assert(fname);
    int ret = ~0;
    FILE *handle = fopen(fname, "rb");
    if (handle)
    {
        ret = stream_fread(handle, pdata, nbyte);
        fclose(handle);
    }
    return ret;
}

int stream_fwrite(FILE *handle, const void *pdata, size_t nbyte)
{
    assert(handle);
    assert(!nbyte || pdata);
    if (fwrite(pdata, 1, nbyte, handle) == nbyte)
    {
        return 0;
    }
    return ~0;
}

int stream_write(const char *fname, const void *pdata, size_t nbyte)
{
    assert(fname);
    int ret = ~0;
    FILE *handle = fopen(fname, "wb");
    if (handle)
    {
        ret = stream_fwrite(handle, pdata, nbyte);
        fclose(handle);
    }
    return ret;
}
