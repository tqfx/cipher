#include "cipher/word.h"
#include <stdio.h>

static void test(size_t n)
{
    c_word_s *ctx = c_word_new();

    for (size_t i = 0; i != n; ++i)
    {
        a_str_s *obj = c_word_push(ctx);
        a_str_printf(obj, "%zu", i);
    }

    c_word_die(ctx);
}

int main(void)
{
    test(0xFF);
    return 0;
}
