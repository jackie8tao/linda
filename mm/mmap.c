#include <types.h>
#include <mm/memory.h>
#include <mm/mmap.h>
#include <string.h>
#include <memlayout.h>
#include <kprintf.h>

__attribute__((aligned(PDSIZE))) pde_t kern_pgdir[NPDENTRIES];

void 
mmap_init()
{
    uint_t phy_addr=0;
    while(phy_addr<=MEM_NORMAL){
        uint_t virt_addr = P2V(phy_addr);
        pte_t *page_table = (pte_t*)(kern_pgdir[PDX(virt_addr)] & PAGE_MASK);
        if(!page_table){
            page_table = (pte_t*)phy_mem_alloc();
            if(!page_table){
                kprintf("physical memory allocator: there is no page!\n");
                return;
            }
            kern_pgdir[PDX(virt_addr)] = ((uint_t)page_table & PAGE_MASK) | PDE_P | PDE_RW;
        }
        page_table = (pte_t*)P2V(page_table);
        page_table[PTX(virt_addr)] = (phy_addr & PAGE_MASK) | PTE_P | PTE_RW;
        phy_addr += PAGE_SIZE;
    }
    asm volatile("movl %0, %%cr3"::"r"(V2P(kern_pgdir)));
    kprintf("kernel virtual memory initialize finished!\n");
}

void* 
mmap_alloc_page()
{
    void *addr = phy_mem_alloc();
    if(!addr){
        kprintf("mmap: cannot alloc a page!\n");
        return NULL;
    }

    if((uint_t)addr >= MEM_NORMAL){
        kprintf("mmap: cannot use a page beyond max kernel memory space!, 0x%X\n", addr);
        return NULL;
    }

    return (void*)P2V(addr);
}

void 
mmap_free_page(void *addr)
{
    phy_mem_free((void*)V2P(addr));
}

void 
mmap_do_map(pde_t *mmap, uint_t va, uint_t pa, uint_t flags)
{
    uint_t pdx = PDX(va), ptx = PTX(va);
    pte_t *page_table = (pte_t*)(mmap[pdx] & PAGE_MASK);
    if(!page_table){
        page_table = (pte_t*)phy_mem_alloc();
        mmap[pdx] = (uint_t)page_table | PDE_P | PDE_RW;
        page_table = (pte_t*)((uint_t)page_table + PAGE_OFFSET);
        memset((void*)page_table, 0, PTSIZE);
    }else{
        page_table = (pte_t*)((uint_t)page_table + PAGE_OFFSET);
    }
    page_table[ptx] = (pa & PAGE_MASK) | flags;
    asm volatile ("invlpg (%0)" : : "a" (va));
}

void 
mmap_do_unmap(pde_t *mmap, uint_t va)
{
    uint_t pdx = PDX(va), ptx = PTX(va);
    pte_t *page_table = (pte_t*)(mmap[pdx] & PAGE_MASK);
    if(!page_table){
        return;
    }
    page_table = (pte_t*)((uint_t)page_table + PAGE_OFFSET);
    page_table[ptx] = 0;
    asm volatile ("invlpg (%0)" : : "a" (va));
}
