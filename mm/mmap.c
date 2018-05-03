#include <types.h>
#include <mm/memory.h>
#include <mm/mmap.h>
#include <string.h>
#include <memlayout.h>
#include <kprintf.h>
#include <trap.h>
#include <mm/kmalloc.h>
#include <process.h>
#include <generic.h>
#include <scheduler.h>
#include <mm/vmmgr.h>
#include <spinlock.h>

__attribute__((aligned(PDSIZE))) pde_t kpgdir[NPDENTRIES];
__attribute__((aligned(PTSIZE))) pte_t kpgtbl[NPDENTRIES][NPTENTRIES];

struct mmap_block *mmaplist;

// 缺页中断处理程序
static void
page_fault_handler(struct trapframe *frame)
{
    struct process *proc = current_cpu()->curprocess;
    uint_t addr = readcr2();
    kprintf("addr: 0x%x\n", addr);
    if(addr >= KERNBASE){
        // @TODO 内核若发生缺页则存在问题，需要处理
        mmap_do_map(kpgdir, addr, (uint_t)phy_mem_alloc(), 0);
    }else{
        mmap_do_map(proc->pgdir, addr, (uint_t)phy_mem_alloc(), PTE_U);
    }

    return;
}

// 初始化内核部分映射
void
kmap_init()
{
    int pdx, ptx;
    uint_t virt_addr, phy_addr = 0;
    pte_t *pgtbl;

    while(phy_addr<MEM_NORMAL){
        virt_addr = P2V(phy_addr);
        pdx = PDX(virt_addr), ptx = PTX(virt_addr);
        pgtbl = (pte_t*)(kpgdir[pdx] & PGMASK);
        if(pgtbl == 0){
            pgtbl = (pte_t*)(kpgtbl[pdx]);
            kpgdir[pdx] = ((uint_t)(V2P(pgtbl)) & PGMASK) | PTE_P | PTE_W;
        }else{
            pgtbl = (pte_t*)(P2V(pgtbl));
        }

        pgtbl[ptx] = (phy_addr & PGMASK) | PTE_P | PTE_W;

        kvm_map_page(P2V(phy_addr));
        kvm_free_page(P2V(phy_addr));

        phy_addr += PGSIZE;
    }

    phy_addr = 0;
    while(phy_addr<MEM_NORMAL){
        phy_addr = phy_mem_alloc(); // 弹出所有已经被内核映射的内存
    }
    if(phy_addr>=MEM_NORMAL){
        phy_mem_free((void*)phy_addr);
    }

    lcr3(V2P(kpgdir));
    register_trap_handler(CPU_PF, page_fault_handler);

    kprintf("kernel pgdir created finished!\n");
}

// 映射虚拟内存至物理内存，单位4096byte
void
mmap_do_map(pde_t *mmap, uint_t va, uint_t pa, uint_t flags)
{
    mmap_do_unmap(mmap, va);
    int pdx = PDX(va), ptx = PTX(va);
    pte_t *pgtbl = (pte_t*)(mmap[pdx] & PGMASK);
    if(pgtbl == 0){
        pgtbl = (pde_t*)kvm_alloc_page();
        memset(pgtbl, 0, PGSIZE);
        mmap[pdx] = ((uint_t)(V2P(pgtbl))) & PGMASK | PTE_P | PTE_W | flags;
    }else{
        pgtbl = (pde_t*)(P2V(pgtbl));
    }
    pgtbl[ptx] = (pa & PGMASK) | PTE_P | PTE_W | flags;
    asm volatile("invlpg (%0)": : "r"(va));

    struct mmap_block *map, *last;
    if(mmaplist == 0){
        map = kmalloc(sizeof(struct mmap_block));
        map->virt_addr = va & PGMASK;
        map->phy_addr = pa & PGMASK;
        map->mapped = 1;
        mmaplist = map;
    }else{
        map = mmaplist;
        while(map!=0){
            if(map->mapped){
                last = map;
                map = map->next;
                continue;
            }else{
                map->virt_addr = va;
                map->phy_addr = pa;
                map->mapped = 1;
                break;
            }
        }

        if(map==0){
            map = kmalloc(sizeof(struct mmap_block));
            map->virt_addr = va;
            map->phy_addr = pa;
            map->mapped = 1;
            last->next = map;
        }
    }
}

// 从页目录中释放已映射的虚拟内存
// 只适用于内核
void
mmap_do_unmap(pde_t *mmap, uint_t va)
{
    uint_t pdx = PDX(va), ptx = PTX(va);
    pte_t *pgtbl = (pte_t*)(mmap[pdx] & PGMASK);
    if(pgtbl == 0){
        return;
    }
    pgtbl = (pte_t*)(P2V(pgtbl));
    pgtbl[ptx] = 0;

    asm volatile ("invlpg (%0)" : : "a"(va));

    struct mmap_block *last=mmaplist, *map=mmaplist;
    int first = 0;
    while(map!=0){
        if((map->virt_addr & PGMASK)==(va & PGMASK)){
            if(first!=0){
                last->next = map->next;
            }else{
                mmaplist = NULL;
            }
            kmfree((void*)map);
            break;
        }
        first++;
        last = map;
        map = map->next;
    }
}

// 用户空间页目录初始化
pde_t*
umap_init()
{
    pde_t *pgdir = (pte_t*)kvm_alloc_page();
    for(int i = PDX(KERNBASE); i<NPDENTRIES; ++i){
        pgdir[i] = kpgdir[i];
    }

    return pgdir;
}

// 切换到内核页表
void
switch_to_kvm()
{
    asm volatile("movl %0, %%cr3"::"r"(V2P(kpgdir)));
}

// 切换到进程proc的空间
void
switch_to_uvm(struct process *proc)
{
    if(!proc){
        panic("switch_to_uvm : no process\n");
    }
    if(!proc->kstack){
        panic("switch_to_uvm : no stack\n");
    }
    if(!proc->pgdir){
        panic("switch_to_uvm : no pgdir\n");
    }

    pushcli();
    struct cpu *curcpu = current_cpu();

    curcpu->gdt[SEG_TSS] = SEG16(STS_T32A, &curcpu->tss, sizeof(struct taskstate)-1, 0);
    curcpu->gdt[SEG_TSS].s = 0;

    curcpu->tss.ss0 = SEG_KDATA << 3;
    curcpu->tss.esp0 = (uint_t)proc->kstack + KSTACKSIZE;
    curcpu->tss.iomb = (ushort_t)0xFFFF;

    ltr(SEG_TSS << 3);
    lcr3(V2P(proc->pgdir));
    popcli();
}

// cpu全局段描述表初始化
void
segment_init()
{
    struct cpu *curcpu = current_cpu();

    curcpu->gdt[SEG_KCODE] = SEG(STA_X|STA_R, 0, 0xFFFFFFFF, DPL_KERN);
    curcpu->gdt[SEG_KDATA] = SEG(STA_W, 0, 0xFFFFFFFF, DPL_KERN);
    curcpu->gdt[SEG_UCODE] = SEG(STA_X|STA_R, 0, 0xFFFFFFFF, DPL_USER);
    curcpu->gdt[SEG_UDATA] = SEG(STA_W, 0, 0xFFFFFFFF, DPL_USER);

    lgdt(curcpu->gdt, sizeof(curcpu->gdt));
}

// 将第一页的代码，载入到用户空间
// 用户空间页目录，已经在umap_init函数中完成
pde_t*
uvm_init(pde_t *pgdir, char *init, uint_t sz)
{
    uint_t phy_mem = (uint_t)phy_mem_alloc();
    uint_t virt_mem = (uint_t)kvm_alloc_umapped_page();
    if(sz>PGSIZE){ // 限制为一页
        panic("uvm_init: more than a page!\n");
    }
    if(phy_mem == 0){
        // @TODO 这里需要处理换页等问题
        panic("uvm_init: no physical page!\n");
    }

    mmap_do_map(kpgdir, virt_mem, phy_mem, 0); //映射到内核空间
    memmove(virt_mem, init, sz);
    mmap_do_unmap(kpgdir, virt_mem); // 从内核空间解映射
    mmap_do_map(pgdir, 0, phy_mem, PTE_U); // 映射到用户空间
}
