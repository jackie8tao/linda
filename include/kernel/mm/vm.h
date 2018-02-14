#ifndef __LINDA_VM_H
#define __LINDA_VM_H

#include <types.h>

#define NPDENTRIES          1024
#define NPTENTRIES          1024
#define PTSIZE              4096
#define PDSIZE              4096

#define PGSHIFT             12
#define PTXSHIFT            12
#define PDXSHIFT            22

#define PTE_ADDR(pte)       ((uint_t)(pte) & ~(0xFFF))
#define PTE_FLAGS(pte)      ((uint_t)(pte) & 0xFFF)

#define PDX(va)             (((uint_t)(va) >> PDXSHIFT) & 0x3FF)
#define PTX(va)             (((uint_t)(va) >> PTXSHIFT) & 0x3FF)

// PDE和PTE的权限位
#define PDE_P               1 << 0
#define PDE_RW              1 << 1
#define PDE_US              1 << 2
#define PDE_PWT             1 << 3
#define PDE_PCD             1 << 4
#define PDE_A               1 << 5
#define PTE_P               1 << 0
#define PTE_RW              1 << 1
#define PTE_US              1 << 2
#define PTE_PWT             1 << 3
#define PTE_PCD             1 << 4
#define PTE_A               1 << 5
#define PTE_D               1 << 6
#define PTE_PAT             1 << 7
#define PTE_G               1 << 8

#define CR3_PWT             1 << 3
#define CR3_PCD             1 << 4

typedef uint_t Pde_t;
typedef uint_t Pte_t;

typedef struct {
    void *vaddr;
    uint_t phys_start;
    uint_t phys_end;
    int pde_flag;
    int pte_flag;
} MemMap_t;

void kvm_init();

void uvm_init();

#endif