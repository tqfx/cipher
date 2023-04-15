#include "cipher/info.h"
#include <string.h>
#include <stdio.h>

void c_info_ctor(c_info_s *ctx)
{
    a_avl_root(ctx->root);
    ctx->count = 0;
}

void c_info_dtor(c_info_s *ctx)
{
    a_avl_fortear(cur, next, ctx->root)
    {
        info_s *it = a_avl_entry(cur, info_s, node);
        cipher_dtor(it->cipher);
        it->time = A_I32_MIN;
        a_die(info_s, it);
    }
    ctx->count = 0;
}

static int cipher_cmp(void const *_lhs, void const *_rhs)
{
    cipher_s const *lhs = a_avl_entry(_lhs, info_s, node)->cipher;
    cipher_s const *rhs = a_avl_entry(_rhs, info_s, node)->cipher;
    return strcmp(lhs->text, rhs->text);
}

info_s *c_info_at(c_info_s *ctx, size_t idx)
{
    size_t i = 0;
    a_avl_foreach(cur, ctx->root)
    {
        if (i == idx)
        {
            return a_avl_entry(cur, info_s, node);
        }
        ++i;
    }
    return A_NULL;
}

info_s *c_info_add(c_info_s *ctx, void const *text)
{
    a_avl_foreach(cur, ctx->root)
    {
        cipher_s *it = a_avl_entry(cur, info_s, node)->cipher;
        if (cipher_get_text(it) && strcmp(cipher_get_text(it), (char const *)text) == 0)
        {
            return a_avl_entry(cur, info_s, node);
        }
    }
    info_s *it = a_new(info_s, A_NULL, 1);
    if (!it)
    {
        return it;
    }
    ++ctx->count;
    it->time = A_I32_MIN;
    cipher_ctor(it->cipher);
    cipher_set_text(it->cipher, text);
    a_avl_insert(ctx->root, it->node, cipher_cmp);
    return it;
}

info_s *c_info_del(c_info_s *ctx, void const *text)
{
    a_avl_foreach(cur, ctx->root)
    {
        info_s *it = a_avl_entry(cur, info_s, node);
        if (cipher_get_text(it->cipher) && strcmp(cipher_get_text(it->cipher), (char const *)text) == 0)
        {
            a_avl_remove(ctx->root, it->node);
            --ctx->count;
            return it;
        }
    }
    return A_NULL;
}
