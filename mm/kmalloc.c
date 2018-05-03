#include <types.h>
#include <mm/kmalloc.h>
#include <mm/vmmgr.h>
#include <kprintf.h>
#include <string.h>
#include <mach.h>

#define KMEM_USED       0
#define KMEM_FREE       1

struct kmem_chunk {
    struct kmem_chunk *next;
    uchar_t status;
    void *addr;
    uint_t size;
};

static struct kmem_chunk *kmem_list = NULL;

// 从空闲块中切割出一小块
static void*
chunk_split(struct kmem_chunk *chunk, uint_t size)
{
    uint_t header_size = sizeof(struct kmem_chunk);
    if(chunk->size <= (size + header_size)){
        panic("chunk_split: kmem chunk does`t have enough size!\n");
    }

    if(chunk->status != KMEM_FREE){
        panic("chunk_split: cannot split used kmem chunk!\n");
    }

    struct kmem_chunk *split = (struct kmem_chunk*)((uint_t)chunk->addr + size);
    split->addr = (void*)((uint_t)split + header_size);
    split->status = KMEM_FREE;
    split->size = chunk->size - size - header_size;
    split->next = chunk->next;

    chunk->size = size;
    chunk->next = split;

    return (void*)chunk;
}

// 合并相邻的空闲块
static void
chunk_glue()
{
    if(!kmem_list){
        panic("chunk_glue: these is no memory chunk to glue!\n");
    }

    uint_t header_size = sizeof(struct kmem_chunk);
    struct kmem_chunk *chunk = kmem_list;
    while(chunk){
        struct kmem_chunk *next = chunk->next;
        if(!next){
            break;
        }

        if(chunk->status==KMEM_FREE && next->status==KMEM_FREE){
            uint_t distance = (uint_t)next->addr - (uint_t)chunk->addr;
            if(distance == (chunk->size + header_size)){
                chunk->size = chunk->size + next->size + header_size;
                chunk->next = next->next;
                memset((void*)chunk->addr, 0, chunk->size);
            }
        }

        chunk = next;
    }
}

static void*
alloc_free_chunk_from_kvm()
{
    uint_t header_size = sizeof(struct kmem_chunk);

    struct kmem_chunk *chunk = kvm_alloc_page();
    if(!chunk){
        panic("kmalloc: there is no enough free memory!\n");
    }

    chunk->size = PGSIZE - header_size;
    chunk->status = KMEM_FREE;
    chunk->addr = (void*)((uint_t)chunk + header_size);
    chunk->next = NULL;

    return (void*)chunk;
}

void*
kmalloc(uint_t size)
{
    if(!kmem_list){
        struct kmem_chunk *chunk = alloc_free_chunk_from_kvm();
        if(!chunk){
            panic("kmalloc: annot alloc any memory, sorry!\n");
        }

        kmem_list = chunk;
    }

    void *res = NULL;
    struct kmem_chunk *block = kmem_list;
alloc_routine:
    while(block){
        if(block->status != KMEM_FREE){
            block = block->next;
            continue;
        }

        if(block->size > size){
            struct kmem_chunk *chunk = (struct kmem_chunk*)chunk_split(block, size);
            if(!chunk){
                break;
            }

            chunk->status = KMEM_USED;
            res = chunk->addr;

            break;
        }

        if(block->size == size){
            res = block->addr;
            block->status = KMEM_USED;
            break;
        }

        if(!block->next){
            struct kmem_chunk *next = alloc_free_chunk_from_kvm();
            if(!next){
                kprintf("kmalloc: cannot alloc any memory, sorry!\n");
                res = NULL;
                break;
            }

            block->next = next;
            chunk_glue();// 在申请新的内存块之后，尝试进行合并
            goto alloc_routine; // 在内存合并之后，需要重新进行遍历查询
        }

        block = block->next;
    }
    return res;
}

void
kmfree(void *addr)
{
    if(!kmem_list){
        // 如果没有进行申请过内存，直接释放的话，不做任何事情
        return;
    }

    struct kmem_chunk *chunk = kmem_list;
    while(chunk){
        if(chunk->status != KMEM_USED){
            chunk = chunk->next;
            continue;
        }

        if(chunk->addr <= addr && ((uint_t)chunk->addr + chunk->size) > (uint_t)addr){
            chunk->status = KMEM_FREE;
            memset(chunk->addr, 0, chunk->size);
            break;
        }

        chunk = chunk->next;
    }

    chunk_glue();// 内存释放完成之后，进行一次合并
}

// 调试使用，打印出内存链表
void
print_chunks()
{
    if(!kmem_list){
        panic("print_chunks : no memory!\n");
    }

    struct kmem_chunk *chunk = kmem_list;
    while(chunk){
        kprintf("addr: 0x%X, size: %d, status: %d\n", chunk->addr, chunk->size, chunk->status);
        chunk = chunk->next;
    }

    return;
}
