#include <mm/vm.h>
#include <memlayout.h>
#include <mm/mmap.h>
#include <mm/memory.h>

static struct vm_block kvm_pgdir[KERNSIZE/PAGE_SIZE];

void
kvm_init()
{
    for(int i=0; i<KERNSIZE/PAGE_SIZE; ++i){
        kvm_free_page((void*)(KERNBASE+i*PAGE_SIZE));
    }

    kprintf("kernel virtual memory space initialized finished!\n");
}

void*
kvm_alloc_page()
{
    for(int i=0; i<KERNSIZE/PAGE_SIZE; ++i){
        if(kvm_pgdir[i].status == VM_FREE){
            return (void*)(KERNBASE + i*PAGE_SIZE);
        }
    }
    return NULL;
}

void
kvm_free_page(void *addr)
{
    if((uint_t)addr < KERNBASE){
        kprintf("illegal kern space address!\n");
        return;
    }

    uint_t kern_addr = (uint_t)virt_kern_addr();

    kvm_pgdir[((uint_t)addr-KERNBASE)/PAGE_SIZE].status =
        (uint_t)addr > kern_addr ? VM_FREE : VM_USED;
}

void
uvm_init()
{

}

void*
uvm_alloc_page()
{

}

void
uvm_free_page(void *addr)
{

}
