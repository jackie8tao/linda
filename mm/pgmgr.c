#include <types.h>
#include <memlayout.h>
#include <mm/pgmgr.h>
#include <string.h>
#include <kprintf.h>

#ifndef NULL
#define NULL ((void*)0)
#endif

extern void* KVADDR_END;

static PgFrameList_t pg_frame_list;

void 
pg_boot()
{
    pg_frame_list = NULL;
}

void 
pg_init(void *start, void *end)
{
    PgFrame_t *page;
    uint_t mem_start, mem_end;

    mem_start = PG_ROUNDUP((uint_t)start);
    mem_end = (uint_t)end;

    if (V2P(mem_end) > MEMSIZE || mem_start < (uint_t)KVADDR_END){
        panic("Physical Memory Allocator Occurs an Error!");
    }

    for(; mem_end > mem_start; mem_end -= PG_SIZE){
        pg_free((void*)mem_end);
    }

}

void* 
pg_alloc()
{
    PgFrame_t *page;
    
    page = pg_frame_list;
    pg_frame_list = page->next;

    return (void*)page;
}

void 
pg_free(void *addr)
{
    // 释放时应该要保证物理页的连续性
    uint_t mem = PG_ROUNDDOWN((uint_t)addr);

    memset((void*)mem, 0, PG_SIZE);

    PgFrame_t *page = (PgFrame_t*)mem;
    page->next = pg_frame_list;
    pg_frame_list = page;
}