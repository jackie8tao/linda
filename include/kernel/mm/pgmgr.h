#ifndef __LINDA_PGMGR_H
#define __LINDA_PGMGR_H

#include <types.h>

#define PG_SIZE             0x1000          // 页面4K大小

#define PG_ROUNDUP(addr)    (((addr)+PG_SIZE-1) & ~(PG_SIZE-1))
#define PG_ROUNDDOWN(addr)  ((addr) & ~(PG_SIZE-1))

struct page_frame {
    struct page_frame *next;
};

struct page_frame_list {
    struct page_frame *framelist;
};

void pg_boot();

void pg_init(void *start, void *end);

void* pg_alloc(uint_t num);

void pg_free(void *addr);

#endif