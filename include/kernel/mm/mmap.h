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
#include <memlayout.h>

#define NPDENTRIES          1024                        // 页目录的项数量
#define NPTENTRIES          1024                        // 页表的项数量
#define PTSIZE              4096                        // 页表内存大小
#define PDSIZE              4096                        // 页目录内存大小
#define PAGE_MASK           0xFFFFF000                  // 页掩码，用于4KB对齐
#define PAGE_OFFSET         KERNBASE                    // 内核的偏移地址
#define PAGE_SIZE           4096                        // 页框大小

#define PGSHIFT             12                          // 页偏移量
#define PTXSHIFT            12                          // 页表偏移量
#define PDXSHIFT            22                          // 页目录偏移量

// 页目录项的标志位
#define PDE_P               1 << 0                      // 如果P=1，表示页表地址指向的该页在内存中，如果P=0，表示不在内存中
#define PDE_RW              1 << 1                      // 页目录项“读写”标志位
#define PDE_US              1 << 2                      // 页目录项“用户权限”标志位，任何权限都可读写
#define PDE_PWT             1 << 3                      // 表示是否采用写透方式，写透方式就是既写内存（RAM）也写高速缓存,该位为1表示采用写透方式
#define PDE_PCD             1 << 4                      // 表示是否启用高速缓存,该位为1表示启用高速缓存。
#define PDE_A               1 << 5                      // 访问位，当对页目录项进行访问时，该位为1
#define PDE_4M              1 << 7                      // 4M页面

// 页表项的标志位
#define PTE_P               1 << 0                      // 
#define PTE_RW              1 << 1                      // 
#define PTE_US              1 << 2                      //
#define PTE_PWT             1 << 3                      //
#define PTE_PCD             1 << 4                      //
#define PTE_A               1 << 5                      //
#define PTE_D               1 << 6                      //
#define PTE_PS              1 << 7                      //
#define PTE_G               1 << 8                      // 

#define PTX(va) (((uint_t)(va) >> PTXSHIFT) & 0x3FF)    // 页表下标
#define PDX(va) (((uint_t)(va) >> PDXSHIFT) & 0x3FF)    // 页目录下标


#define MEM_DMA             0x1000000                   // 0～16M
#define MEM_NORMAL          0x38000000                  // 16～896M
#define MEM_HIGH            0x40000000                  // 896～1G

// 页目录和页表的项数据类型
typedef uint_t pde_t;                                   // 页目录项类型
typedef uint_t pte_t;                                   // 页表项类型

// 内核页目录初始化
void mmap_init();

// pa=>va
void mmap_do_map(pde_t *mmap, uint_t va, uint_t pa, uint_t flags);

// pa !=> va
void mmap_do_unmap(pde_t *mmap, uint_t va);

// 获取虚拟内存空间的空闲页
void* mmap_alloc_page();

// 释放虚拟内存页
void mmap_free_page(void *addr);

// 申请虚拟内存区域
//void* mmap_alloc_area();

// 释放虚拟内存区域
//void mmap_free_area();

#endif