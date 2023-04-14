/*!
 @file rule.h
 @brief rule rule
*/

#ifndef CIPHER_RULE_H
#define CIPHER_RULE_H

#include "a/host/vec.h"
#include "a/host/str.h"

typedef a_vec_s c_rule_s;
typedef struct rule_s
{
    a_str_s r[4];
} rule_s;

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

rule_s *rule_new(void);
void rule_die(rule_s *ctx);
void rule_ctor(rule_s *ctx);
void rule_dtor(rule_s *ctx);

int rule_copy(rule_s *ctx, rule_s const *obj);
rule_s *rule_move(rule_s *ctx, rule_s *obj);

c_rule_s *c_rule_new(void);
void c_rule_die(c_rule_s *ctx);

int c_rule_add(c_rule_s *ctx, rule_s *obj);
int c_rule_del(c_rule_s *ctx, rule_s *obj);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* __cplusplus */

static inline rule_s *c_rule_push(c_rule_s *ctx)
{
    rule_s *obj = a_vec_push(rule_s, ctx);
    rule_ctor(obj);
    return obj;
}

static inline rule_s *c_rule_pull(c_rule_s *ctx)
{
    return a_vec_pull(rule_s, ctx);
}

#endif /* CIPHER_RULE_H */
