#include <types.h>
#include <stddef.h>
#include <kprintf.h>
#include <string.h>
#include <mm/kalloc.h>
#include <mm/pgmgr.h>

// TODO:
//  1、添加原子操作
//  2、算法调整优化
//  3、释放时的块合并功能

// 内核堆初始块的申请物理页
#define KHEAP_INIT_PAGES        2
#define BLOCK_FREEADDR(addr)    ((char*)(addr) + sizeof(struct kmem_block))

static KmemBlockList_t kheap;

static void 
block_insert(struct kmem_block *pos, struct kmem_block *blk)
{
    blk->next = pos->next;
    pos->next = blk;
}

static void 
block_update(struct kmem_block *blk, uchar_t status, void *addr, uint_t size)
{
    blk->status = status;
    blk->addr = addr;
    blk->size = size;
}

// 此处返回分割剩余之后的块
static struct kmem_block* 
block_split(struct kmem_block *blk, uint_t size)
{
    if((blk->size - size) <= sizeof(struct kmem_block)){
        return NULL;
    }

    struct kmem_block *result = (struct kmem_block*)((char*)blk->addr+size);

    uint_t r_addr, r_size;
    r_addr = (uint_t)BLOCK_FREEADDR(result);
    r_size = blk->size - size - sizeof(struct kmem_block);

    block_update(result, KMEM_FREE, (void*)r_addr, r_size);
    
    block_update(blk, blk->status, blk->addr, size);

    return result;
}

static struct kmem_block* 
block_create_from_page(unsigned int num)
{
    // 需要原子操作保证申请物理页的过程不能中断
    struct kmem_block *blk = (struct kmem_block*)pg_alloc(1);
    if(!blk){
        panic("Kernel Memory Allocator Failed!");
    }

    int i = (int)(num - 1);
    for(; i>0; --i){
        if(!pg_alloc(1)){
            panic("Kernel Memory Allocator Failed!");
        }
    }

    uint_t addr, size;
    
    addr = (uint_t)BLOCK_FREEADDR(blk);
    size = num*PG_SIZE - sizeof(struct kmem_block);

    block_update(blk, KMEM_FREE, (void*)addr, size);

    return blk;
}

void 
kheap_init()
{
    kheap.blocks = block_create_from_page(KHEAP_INIT_PAGES);
}

void* 
kalloc(uint_t size)
{
    struct kmem_block *blk, *lastblk;
    
alloc:
    blk = kheap.blocks;
    do{
        if(blk->status != KMEM_FREE)
            goto notfound;
        if(blk->size < size)
            goto notfound;

        struct kmem_block *result = block_split(blk, size);
        if(!result)
            goto notfound;

        // 插入分割剩余的块
        block_insert(blk, result);
        
        block_update(blk, KMEM_USED, blk->addr, blk->size);

        return (void*)result;

        notfound:
            lastblk = blk;
            blk = blk->next;
    }while(!blk);

    struct kmem_block *freeblock;

    freeblock = block_create_from_page((size + sizeof(struct kmem_block)) / PG_SIZE + 1);
    
    block_insert(lastblk, freeblock);

    goto alloc;
}

void 
kfree(void *addr)
{
    struct kmem_block *blk;

    blk = kheap.blocks;
    do{
        if(
            blk->status == KMEM_USED && 
            (uint_t)addr >= (uint_t)blk->addr && 
            (uint_t)addr < (uint_t)((char*)blk->addr + blk->size)
        ){
            blk->status = KMEM_FREE;
            memset(blk->addr, blk->size, 0);
        }
    }while(!blk);
}