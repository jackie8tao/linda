/*
 * 文件系统读写的缓冲区
 *
 * @author Jackie Tao <taodingfei@gmail.com>
 */

#ifndef __LINDA_BUF_H
#define __LINDA_BUF_H

#include <types.h>
#include <fs/fs.h>

#define B_VALID     1 << 0
#define B_DIRTY     1 << 1

struct buf{
    int flags;
    uint_t dev;
    struct buf *prev;
    struct buf *next;
    char data[BLOCK_SIZE];
};

void buf_init();
struct buf* buf_read(uint_t, uint_t);
void buf_write(struct buf*);
void buf_release(struct buf*);

#endif
