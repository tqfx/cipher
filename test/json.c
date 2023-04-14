#include "cipher/json.h"
#include <stdio.h>

int main(void)
{
    info_s *item;
    c_info_s info[1] = {C_INFO_INIT};
    item = c_info_add(info, "0");
    item = c_info_add(info, "1");
    cipher_set_size(item->cipher, 6);
    cipher_set_type(item->cipher, CIPHER_DIGIT);
    item = c_info_add(info, "2");
    cipher_set_text(item->cipher, "2");
    cipher_set_type(item->cipher, CIPHER_OTHER);
    cipher_set_misc(item->cipher, "misc");
    cipher_set_hint(item->cipher, "hint");
    cJSON *json = 0;
    c_json_import_info(&json, info);
    {
        char *str = cJSON_Print(json);
        puts(str);
        free(str);
    }
    c_info_dtor(info);
    c_json_export_info(json, info);
    cJSON_Delete(json);
    c_json_import_info(&json, info);
    c_info_dtor(info);
    cJSON_Delete(json);
    return 0;
}
