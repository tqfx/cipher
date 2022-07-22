/*!
 @file json.c
 @brief Testing cipher json
 @copyright Copyright (C) 2020-present tqfx, All rights reserved.
*/

#include "cipher/json.h"

#include <stdio.h>

int main(void)
{
    c_info_s *info = c_info_new();
    cipher_s *key = c_info_push(info);
    cipher_set_text(key, "0");
    key = c_info_push(info);
    cipher_set_text(key, "1");
    cipher_set_size(key, 6);
    cipher_set_type(key, CIPHER_DIGIT);
    key = c_info_push(info);
    cipher_set_text(key, "2");
    cipher_set_type(key, CIPHER_OTHER);
    cipher_set_misc(key, "misc");
    cipher_set_hint(key, "hint");
    cJSON *json = 0;
    c_json_import_info(&json, info);
    c_info_dtor(info);
    c_info_ctor(info);
    c_json_export_info(json, info);
    cJSON_Delete(json);
    c_json_import_info(&json, info);
    c_info_die(info);
    cJSON_Delete(json);
    return 0;
}
