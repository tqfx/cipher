/*!
 @file info.c
 @brief Testing cipher infomation
 @copyright Copyright (C) 2020-present tqfx, All rights reserved.
*/

#include "cipher/info.h"

#include <stdio.h>

static void test(size_t n)
{
    c_info_s *ctx = c_info_new();

    for (size_t i = 0; i != n; ++i)
    {
        cipher_s *obj = c_info_push(ctx);
        if (obj == 0)
        {
            continue;
        }
        char buf[0x20];
        cipher_ctor(obj);
        sprintf(buf, "%zu", i);
        cipher_set_hash(obj, buf);
        cipher_set_text(obj, buf);
        cipher_set_hint(obj, buf);
        cipher_set_misc(obj, buf);
    }

    c_info_die(ctx);
}

int main(void)
{
    test(0xFF);
    return 0;
}
