#include "clipboard.h"

#include <string.h>

#if defined(_WIN32)
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 5105)
#endif /* _MSC_VER */
#include <windows.h>
#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */
static UINT clipboard_uformat = (sizeof(TCHAR) == sizeof(WCHAR)) ? CF_UNICODETEXT : CF_TEXT;
#endif /* _WIN32 */

int clipboard_set(void const *pdata, size_t nbyte)
{
    (void)pdata;
    (void)nbyte;
    int ret = ~0;
#if defined(_WIN32)
    do
    {
        if (OpenClipboard(0) && EmptyClipboard())
        {
            HGLOBAL hmem = GlobalAlloc(GMEM_MOVEABLE, nbyte);
            if (hmem == 0)
            {
                CloseClipboard();
                break;
            }
            LPTSTR lpdest = (LPTSTR)GlobalLock(hmem);
            memcpy(lpdest, pdata, nbyte);
            GlobalUnlock(hmem);
            SetClipboardData(clipboard_uformat, hmem);
            CloseClipboard();
            ret = 0;
        }
    } while (0);
#endif /* _WIN32 */
    return ret;
}

int clipboard_sets(void const *in)
{
    return clipboard_set(in, strlen((char const *)in) + 1);
}

int clipboard_get(char **out)
{
    (void)out;
    int ret = ~0;
#if defined(_WIN32)
    if (OpenClipboard(0))
    {
        HGLOBAL hmem = GetClipboardData(clipboard_uformat);
        if (hmem)
        {
            size_t z = strlen((char *)hmem);
            *out = (char *)malloc(z + 1);
            memcpy(*out, hmem, z);
            (*out)[z] = 0;
            ret = 0;
        }
        CloseClipboard();
    }
#endif /* _WIN32 */
    return ret;
}
