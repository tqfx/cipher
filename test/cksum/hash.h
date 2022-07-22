/*!
 @file hash.h
 @brief Header file for hash testing
 @copyright Copyright (C) 2020-present tqfx, All rights reserved.
*/

#ifndef __HASH_H__
#define __HASH_H__

#include "cksum/util/conv.h"

#include <string.h>
#include <stdio.h>

#define HASH_DIFF(src, dst, size, info)               \
    do                                                \
    {                                                 \
        if (memcmp(src, dst, size))                   \
        {                                             \
            char bsrc_[((size) << 1) + 1];            \
            char bdst_[((size) << 1) + 1];            \
            digest_upper(src, size, bsrc_);           \
            digest_upper(dst, size, bdst_);           \
            printf("%s %s %s\n", bsrc_, bdst_, info); \
        }                                             \
    } while (0)

#endif /* __HASH_H__ */
