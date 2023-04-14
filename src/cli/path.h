#ifndef PATH_H
#define PATH_H

#include <stdlib.h>

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

char *path_self(void);
char const *path_home(void);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* __cplusplus */

#endif /* PATH_H */
