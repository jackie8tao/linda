// 这里固定内存的大小，主要是为了简化初始的内存管理系统。
// 后续将会添加内存大小检测功能

#ifndef __LINDA_MEMLAYOUT_H
#define __LINDA_MEMLAYOUT_H

#include <types.h>

#define EXTMEM          0x100000                                // 扩展内存开始位置
#define KERNBASE        0xC0000000                              // 内核虚拟空间起始地址3G
#define KERNSIZE        0x40000000                              // 内核空间大小
#define KERNLINK        (KERNBASE+EXTMEM)                       // 内核链接位置
#define USERSIZE        0xC0000000                              // 用户空间大小

#define P2V(addr)       ((uint_t)(addr) + KERNBASE)             // 内核空间物理地址转为虚拟地址
#define V2P(addr)       ((uint_t)(addr) - KERNBASE)             // 内核空间虚拟地址转为物理地址

#endif
