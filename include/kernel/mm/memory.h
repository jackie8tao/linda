/*
 * 物理内存管理机制，按页分配。
 * 
 * @author Jackie Tao <taodingfei@gmail.com>
 * @date 2018-03-21 16:00
 */

#ifndef __LINDA_MEMORY_H
#define __LINDA_MEMORY_H

#include <multiboot.h>

#define PAGE_SIZE 4096

#define PAGE_ROUND_DOWN(addr)   ((addr) & ~(PAGE_SIZE-1))
#define PAGE_ROUND_UP(addr)     (((addr) + PAGE_SIZE-1) & ~(PAGE_SIZE-1))

// 输出 BIOS 提供的物理内存布局
void show_memory_map();

// 物理内存管理初始化
void phy_mem_init();

// 物理内存页申请
void* phy_mem_alloc();

// 物理内存页释放
void phy_mem_free(void *addr);

#endif