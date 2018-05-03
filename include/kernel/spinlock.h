/*
 * 简单的自旋锁，存在忙等待的情况
 */

#ifndef __LINDA_SPINLOCK_H
#define __LINDA_SPINLOCK_H

#include <types.h>

struct spinlock{
    uint_t locked;
    char *name;
};

void initlock(struct spinlock*, char*);

void acquire(struct spinlock*);

void release(struct spinlock*);

int holding(struct spinlock*);

void pushcli();

void popcli();

#endif
