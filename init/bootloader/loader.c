#include <elf.h>
#include <types.h>
#include <generic.h>

#define SECTOR_SIZE     512

static void 
wait_disk()
{
    // 磁盘准备完成并且没有执行命令
    while((inb(0x01F7) & 0xC0) != 0x40);
}

static void 
read_sectors(void *dst, uint_t offset, uchar_t count)
{
    // 0x01F0 ~ 0x01F7是IDE主磁盘I/O端口
    wait_disk();
    outb(0x01F2, count);
    outb(0x01F3, offset & 0xFF);
    outb(0x01F4, (offset >> 8) & 0xFF);
    outb(0x01F5, (offset >> 16) & 0xFF);
    outb(0x01F6, (offset >> 24) | 0xE0);
    outb(0x01F7, 0x20);

    wait_disk();
    insl(0x01F0, dst, SECTOR_SIZE/4);
}

static void 
read_segment(void *dst, uint_t length, uint_t offset)
{
    char *start_addr, *end_addr;
    
    start_addr = (char*)dst;
    end_addr = start_addr + length;

    start_addr -= offset % SECTOR_SIZE;

    uint_t sectpos = offset/SECTOR_SIZE + 1;
    for(; start_addr < end_addr; start_addr += SECTOR_SIZE, sectpos++){
        read_sectors(start_addr, sectpos, 1);
    }
}

void 
loader_start()
{
    struct elf_hdr *elf = (struct elf_hdr*)0x10000;

    read_segment(elf, 4096, 0);
    // TODO 从扇区读取内核代码
}