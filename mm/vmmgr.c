#include <mm/vmmgr.h>
#include <memlayout.h>
#include <mm/mmap.h>
#include <mm/memory.h>
#include <mm/kmalloc.h>
#include <kprintf.h>
#include <mach.h>
#include <string.h>

static struct vm_block kvmlist[NVMBLOCKS];

// 初始化内核虚拟空间内存页状态
void
kvm_init()
{
    uint_t addr, kernaddr;

    kernaddr = (uint_t)virt_kern_addr();
    for(int i=0; i<NVMBLOCKS; ++i){
        addr = P2V(i*PGSIZE);
        kvmlist[i].address = KERNBASE + i*PGSIZE;
        if(addr<=kernaddr){
            kvmlist[i].status |= (VM_USED | VM_MAPPED);
        }
    }

    kprintf("kernel virtual memory space initialized finished!\n");
}

// 申请内核虚拟空间内存页
void*
kvm_alloc_page()
{
    char status;
    for(int i=0; i<NVMBLOCKS; ++i){
        status = kvmlist[i].status;
        if((status & VM_MAPPED) && !(status & VM_USED)){
            kvmlist[i].status |= VM_USED;
            return (void*)kvmlist[i].address;
        }
    }

    // @TODO 内核空间不存在虚拟页情况的处理
    panic("no free mapped page!\n");
}

// 从内核虚拟空间获取未映射的页
void*
kvm_alloc_umapped_page()
{
    char status;
    for(int i=0; i<NVMBLOCKS; ++i){
        status = kvmlist[i].status;
        if(status & VM_MAPPED){
            continue;
        }

        return kvmlist[i].address;
    }

    panic("kvm_alloc_umapped_page: no page!\n");
}

// 释放内核虚拟空间内存页
void
kvm_free_page(void *addr)
{
    uint_t offset = (uint_t)addr - KERNBASE;
    if(offset<0 || offset>=KERNSIZE){
        panic("kvm_free_page: 0x%x is not in kernel!\n", (uint_t)addr);
    }

    int idx = offset/PGSIZE;
    if(kvmlist[idx].address == 0){
        kvmlist[idx].address = P2V(idx*PGSIZE);
    }
    kvmlist[idx].status &= (~VM_USED);
}

// 从内核虚拟空间中解映射出空闲页
void*
kvm_unmap_page(void *addr)
{
    uint_t offset = (uint_t)addr - KERNBASE;
    if(offset>=KERNSIZE){
        panic("kvm_map_page: 0x%x is not in kernel!\n", (uint_t)addr);
    }

    kvmlist[offset/PGSIZE].status &= (~VM_MAPPED);
    // @TODO 页面换出
}

// 申明内核空间某一页已经被映射
void*
kvm_map_page(void *addr)
{
    uint_t offset = (uint_t)addr - KERNBASE;
    if(offset>=KERNSIZE){
        panic("kvm_map_page: 0x%x is not in kernel!\n", (uint_t)addr);
    }
    kvmlist[offset/PGSIZE].status |= VM_MAPPED;
}
