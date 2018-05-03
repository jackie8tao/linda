#ifndef __LINDA_KPRINTF_H
#define __LINDA_KPRINTF_H

#include <stdarg.h>

int sprintf(char *buf, const char *fmt, ...);

int kprintf(const char *fmt, ...);

void panic(const char *fmt, ...);

#endif
