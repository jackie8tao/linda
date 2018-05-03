/*
 * 虚拟内存管理： 申请虚拟内存页，释放虚拟内存页，
 * 映射虚拟内存，解除映射虚拟内存。
 *
 * @author Jackie Tao <taodingfei@gmail.com>
 * @date 2018-03-21 17:34
 */

#ifndef __LINDA_MMAP_H
#define __LINDA_MMAP_H

#include <types.h>
#include <process.h>
#include <mach.h>

#define MEM_NORMAL          0x38000000                  // 16～896M
#define MEM_HIGH            0x40000000                  // 896～1G

#define MEM_KERN            1 << 0                      // 内核空间
#define MEM_USER            1 << 1                      // 用户空间

struct mmap_block{
    uint_t virt_addr;
    uint_t phy_addr;
    char mapped;
    struct mmap_block *next;
};

void kmap_init();

void mmap_do_map(pde_t*, uint_t, uint_t, uint_t);

void mmap_do_unmap(pde_t*, uint_t);

pde_t* umap_init();

void switch_to_uvm(struct process*);

void switch_to_kvm();

void segment_init();

pde_t* uvm_init(pde_t*, char*, uint_t);

#endif
