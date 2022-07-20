/*!
 @file json.c
 @brief Testing cipher json
 @copyright Copyright (C) 2020 tqfx, All rights reserved.
*/

#include "cipher/json.h"

#include <stdio.h>

int main(void)
{
    cipher_info_s *info = cipher_info_new();
    cipher_s *key = cipher_info_push(info);
    cipher_set_text(key, "0");
    key = cipher_info_push(info);
    cipher_set_text(key, "1");
    cipher_set_size(key, 6);
    cipher_set_type(key, CIPHER_DIGIT);
    key = cipher_info_push(info);
    cipher_set_text(key, "2");
    cipher_set_type(key, CIPHER_OTHER);
    cipher_set_blob(key, "blob");
    cipher_set_hint(key, "hint");
    cJSON *json = 0;
    cipher_json_import_info(&json, info);
    cipher_info_dtor(info);
    cipher_info_ctor(info);
    cipher_json_export_info(json, info);
    cJSON_Delete(json);
    cipher_json_import_info(&json, info);
    cipher_info_die(info);
    cJSON_Delete(json);
    return 0;
}
