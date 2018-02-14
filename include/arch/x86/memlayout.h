// 这里固定内存的大小，主要是为了简化初始的内存管理系统。
// 后续将会添加内存大小检测功能

#ifndef __LINDA_MEMLAYOUT_H
#define __LINDA_MEMLAYOUT_H

#include <types.h>

#define USERBASE        0x0
#define KERNLINK        0x100000            // 1M地址处开始装载内核
#define KERNBASE        0xC0000000          // 内核虚拟空间起始地址3G

#define MEMSIZE         0x10000000          // 内存的大小固定为256M

#define P2V(addr)       ((uint_t)(addr) + KERNBASE)
#define V2P(addr)       ((uint_t)(addr) - KERNBASE)

#endif