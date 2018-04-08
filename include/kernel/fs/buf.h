/*
 * 文件系统读写的缓冲区
 *
 * @author Jackie Tao <taodingfei@gmail.com>
 */

#ifndef __LINDA_BUF_H
#define __LINDA_BUF_H

#include <types.h>
#include <fs/fs.h>
#include <spinlock.h>

#define B_VALID     1 << 0
#define B_DIRTY     1 << 1

extern struct buf_queue bufs;

// 缓冲区数据结构
struct buf{
    int flags;
    uint_t dev;
    uint_t blockno;
    struct buf *prev;
    struct buf *next;
    struct spinlock *lock;
    char data[BLOCK_SIZE];
};

struct buf_queue{
    struct buf *queue;
    uint_t count;
};

void buf_init();
struct buf* buf_read(uint_t, uint_t);
void buf_write(struct buf*);
void buf_remove(struct buf*);
struct buf *buf_get();

#endif
