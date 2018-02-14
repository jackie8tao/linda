#include <memlayout.h>
#include <mm/vm.h>
#include <mm/kalloc.h>
#include <string.h>
#include <stddef.h>
#include <types.h>
#include <mm/pgmrg.h>
#include <generic.h>

// 1、完善CPU的所有分页机制
// 2、根据硬件情况选择最优方案
// 3、手动配置分页方案

#define NEMLEM(arr)      (sizeof(arr)/sizeof((arr)[0]))

static MemMap_t kmap[] = {
    {}
};

// 这里对虚拟地址做了4k对齐
static Pte_t* 
vm_create_ptbl(MemMap_t *map)
{
    Pte_t *ptbl = kalloc(PTSIZE);
    if(!ptbl){
        panic("Page Table Creates Failed!");
    }

    uint_t va, phystart, physend;

    va = (uint_t)map->vaddr;
    phystart = PG_ROUNDUP(map->phys_start);
    physend = PG_ROUNDDOWN(map->phys_end);

    for(; phystart <= physend; phystart += PG_SIZE, va += PG_SIZE){
        ptbl[PTX(va)] = (phystart & ~0xFFF) | (map->pte_flag & 0xFFF);
    }

    return ptbl;
}

static Pde_t* 
vm_create_pdir()
{
    Pde_t *pdir = kalloc(PDSIZE);
    if(!pdir){
        panic("Page Directory Creates Failed!");
    }

    return pdir;
}

void 
kvm_init()
{
    Pde_t *kpdir = vm_create_pdir();

    Pte_t *ptbl;
    for(int i=NEMELM(kmap); i>0; --i){
        ptbl = vm_create_ptbl(&kmap[i]);
        kpdir[PDX(kmap[i].vaddr)] = ((uint_t)ptbl & ~0xFFF) | kmap[i].pde_flag;
    }
    
    pdtbl_load((void*)kpdir, 0);

    pg_enable();
}