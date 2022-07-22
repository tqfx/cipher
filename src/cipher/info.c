/*!
 @file info.c
 @brief cipher infomation
 @copyright Copyright (C) 2020-present tqfx, All rights reserved.
*/

#include "cipher/info.h"

int c_info_add(c_info_s *ctx, cipher_s *obj)
{
    cipher_s *it = 0;
    c_info_forenum(i, ctx)
    {
        it = c_info_at(ctx, i);
        if (cipher_get_text(it) && strcmp(cipher_get_text(it), cipher_get_text(obj)) == 0)
        {
            cipher_dtor(it);
            break;
        }
        it = 0;
    }
    if (it == 0)
    {
        it = c_info_push(ctx);
        if (it == 0)
        {
            return FAILURE;
        }
        cipher_ctor(it);
    }
    return cipher_copy(it, obj);
}

int c_info_del(c_info_s *ctx, cipher_s *obj)
{
    int ret = FAILURE;
    c_info_foreach(it, ctx)
    {
        if (cipher_get_text(it) && strcmp(cipher_get_text(it), cipher_get_text(obj)) == 0)
        {
            cipher_dtor(it);
            ret = SUCCESS;
        }
    }
    return ret;
}
