/*
 * 内核初始化阶段，建立临时的虚拟内存映射。
 * 在建立完整的内存管理机制之后，将放弃。
 * 
 * @author Jackie Tao <taodingfei@gmail.com>
 * @date 2018-03-21 13:40
 */

#ifndef __LINDA_BOOTMEM_H
#define __LINDA_BOOTMEM_H

#include <types.h>
#include <memlayout.h>
#include <mm/mmap.h>


// 这里是简单的线性映射，virt_addr = phy_addr + KERNBASE
__attribute__((aligned(PDSIZE))) 
pde_t boot_pgdir[NPDENTRIES] = {
    [0] = (0x0) | PDE_P | PDE_RW | PDE_4M,
    [1] = (0x400000) | PDE_P | PDE_RW | PDE_4M,
    [2] = (0x800000) | PDE_P | PDE_RW | PDE_4M,
    [PDX(KERNBASE)] = (0x0) | PDE_P | PDE_RW | PDE_4M,
    [PDX(KERNBASE+0x400000)] = (0x400000) | PDE_P | PDE_RW | PDE_4M,
    [PDX(KERNBASE+0x800000)] = (0x800000) | PDE_P | PDE_RW | PDE_4M,
};

#endif