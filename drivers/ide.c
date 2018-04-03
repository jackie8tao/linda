/*
 * ide硬盘的pio方式读写
 *
 * @author Jackie Tao <taodingfei@gmail.com>
 * @date 2018-04-02 11:08
 */

#include <drivers/ide.h>
#include <types.h>
#include <generic.h>
#include <trap.h>
#include <mm/kmalloc.h>
#include <string.h>

/*
 *       Reading the harddisk using ports!
 *       +-------------------------------+   by qark
 *
 *
 *  This took me months to get working but I finally managed it.
 *
 *  This code only works for the 286+ so you must detect for 8088's somewhere
 *  in your code.
 *
 *  Technical Information on the ports:
 *      Port    Read/Write   Misc
 *     ------  ------------ -------------------------------------------------
 *       1f0       r/w       data register, the bytes are written/read here
 *       1f1       r         error register  (look these values up yourself)
 *       1f2       r/w       sector count, how many sectors to read/write
 *       1f3       r/w       sector number, the actual sector wanted
 *       1f4       r/w       cylinder low, cylinders is 0-1024
 *       1f5       r/w       cylinder high, this makes up the rest of the 1024
 *       1f6       r/w       drive/head
 *                              bit 7 = 1
 *                              bit 6 = 0
 *                              bit 5 = 1
 *                              bit 4 = 0  drive 0 select
 *                                    = 1  drive 1 select
 *                              bit 3-0    head select bits
 *       1f7       r         status register
 *                              bit 7 = 1  controller is executing a command
 *                              bit 6 = 1  drive is ready
 *                              bit 5 = 1  write fault
 *                              bit 4 = 1  seek complete
 *                              bit 3 = 1  sector buffer requires servicing
 *                              bit 2 = 1  disk data read corrected
 *                              bit 1 = 1  index - set to 1 each revolution
 *                              bit 0 = 1  previous command ended in an error
 *       1f7       w         command register
 *                            commands:
 *                              50h format track
 *                              20h read sectors with retry
 *                              21h read sectors without retry
 *                              22h read long with retry
 *                              23h read long without retry
 *                              30h write sectors with retry
 *                              31h write sectors without retry
 *                              32h write long with retry
 *                              33h write long without retry
 *
 *  Most of these should work on even non-IDE hard disks.
 *  This code is for reading, the code for writing is the next article.
 */

#define SECTORSZ        512             // 扇区大小
#define BLOCKSZ         512             // 块大小

// ide命令
#define IDE_CMD_READ    0x20
#define IDE_CMD_WRITE   0x30
#define IDE_CMD_RDMUL   0xC4
#define IDE_CMD_WRMUL   0xC5

// ide状态
#define IDE_ERR         1 << 0
#define IDE_DRY         1 << 3
#define IDE_SRV         1 << 4
#define IDE_DF          1 << 5
#define IDE_RDY         1 << 6
#define IDE_BSY         1 << 7

void *buf;

static int
ide_wait()
{
    int status;
    while(((status = inb(0x01F7)) & (IDE_BSY | IDE_RDY)) != IDE_RDY);
    if(status & IDE_DF){
        return -1;
    }
    return 0;
}

void
ide_handler(struct trapframe *frame)
{
    buf = (void*)kmalloc(BLOCKSZ);
    kprintf("0x%X\n", buf);
    insl(0x1F0, buf, BLOCKSZ/4);
    kprintf("%s\n", (char*)buf);
}

void
ide_init()
{
    register_trap_handler(IRQ14, ide_handler);
}

int
ide_read(uint_t lba)
{
    ide_wait();

    // 打开中断
    outb(0x3F6, 0x0);

    outb(0x1F2, BLOCKSZ/SECTORSZ);
    outb(0x1F3, lba & 0xFF);
    outb(0x1F4, (lba >> 8) & 0xFF);
    outb(0x1F5, (lba >> 16) & 0xFF);
    outb(0x1F6, 0xE0 | ((lba >> 24) & 0x0F));
    outb(0x1F7, 0x20);

    return 1;
}

int
ide_write(uint_t lba, char *cont)
{

    ide_wait();
    outb(0x3F6, 0x0);

    outb(0x1F2, BLOCKSZ/SECTORSZ);
    outb(0x1F3, lba & 0xFF);
    outb(0x1F4, (lba >> 8) & 0xFF);
    outb(0x1F5, (lba >> 16) & 0xFF);
    outb(0x1F6, 0xE0 | ((lba >> 24) & 0x0F) | (0 << 4));
    outb(0x1F7, 0x30);
    outsl(0x1F0, (void*)cont, BLOCKSZ/4);
}
