/*!
 @file info.c
 @brief Testing cipher infomation
 @copyright Copyright (C) 2020 tqfx, All rights reserved.
*/

#include "cipher/info.h"

#include <stdio.h>

static void test(size_t n)
{
    cipher_info_s *ctx = cipher_info_new();

    for (size_t i = 0; i != n; ++i)
    {
        cipher_s *obj = cipher_info_push(ctx);
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
        cipher_set_blob(obj, buf);
    }

    cipher_info_die(ctx);
}

int main(void)
{
    test(0xFF);
    return 0;
}
