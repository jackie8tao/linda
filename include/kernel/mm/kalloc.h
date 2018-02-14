#ifndef __LINDA_KALLOC_H
#define __LINDA_KALLOC_H

#include <types.h>

#define KMEM_USED       0
#define KMEM_FREE       1

struct kmem_block {
    uchar_t status;
    void* addr;
    uint_t size;
    struct kmem_block *next;
};

typedef struct {
    struct kmem_block *blocks;
} KmemBlockList_t;

void kheap_init();

void* kalloc(uint_t size);

void kfree(void* addr);

#endif