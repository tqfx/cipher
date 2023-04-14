#include "cipher/word.h"

c_word_s *c_word_new(void)
{
    return a_vec_new(sizeof(a_str_s));
}

void c_word_die(c_word_s *ctx)
{
    a_vec_die(ctx, (void (*)(a_vptr_t))a_str_dtor);
}

void c_word_ctor(c_word_s *ctx)
{
    a_vec_ctor(ctx, sizeof(a_str_s));
}

void c_word_dtor(c_word_s *ctx)
{
    a_vec_dtor(ctx, (void (*)(a_vptr_t))a_str_dtor);
}

int c_word_add(c_word_s *ctx, a_str_s *obj)
{
    a_str_s *it = 0;
    a_vec_forenum(i, ctx)
    {
        it = a_vec_at_(a_str_s, ctx, i);
        if (a_str_ptr(it) && strcmp(a_str_ptr(it), a_str_ptr(obj)) == 0)
        {
            a_str_dtor(it);
            break;
        }
        it = 0;
    }
    if (it == 0)
    {
        it = a_vec_push(a_str_s, ctx);
        a_str_ctor(it);
        if (it == 0)
        {
            return A_FAILURE;
        }
    }
    return a_str_copy(it, obj);
}

int c_word_del(c_word_s *ctx, a_str_s *obj)
{
    int ret = A_FAILURE;
    a_vec_foreach(a_str_s, it, ctx)
    {
        if (a_str_ptr(it) && strcmp(a_str_ptr(it), a_str_ptr(obj)) == 0)
        {
            a_str_dtor(it);
            ret = A_SUCCESS;
        }
    }
    return ret;
}
