#ifndef __LINDA_KPRINTF_H
#define __LINDA_KPRINTF_H

#include <stdarg.h>

void kprintf(char *fmt, ...);

void panic(char *msg, ...);

#endif