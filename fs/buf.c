#include <types.h>
#include <fs/buf.h>
#include <spinlock.h>
#include <kprintf.h>
#include <drivers/ide.h>
#include <mm/kmalloc.h>

// 缓冲任务队列
struct buf_queue bufs;

void
buf_init()
{
    bufs.count = 0;
    bufs.queue = NULL;
    kprintf("ide buffer queue initialized finished!\n");
}

struct buf*
buf_read(uint_t dev, uint_t blockno)
{
    struct buf *buf = kmalloc(sizeof(struct buf));
    if(!buf){
        kprintf("cannot alloc buf memory!\n");
        return NULL;
    }

    struct spinlock *lock = kmalloc(sizeof(struct spinlock));
    if(!lock){
        kprintf("cannot get a spinlock for buf[%d]\n", blockno);
        return NULL;
    }
    initlock(lock, "ide read lock");

    buf->blockno = blockno;
    buf->dev = dev;
    buf->flags = B_VALID;
    buf->lock = lock;

    struct buf *header;
    if(!bufs.queue){
        bufs.queue = buf;
        bufs.count++;
        goto do_read_ide;
    }else{
        header = bufs.queue;

        while(header->next){
            header = header->next;
        }

        header->next = buf;
        buf->prev = header;
        bufs.count++;
    }

do_read_ide:
    ide_execute();

    return buf;
}

void
buf_write(struct buf *buf)
{
    // 将buf添加到缓冲队列
    struct buf *header;
    if(!bufs.queue){
        bufs.queue = buf;
        bufs.count++;
        goto do_write_ide;
    }else{
        header = bufs.queue;
        while(header->next){
            header = header->next;
        }
        header->next = buf;
        buf->prev = header;
        bufs.count++;
    }
    // 尝试写缓冲至磁盘
do_write_ide:
//asm volatile("xchg %bx, %bx");
    ide_execute();
}

void
buf_remove(struct buf *buf)
{
    struct buf *header = bufs.queue;
    while(header){
        if(!header){
            kprintf("There has no buf:[%d]\n", buf->blockno);
            return;
        }

        if(header == buf){
            if(!header->prev){
                if(!header->next){
                    // 唯一一个元素
                    bufs.queue = NULL;
                }else{
                    // 第一个元素
                    bufs.queue = header->next;
                }
            }else{
                if(!header->next){
                    // 最后一个元素
                    header->prev->next = NULL;
                }else{
                    // 中间元素
                    header->prev->next = header->next;
                    header->next->prev = header->prev;
                }
            }

            bufs.count--;
            return;
        }

        header = header->next;
    }
}

struct buf*
buf_get()
{
    struct buf *buf = bufs.queue;
    if(buf){
        return buf;
    }

    return NULL;
}
