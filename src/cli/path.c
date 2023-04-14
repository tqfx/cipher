#include "path.h"

#if defined(_WIN32)
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 5105)
#endif /* _MSC_VER */
#include <windows.h>
#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */
#else /* !_WIN32 */
#include <unistd.h>
#endif /* _WIN32 */

char *path_self(void)
{
    char *self = 0;
    size_t length = 0;
    size_t size = 0x10;
    do
    {
        size <<= 1;
        char *ptr = (char *)realloc(self, size);
        if (ptr == 0)
        {
            break;
        }
        self = ptr;
#if defined(_WIN32)
        length = GetModuleFileName(NULL, self, (DWORD)(size - 1));
#else /* !_WIN32 */
        length = (size_t)readlink("/proc/self/exe", self, size - 1);
#endif /* _WIN32 */
    } while (length >= size - 1);
    if (self)
    {
        self[length] = 0;
    }
    return self;
}

char const *path_home(void)
{
    char const *home = getenv("HOME");
    if (home)
    {
        return home;
    }
    home = getenv("USERPROFILE");
    if (home)
    {
        return home;
    }
    return ".";
}
