/*!
 @file word.c
 @brief Testing cipher word
 @copyright Copyright (C) 2020 tqfx, All rights reserved.
*/

#include "cipher/word.h"

#include <stdio.h>

static void test(size_t n)
{
    cipher_word_s *ctx = cipher_word_new();

    for (size_t i = 0; i != n; ++i)
    {
        str_s *obj = cipher_word_push(ctx);
        str_printf(obj, "%zu", i);
    }

    cipher_word_die(ctx);
}

int main(void)
{
    test(0xFF);
    return 0;
}
