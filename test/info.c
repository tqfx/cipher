#include "cipher/info.h"
#include <stdio.h>

static void test(size_t n)
{
    c_info_s root[1] = {C_INFO_INIT};

    for (size_t i = 0; i != n; ++i)
    {
        char buf[0x20];
        sprintf(buf, "%zu", i);
        info_s *ctx = c_info_add(root, buf);
        cipher_set_hash(ctx->cipher, buf);
        cipher_set_text(ctx->cipher, buf);
        cipher_set_hint(ctx->cipher, buf);
        cipher_set_misc(ctx->cipher, buf);
    }

    c_info_dtor(root);
}

int main(void)
{
    test(0xFF);
    return 0;
}
