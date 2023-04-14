#include "cipher/rule.h"

rule_s *rule_new(void)
{
    rule_s *ctx = (rule_s *)malloc(sizeof(rule_s));
    if (ctx)
    {
        rule_ctor(ctx);
    }
    return ctx;
}

void rule_die(rule_s *ctx)
{
    if (ctx)
    {
        rule_dtor(ctx);
        free(ctx);
    }
}

void rule_ctor(rule_s *ctx)
{
    a_str_ctor(ctx->r + 0);
    a_str_ctor(ctx->r + 1);
    a_str_ctor(ctx->r + 2);
    a_str_ctor(ctx->r + 3);
}

void rule_dtor(rule_s *ctx)
{
    a_str_dtor(ctx->r + 0);
    a_str_dtor(ctx->r + 1);
    a_str_dtor(ctx->r + 2);
    a_str_dtor(ctx->r + 3);
}

int rule_copy(rule_s *ctx, rule_s const *obj)
{
    if (a_str_copy(ctx->r + 0, obj->r + 0))
    {
        return A_FAILURE;
    }
    if (a_str_copy(ctx->r + 1, obj->r + 1))
    {
        return A_FAILURE;
    }
    if (a_str_copy(ctx->r + 2, obj->r + 2))
    {
        return A_FAILURE;
    }
    if (a_str_copy(ctx->r + 3, obj->r + 3))
    {
        return A_FAILURE;
    }
    return A_SUCCESS;
}

rule_s *rule_move(rule_s *ctx, rule_s *obj)
{
    memcpy(ctx, obj, sizeof(rule_s));
    memset(obj, 000, sizeof(rule_s));
    return ctx;
}

c_rule_s *c_rule_new(void)
{
    return a_vec_new(sizeof(rule_s));
}

void c_rule_die(c_rule_s *ctx)
{
    a_vec_die(ctx, (void (*)(a_vptr_t))rule_dtor);
}

int c_rule_add(c_rule_s *ctx, rule_s *obj)
{
    rule_s *it = 0;
    a_vec_forenum(i, ctx)
    {
        it = a_vec_at_(rule_s, ctx, i);

        if (a_str_cmp(it->r + 0, obj->r + 0) &&
            a_str_cmp(it->r + 1, obj->r + 1) &&
            a_str_cmp(it->r + 2, obj->r + 2) &&
            a_str_cmp(it->r + 3, obj->r + 3))
        {
            rule_dtor(it);
            break;
        }
        it = 0;
    }
    if (it == 0)
    {
        it = a_vec_push(rule_s, ctx);
        if (it == 0)
        {
            return A_FAILURE;
        }
        rule_ctor(it);
    }
    return rule_copy(it, obj);
}

int c_rule_del(c_rule_s *ctx, rule_s *obj)
{
    int ret = A_FAILURE;
    a_vec_foreach(rule_s, it, ctx)
    {
        if (a_str_cmp(it->r + 0, obj->r + 0) &&
            a_str_cmp(it->r + 1, obj->r + 1) &&
            a_str_cmp(it->r + 2, obj->r + 2) &&
            a_str_cmp(it->r + 3, obj->r + 3))
        {
            rule_dtor(it);
            ret = A_SUCCESS;
        }
    }
    return ret;
}
