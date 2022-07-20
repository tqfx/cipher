/*!
 @file console.h
 @brief console
 @copyright Copyright (C) 2020 tqfx, All rights reserved.
*/

#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include <stdio.h>
#include <stdarg.h>

// clang-format off
#define CONSOLE_TEXT_BLACK     0
#define CONSOLE_TEXT_RED       1
#define CONSOLE_TEXT_YELLOW    2
#define CONSOLE_TEXT_GREEN     3
#define CONSOLE_TEXT_TURQUOISE 4
#define CONSOLE_TEXT_BLUE      5
#define CONSOLE_TEXT_PURPLE    6
#define CONSOLE_TEXT_WHITE     7
#define CONSOLE_TEXT_DEFAULT   CONSOLE_TEXT_WHITE
// clang-format on

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

void console_text_reset(void);
unsigned int console_text_set(unsigned int ft, unsigned int bg);

int console_printf(unsigned int num, ...);
int console_vprint(const char *sep, unsigned int num, va_list ap);
int console_print(const char *sep, unsigned int num, ...);
int console_println(const char *sep, unsigned int num, ...);
int console_aprint(const char *sep, unsigned int num, unsigned int *x, const char **s);
int console_aprintln(const char *sep, unsigned int num, unsigned int *x, const char **s);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* __CONSOLE_H__ */
