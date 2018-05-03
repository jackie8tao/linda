/*
 * 内核虚拟空间和用户虚拟空间管理，主要负责页分配
 *
 * @author Jackie Tao <taodingfei@gmail.com>
 * @date 2018-04-02
 */

#ifndef __LINDA_VM_H
#define __LINDA_VM_H

#include <types.h>
#include <memlayout.h>
#include <mm/mmap.h>
#include <mach.h>

#define VM_USED     1 << 0
#define VM_MAPPED   1 << 1

#define NVMBLOCKS   KERNSIZE/PGSIZE

struct vm_block{
    uint_t address;
    char status;
};

void kvm_init();

void* kvm_alloc_page();

void kvm_free_page(void*);

void* kvm_map_page(void*);

void* kvm_alloc_umapped_page();

void* kvm_unmap_page(void *addr);

void* kvm_map_page(void *addr);

#endif
