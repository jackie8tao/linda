/*
 * 虚拟内存空间管理
 *
 * @author Jackie Tao <taodingfei@gmail.com>
 * @date 2018-04-02
 */

#ifndef __LINDA_VM_H
#define __LINDA_VM_H

#include <types.h>
#include <memlayout.h>
#include <mm/mmap.h>

#define VM_USED     1
#define VM_FREE     0

struct vm_block{
    uint_t address;
    char status;
};

// 初始化内核空间内存页表
void kvm_init();

// 申请内核空间页
void* kvm_alloc_page();

// 释放内核空间页
void kvm_free_page(void*);

// 初始化用户空间内存页表
void uvm_init();

// 申请用户空间页
void* uvm_alloc_page();

// 释放用户空间页
void uvm_free_page(void*);

#endif
