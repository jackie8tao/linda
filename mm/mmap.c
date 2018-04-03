#include <types.h>
#include <mm/memory.h>
#include <mm/mmap.h>
#include <string.h>
#include <memlayout.h>
#include <kprintf.h>
#include <trap.h>

__attribute__((aligned(PDSIZE))) pde_t kern_pgdir[NPDENTRIES];
__attribute__((aligned(PTSIZE))) pte_t kern_pgtbl[NPDENTRIES][NPTENTRIES];

static void
page_fault_handler(struct trapframe *frame)
{
    uint_t addr;
    asm volatile("movl %%cr2, %0":"=r"(addr):);
    mmap_do_map(kern_pgdir, addr, addr >= KERNBASE ? MEM_KERN : MEM_USER);

    return;
}

void
mmap_init()
{
    uint_t kern_bound = (uint_t)virt_kern_addr() - KERNBASE;
    kprintf("kernel bound:0x%x\n", kern_bound);
    uint_t phy_addr=0;
    while(phy_addr<=kern_bound){
        uint_t virt_addr = P2V(phy_addr);
        pte_t *page_table = (pte_t*)(kern_pgdir[PDX(virt_addr)] & PAGE_MASK);
        if(!page_table){
            page_table = (pte_t*)(kern_pgtbl[PDX(virt_addr)]);
            if(!page_table){
                kprintf("physical memory allocator: there is no page!\n");
                return;
            }
            kern_pgdir[PDX(virt_addr)] = ((uint_t)(V2P(page_table)) & PAGE_MASK) | PDE_P | PDE_RW;
        }
        page_table[PTX(virt_addr)] = (phy_addr & PAGE_MASK) | PTE_P | PTE_RW;
        phy_addr += PAGE_SIZE;
    }

    asm volatile("movl %0, %%cr3"::"r"(V2P(kern_pgdir)));

    // 注册缺页处理函数
    register_trap_handler(CPU_PF, page_fault_handler);

    kprintf("kernel initial page dir created finished!\n");
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

// 映射虚拟内存至物理内存，单位4096byte
void
mmap_do_map(pde_t *mmap, uint_t va, uint_t flags)
{
    uint_t pa, pdx = PDX(va), ptx = PTX(va);

    pa = phy_mem_alloc();
    if(!pa){
        kprintf("physical memory allocator has no page!\n");
        return;
    }

    pte_t *page_table = (pte_t*)(mmap[pdx] & PAGE_MASK);
    if(flags & MEM_KERN){
        // 内核空间
        if(!page_table){
            page_table = (pte_t*)(kern_pgtbl[pdx]);
            mmap[pdx] = ((uint_t)(V2P(page_table)) & PAGE_MASK) | PDE_P | PDE_RW;
        }else{
            page_table = (pte_t*)(P2V(page_table));
        }
        page_table[ptx] = (pa & PAGE_MASK) | PTE_P | PTE_RW;
    }else if(flags & MEM_USER){
        // 用户空间
    }
    asm volatile("invlpg (%0)" : : "a" (va));
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
