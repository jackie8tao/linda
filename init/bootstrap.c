#include <drivers/console.h>
#include <kprintf.h>
#include <types.h>
#include <multiboot.h>
#include <memlayout.h>
#include <mm/bootmem.h>
#include <mm/memory.h>
#include <mm/mmap.h>
#include <mm/kmalloc.h>
#include <mm/vm.h>
#include <trap.h>
#include <generic.h>
#include <drivers/timer.h>
#include <drivers/ide.h>
#include <fs/buf.h>
#include <string.h>

extern uchar_t kern_start[];                // 内核文件在内存中的起始位置
extern uchar_t kern_end[];                  // 内核文件在内存中的结束位置

struct multiboot_info *glb_mboot_ptr;       // multiboot信息的全局指针

static void
show_welcome_msg()
{
    kprintf("Hello World! This Linda!\n");
    kprintf("Hope you have fun by using Linda!\n");
}

static int
init_glb_mboot(uint_t magic_num, uint_t mboot_info)
{
    if (magic_num != MULTIBOOT_BOOTLOADER_MAGIC) {
        kprintf("It`s not a valid multiboot! Magic Number= %#x\n", magic_num);
        return 0;
    }

    glb_mboot_ptr = (struct multiboot_info*)mboot_info;
    return 1;
}

static void
show_kernel_info()
{
    kprintf("kernel_start_addr:0x%X, kernel_end_addr:0x%X\n", kern_start, kern_end);
}

static void
timer_handler(struct trapframe *frame)
{

}

void
kern_bootstrap(uint_t magic_num, uint_t mboot_info)
{
    init_glb_mboot(magic_num, mboot_info);

    console_init();

    show_welcome_msg();
    show_kernel_info();
    show_memory_map();

    trap_init();

    phy_mem_init();
    mmap_init();
    kvm_init();

    timer_setup(timer_handler);

    ide_init();
    buf_init();

    sti();

    // ide写测试代码
    // struct buf *buf = kmalloc(sizeof(struct buf));
    // buf->dev = 0;
    // buf->blockno = 1;
    // char *cnt = "Hello world! Jackie!";
    // for(int i=0; i<strlen(cnt); ++i){
    //     buf->data[i] = cnt[i];
    // }
    // buf->data[strlen(cnt)] = '0';
    // struct spinlock *lock = kmalloc(sizeof(struct spinlock));
    // initlock(lock, "buf lock");
    // buf->lock = lock;
    // buf->flags = B_DIRTY;
    // buf_write(buf);
    // kprintf("0x%x\n", kmalloc(111));

    // ide读测试代码
    // struct buf *buf = buf_read(0, 0);
    // while(1){
    //     if(buf->flags & B_DIRTY){
    //         kprintf("%s\n", buf->data);
    //         break;
    //     }
    // }
}
