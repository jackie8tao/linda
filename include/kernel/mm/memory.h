/*
 * 物理内存管理机制，按页分配。
 *
 * @author Jackie Tao <taodingfei@gmail.com>
 * @date 2018-03-21 16:00
 */

#ifndef __LINDA_MEMORY_H
#define __LINDA_MEMORY_H

#include <multiboot.h>

void show_memory_map();

void phy_mem_init();

void* phy_mem_alloc();

void phy_mem_free(void *addr);

void* virt_kern_addr();

#endif
