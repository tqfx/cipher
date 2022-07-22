/*!
 @file word.c
 @brief cipher word
 @copyright Copyright (C) 2020-present tqfx, All rights reserved.
*/

#include "cipher/word.h"

int c_word_add(c_word_s *ctx, str_s *obj)
{
    str_s *it = 0;
    c_word_forenum(i, ctx)
    {
        it = c_word_at(ctx, i);
        if (str_val(it) && strcmp(str_val(it), str_val(obj)) == 0)
        {
            str_dtor(it);
            break;
        }
        it = 0;
    }
    if (it == 0)
    {
        it = c_word_push(ctx);
        str_ctor(it);
        if (it == 0)
        {
            return FAILURE;
        }
    }
    return str_copy(it, obj);
}

int c_word_del(c_word_s *ctx, str_s *obj)
{
    int ret = FAILURE;
    c_word_foreach(it, ctx)
    {
        if (str_val(it) && strcmp(str_val(it), str_val(obj)) == 0)
        {
            str_dtor(it);
            ret = SUCCESS;
        }
    }
    return ret;
}
