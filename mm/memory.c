#include <types.h>
#include <multiboot.h>
#include <mm/memory.h>
#include <kprintf.h>
#include <memlayout.h>
#include <mach.h>

extern char kern_start[];
extern char kern_end[];

extern struct multiboot_info *glb_mboot_ptr;

static struct {
    uint_t *free_list;
    uint_t page_count;
    int header;
} mem_map;

typedef struct multiboot_mmap_entry mmap_entry_t;

// 输出BIOS提供的物理内存布局
void
show_memory_map()
{
    if(glb_mboot_ptr->flags & MULTIBOOT_INFO_MEM_MAP){
        struct multiboot_mmap_entry* mmap;
        kprintf("mmap_addr:%#x, mmap_length:%#x\n", glb_mboot_ptr->mmap_addr, glb_mboot_ptr->mmap_length);
        for(
            mmap=(mmap_entry_t*)glb_mboot_ptr->mmap_addr;
            (uint_t)mmap < glb_mboot_ptr->mmap_addr + glb_mboot_ptr->mmap_length;
            mmap++
        ){
            kprintf("base_addr = 0x%X%08X, length = 0x%X%08X, type = 0x%X\n",
            (uint_t)mmap->addr_high, (uint_t)mmap->addr_low,
            (uint_t)mmap->len_high, (uint_t)mmap->len_low,
            (uint_t)mmap->type);
        }
    }else{
        panic("show_memory_map: it`s not a valid multiboot mmap struct!\n");
    }
}

// 初始化物理内存管理系统
void
phy_mem_init()
{
    uint_t start_addr, end_addr;

    mmap_entry_t *mmap_start_addr = (mmap_entry_t *)glb_mboot_ptr->mmap_addr;
    mmap_entry_t *mmap_end_addr = (mmap_entry_t *)glb_mboot_ptr->mmap_addr + glb_mboot_ptr->mmap_length;

    mmap_entry_t *map_entry;

    for (map_entry = mmap_start_addr; map_entry < mmap_end_addr; map_entry++) {
        if (map_entry->type == MULTIBOOT_MEMORY_AVAILABLE && map_entry->addr_low == EXTMEM) {
            start_addr = map_entry->addr_low + (uint_t)(kern_end - kern_start);
            end_addr = map_entry->addr_low + map_entry->len_low;
        }
    }

    // 分页偏移，实现对齐
    start_addr = PGROUNDUP(start_addr), end_addr = PGROUNDDOWN(end_addr);

    mem_map.free_list = (uint_t*)((uint_t)kern_end);
    while(end_addr>=start_addr){
        phy_mem_free((void*)end_addr);
        end_addr -= PGSIZE;
        mem_map.page_count++;
    }

    kprintf("physical memory initialize finished, page count:%d\n", mem_map.page_count);
}

// 申请一页物理内存
void*
phy_mem_alloc()
{
    if(mem_map.header<0){
        return NULL;
    }

    return (void*)mem_map.free_list[mem_map.header--];
}

// 释放物理内存
void
phy_mem_free(void *addr)
{
    mem_map.free_list[++mem_map.header] = PGROUNDDOWN((uint_t)addr);
}

// 获取内核数据边界，这里返回的是虚拟地址
void*
virt_kern_addr()
{
    return (void*)((uint_t)kern_end + mem_map.page_count*sizeof(uint_t));
}
