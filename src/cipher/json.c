/*!
 @file json.c
 @brief cipher json
 @copyright Copyright (C) 2020 tqfx, All rights reserved.
*/

#include "cipher/stream.h"
#include "cipher/json.h"

#include <assert.h>

int cipher_json_load(cJSON **out, const char *in)
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

int cipher_json_export_info(const cJSON *in, cipher_info_s *out)
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
        cipher_get_text(ctx) = cJSON_GetStringValue(object);

        object = cJSON_GetObjectItem(item, "hash");
        if (object == 0)
        {
            continue;
        }
        cipher_get_hash(ctx) = cJSON_GetStringValue(object);

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wbad-function-cast"
#endif /* __GNUC__ || __clang__ */

        object = cJSON_GetObjectItem(item, "size");
        if (object == 0)
        {
            continue;
        }
        cipher_get_size(ctx) = (unsigned int)cJSON_GetNumberValue(object);

        object = cJSON_GetObjectItem(item, "type");
        if (object == 0)
        {
            continue;
        }
        cipher_get_type(ctx) = (unsigned int)cJSON_GetNumberValue(object);

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif /* __GNUC__ || __clang__ */

        if (cipher_get_type(ctx) == CIPHER_OTHER)
        {
            object = cJSON_GetObjectItem(item, "blob");
            if (object == 0)
            {
                continue;
            }
            cipher_get_blob(ctx) = cJSON_GetStringValue(object);
        }

        object = cJSON_GetObjectItem(item, "hint");
        if (object)
        {
        }
        cipher_get_hint(ctx) = cJSON_GetStringValue(object);

        cipher_info_add(out, ctx);
    }
    return 0;
}

int cipher_json_import_info(cJSON **out, const cipher_info_s *in)
{
    assert(in);
    assert(out);
    *out = cJSON_CreateArray();
    cipher_info_foreach(it, in)
    {
        if (cipher_get_text(it) == 0)
        {
            continue;
        }
        cJSON *item = cJSON_CreateObject();
        cJSON_AddStringToObject(item, "text", cipher_get_text(it));
        cJSON_AddStringToObject(item, "hash", cipher_get_hash(it));
        cJSON_AddNumberToObject(item, "size", cipher_get_size(it));
        cJSON_AddNumberToObject(item, "type", cipher_get_type(it));
        if (cipher_get_blob(it) && cipher_get_type(it) == CIPHER_OTHER)
        {
            cJSON_AddStringToObject(item, "blob", cipher_get_blob(it));
        }
        if (cipher_get_hint(it))
        {
            cJSON_AddStringToObject(item, "hint", cipher_get_hint(it));
        }
        cJSON_AddItemToArray(*out, item);
    }
    return 0;
}
