/*
 * 内核使用的基础类型
 */

#ifndef __LINDA_TYPES_H
#define __LINDA_TYPES_H

#ifndef _LINDA_KERNEL_TYPES
#define _LINDA_KERNEL_TYPES
    typedef unsigned char   uchar_t;
    typedef unsigned short  ushort_t;
    typedef unsigned int    uint_t;
    typedef unsigned long   ulong_t;
#endif

#ifndef _LINDA_SIZE_T
    typedef unsigned int size_t;
#endif

#ifndef NULL
#define NULL ((void*)0)
#endif

#endif