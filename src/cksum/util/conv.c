/*!
 @file conv.c
 @brief convert library
 @copyright Copyright (C) 2020-present tqfx, All rights reserved.
*/

#include "cksum/util/conv.h"

#include <assert.h>
#include <ctype.h>

int xdigit(int x)
{
    int ret = ~0;
    switch (x)
    {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    {
        /* 0 ~ 9 */
        ret = x - '0';
    }
    break;
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
    {
        /* A ~ F */
        ret = x - 'A' + 10;
    }
    break;
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
    {
        /* a ~ f */
        ret = x - 'a' + 10;
    }
    break;
    default:
        break;
    }
    return ret;
}

#undef TO
#define TO(func, to)                                       \
    void *func(const void *pdata, size_t nbyte, void *out) \
    {                                                      \
        assert(!nbyte || pdata);                           \
        const char *p = (const char *)pdata;               \
        if (out || ((void)(out = malloc(nbyte + 1)), out)) \
        {                                                  \
            char *o = (char *)out;                         \
            pdata = (const void *)(p + nbyte);             \
            for (; p != pdata; ++p)                        \
            {                                              \
                *o++ = (char)to(*p);                       \
            }                                              \
            *o = 0;                                        \
        }                                                  \
        return out;                                        \
    }
TO(lower, tolower)
TO(upper, toupper)
#undef TO

void *digest(const void *pdata, size_t nbyte, unsigned int cases, void *out)
{
    static const char hexits[2][0x10] = {
        /* clang-format off */
        {
            '0', '1', '2', '3', '4', '5', '6', '7',
            '8', '9', 'a', 'b', 'c', 'd', 'e', 'f',
        },
        {
            '0', '1', '2', '3', '4', '5', '6', '7',
            '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
        },
        /* clang-format on */
    };

    assert(!nbyte || pdata);

    const char *hexit = hexits[cases % 2];
    const unsigned char *p = (const unsigned char *)pdata;
    if (out || ((void)(out = malloc((nbyte << 1) + 1)), out))
    {
        char *o = (char *)out;
        pdata = (const void *)(p + nbyte);
        while (p != pdata)
        {
            *o++ = hexit[*p >> 0x4];
            *o++ = hexit[*p & 0x0F];
            ++p;
        }
        *o = 0;
    }

    return out;
}

void *digest_lower(const void *pdata, size_t nbyte, void *out)
{
    return digest(pdata, nbyte, 0, out);
}

void *digest_upper(const void *pdata, size_t nbyte, void *out)
{
    return digest(pdata, nbyte, 1, out);
}
