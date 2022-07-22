/*!
 @file hmac.c
 @brief hmac library utils
 @copyright Copyright (C) 2020-present tqfx, All rights reserved.
*/

#include "cksum/util/hmac.h"

#include "../hash.h"

#include <assert.h>
#include <stdarg.h>

int hmac_memory(const hash_s *hash, const void *pkey, size_t nkey, const void *pmsg, size_t nmsg, void *out, size_t *siz)
{
    assert(out);
    assert(siz);
    assert(hash);
    assert(!nkey || pkey);
    assert(!nmsg || pmsg);

    if (*siz < hash->outsiz)
    {
        *siz = hash->outsiz;
        return OVERFLOW;
    }

    hmac_s hmac[1];

    if (hmac_init(hmac, hash, pkey, nkey) != SUCCESS)
    {
        return FAILURE;
    }
    if (hmac_proc(hmac, pmsg, nmsg) != SUCCESS)
    {
        return FAILURE;
    }
    *siz = hmac_done(hmac, out) ? hash->outsiz : 0;

    return SUCCESS;
}

int hmac_mmulti(const hash_s *hash, void *out, size_t *siz, const void *pkey, size_t nkey, const void *pmsg, size_t nmsg, ...)
{
    assert(out);
    assert(siz);
    assert(hash);
    assert(!nkey || pkey);
    assert(!nmsg || pmsg);

    if (*siz < hash->outsiz)
    {
        *siz = hash->outsiz;
        return OVERFLOW;
    }

    va_list arg;
    hmac_s hmac[1];
    int ret = FAILURE;
    va_start(arg, nmsg);

    if (hmac_init(hmac, hash, pkey, nkey) != SUCCESS)
    {
        goto done;
    }
    for (;;)
    {
        ret = hmac_proc(hmac, pmsg, nmsg);
        if (ret != SUCCESS)
        {
            goto done;
        }
        pmsg = va_arg(arg, const void *);
        if (pmsg == 0)
        {
            break;
        }
        nmsg = va_arg(arg, size_t);
    }
    *siz = hmac_done(hmac, out) ? hash->outsiz : 0;

done:
    va_end(arg);
    return ret;
}

int hmac_filehandle(const hash_s *hash, const void *pkey, size_t nkey, FILE *in, void *out, size_t *siz)
{
    assert(in);
    assert(out);
    assert(siz);
    assert(hash);
    assert(!nkey || pkey);

    if (*siz < hash->outsiz)
    {
        *siz = hash->outsiz;
        return OVERFLOW;
    }

    char buf[BUFSIZ];
    hmac_s hmac[1];

    if (hmac_init(hmac, hash, pkey, nkey) != SUCCESS)
    {
        return FAILURE;
    }
    do
    {
        *siz = fread(buf, 1, BUFSIZ, in);
        hmac_proc(hmac, buf, *siz);
    } while (*siz == BUFSIZ);
    *siz = hmac_done(hmac, out) ? hash->outsiz : 0;

    return SUCCESS;
}

int hmac_file(const hash_s *hash, const void *pkey, size_t nkey, const char *fname, void *out, size_t *siz)
{
    assert(out);
    assert(siz);
    assert(hash);
    assert(fname);
    assert(!nkey || pkey);

    if (*siz < hash->outsiz)
    {
        *siz = hash->outsiz;
        return OVERFLOW;
    }

    FILE *in = fopen(fname, "rb");
    if (in == 0)
    {
        return NOTFOUND;
    }

    int ret = hmac_filehandle(hash, pkey, nkey, in, out, siz);

    if (fclose(in))
    {
        return FAILURE;
    }

    return ret;
}
