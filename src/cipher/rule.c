/*!
 @file rule.c
 @brief cipher rule
 @copyright Copyright (C) 2020-present tqfx, All rights reserved.
*/

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
    str_ctor(ctx->r + 0);
    str_ctor(ctx->r + 1);
    str_ctor(ctx->r + 2);
    str_ctor(ctx->r + 3);
}

void rule_dtor(rule_s *ctx)
{
    str_dtor(ctx->r + 0);
    str_dtor(ctx->r + 1);
    str_dtor(ctx->r + 2);
    str_dtor(ctx->r + 3);
}

int rule_copy(rule_s *ctx, const rule_s *obj)
{
    if (str_copy(ctx->r + 0, obj->r + 0))
    {
        return FAILURE;
    }
    if (str_copy(ctx->r + 1, obj->r + 1))
    {
        return FAILURE;
    }
    if (str_copy(ctx->r + 2, obj->r + 2))
    {
        return FAILURE;
    }
    if (str_copy(ctx->r + 3, obj->r + 3))
    {
        return FAILURE;
    }
    return SUCCESS;
}

rule_s *rule_move(rule_s *ctx, rule_s *obj)
{
    memcpy(ctx, obj, sizeof(rule_s));
    memset(obj, 000, sizeof(rule_s));
    return ctx;
}

int c_rule_add(c_rule_s *ctx, rule_s *obj)
{
    rule_s *it = 0;
    c_rule_forenum(i, ctx)
    {
        it = c_rule_at(ctx, i);

        if (str_cmp(it->r + 0, obj->r + 0) &&
            str_cmp(it->r + 1, obj->r + 1) &&
            str_cmp(it->r + 2, obj->r + 2) &&
            str_cmp(it->r + 3, obj->r + 3))
        {
            rule_dtor(it);
            break;
        }
        it = 0;
    }
    if (it == 0)
    {
        it = c_rule_push(ctx);
        if (it == 0)
        {
            return FAILURE;
        }
        rule_ctor(it);
    }
    return rule_copy(it, obj);
}

int c_rule_del(c_rule_s *ctx, rule_s *obj)
{
    int ret = FAILURE;
    c_rule_foreach(it, ctx)
    {
        if (str_cmp(it->r + 0, obj->r + 0) &&
            str_cmp(it->r + 1, obj->r + 1) &&
            str_cmp(it->r + 2, obj->r + 2) &&
            str_cmp(it->r + 3, obj->r + 3))
        {
            rule_dtor(it);
            ret = SUCCESS;
        }
    }
    return ret;
}
