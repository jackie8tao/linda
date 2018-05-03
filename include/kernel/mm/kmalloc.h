/*
 * 内核动态内存管理。
 * 主要处理内核中小内存片管理。
 *
 * @author Jackie Tao <taodingfei@gmail.com>
 * @date 2018-03-21 22:40
 */

#ifndef __LINDA_KALLOC_H
#define __LINDA_KALLOC_H

#include <types.h>

void* kmalloc(uint_t size);

void kmfree(void* addr);

void print_chunks();

#endif
