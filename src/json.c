#include "cipher/stream.h"
#include "cipher/json.h"
#include <assert.h>
#include <time.h>

int c_json_load(cJSON **out, char const *in)
{
    assert(in);
    assert(out);
    int ret = ~0;
    void *pdata = 0;
    size_t nbyte = 0;
    ret = stream_read(in, &pdata, &nbyte);
    if (ret == 0)
    {
        *out = cJSON_ParseWithLength((char *)pdata, nbyte);
        free(pdata);
    }
    return ret;
}

int c_json_export_info(cJSON const *in, c_info_s *out)
{
    assert(in);
    assert(out);
    cipher_s ctx[1];
    cJSON *object;
    int n = cJSON_GetArraySize(in);
    for (int i = 0; i != n; ++i)
    {
        cipher_ctor(ctx);
        cJSON *item = cJSON_GetArrayItem(in, i);

        object = cJSON_GetObjectItem(item, "text");
        if (object == 0)
        {
            continue;
        }
        ctx->text = cJSON_GetStringValue(object);

        object = cJSON_GetObjectItem(item, "hash");
        ctx->hash = object ? cJSON_GetStringValue(object) : "MD5";

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wbad-function-cast"
#endif /* __GNUC__ || __clang__ */

        object = cJSON_GetObjectItem(item, "size");
        if (object == 0)
        {
            continue;
        }
        ctx->size = (unsigned int)cJSON_GetNumberValue(object);

        object = cJSON_GetObjectItem(item, "type");
        if (object == 0)
        {
            continue;
        }
        ctx->type = (unsigned int)cJSON_GetNumberValue(object);

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif /* __GNUC__ || __clang__ */

        if (cipher_get_type(ctx) == CIPHER_OTHER)
        {
            object = cJSON_GetObjectItem(item, "misc");
            if (object == 0)
            {
                continue;
            }
            cipher_get_misc(ctx) = cJSON_GetStringValue(object);
        }

        object = cJSON_GetObjectItem(item, "hint");
        ctx->hint = cJSON_GetStringValue(object);

        info_s *info = c_info_add(out, ctx);
        cipher_copy(info->cipher, ctx);

        object = cJSON_GetObjectItem(item, "time");
        info->time = object ? (a_i64_t)cJSON_GetNumberValue(object) : time(NULL) + LONG_MIN;
    }
    return 0;
}

int c_json_import_info(cJSON **out, c_info_s const *in)
{
    assert(in);
    assert(out);
    *out = cJSON_CreateArray();
    a_avl_foreach(cur, in->root)
    {
        info_s *info = a_avl_entry(cur, info_s, node);
        cipher_s *it = a_avl_entry(cur, info_s, node)->cipher;
        if (cipher_get_text(it) == 0)
        {
            continue;
        }
        cJSON *item = cJSON_CreateObject();
        cJSON_AddStringToObject(item, "text", cipher_get_text(it));
        cJSON_AddStringToObject(item, "hash", cipher_get_hash(it));
        cJSON_AddNumberToObject(item, "size", cipher_get_size(it));
        cJSON_AddNumberToObject(item, "type", cipher_get_type(it));
        if (cipher_get_misc(it) && cipher_get_type(it) == CIPHER_OTHER)
        {
            cJSON_AddStringToObject(item, "misc", cipher_get_misc(it));
        }
        if (cipher_get_hint(it))
        {
            cJSON_AddStringToObject(item, "hint", cipher_get_hint(it));
        }
        cJSON_AddNumberToObject(item, "time", info->time);
        cJSON_AddItemToArray(*out, item);
    }
    return 0;
}
