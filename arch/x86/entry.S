# 内核入口，兼容multiboot1

.code32
.set KERNBASE,      0xC0000000      # 内核虚拟空间起始地址
.set KERNLINK,      0xC0100000      # 内核虚拟空间载入地址

# --------------------------------------------------------
# multiboot头信息声明
.set HEADER_MAGIC,  0x1BADB002      # multiboot头魔数
.set PAGE_ALIGN,    1 << 0          # i386下的4k对齐
.set MEMORY_INFO,   1 << 1          # 显示内存信息
.set VIDEO_MODE,    1 << 2          # 显示显卡模式信息

.set HEADER_FLAGS, PAGE_ALIGN | MEMORY_INFO
.set HEADER_CHECKSUM, -(HEADER_MAGIC + HEADER_FLAGS)

.section multiboot_header
.align 4
.long HEADER_MAGIC
.long HEADER_FLAGS
.long HEADER_CHECKSUM
# -------------------------------------------------------


# -------------------------------------------------------
# 内核栈，分页开启前后都会使用
.section .data
.align 4
kstack_bottom:
    .space 0x100000                 # 1M
kstack_top:
# -------------------------------------------------------


# -------------------------------------------------------
# gdt表设置和段选择子声明
.set KERN_CS,       0x08            # 内核代码段
.set KERN_DS,       0x10            # 内核数据段
.set USER_CS,       0x18            # 用户代码段
.set USER_DS,       0x20            # 用户数据段

.set SGP_ACCESSED,  1 << 0          # 已访问
.set SGP_READABLE,  1 << 1          # 可读
.set SGP_CONFIRM,   1 << 2          # confirming
.set SGP_CODE,      1 << 3          # 代码段
.set SGP_NSYS,      1 << 4          # code/data段
.set SGP_KERNEL,    0 << 5 | 0 << 6 # 内核权限
.set SGP_USER,      1 << 5 | 1 << 6 # 用户权限
.set SGP_PRESENT,   1 << 7

.macro SEG_NULL
    .word 0, 0
    .byte 0, 0, 0, 0
.endm

# 参数为type,base,limit
.macro SEG_CRT type, base, lim
    .word ((\lim >> 12) & 0xFFFF), ((\base) & 0xFFFF)
    .byte ((\base) >> 16 & 0xFF), ((\type) & 0xFF)
    .byte (0xC0 | (((\lim) >> 28) & 0xF)), (((\base) >> 24) & 0xFF)
.endm

.set KERN_CODE, SGP_CODE | SGP_READABLE | SGP_KERNEL | SGP_PRESENT | SGP_NSYS
.set KERN_DATA, SGP_READABLE | SGP_KERNEL | SGP_PRESENT | SGP_NSYS
.set USER_CODE, SGP_CODE | SGP_READABLE | SGP_USER | SGP_PRESENT | SGP_NSYS
.set USER_DATA, SGP_READABLE | SGP_USER | SGP_PRESENT | SGP_NSYS

# gdt表初始化
.section .rodata
.align 4
gdtable:
    .word 0, 0, 0, 0
    SEG_CRT KERN_CODE, 0x0, 0xFFFFFFFF
    SEG_CRT KERN_DATA, 0x0, 0xFFFFFFFF
    SEG_CRT USER_CODE, 0x0, 0xFFFFFFFF
    SEG_CRT USER_DATA, 0x0, 0xFFFFFFFF
gdtdesc:
    .word gdtdesc - gdtable -1      # sizeof(gdtable) -1, 5*8-1
    .long gdtable
# -------------------------------------------------------


# -------------------------------------------------------
# 内核引导代码，此时未打开分页机制
.section .text
.global kern_entry
kern_entry = kern_boot
kern_boot:
    cli
    cld
page_enable:
    # 开启4M页面支持
    movl %cr4, %ecx
    orl $(1 << 4), %ecx
    movl %ecx, %cr4
    # 载入页目录
    movl $(boot_pgdir-KERNBASE), %ecx
    movl %ecx, %cr3
    # 打开分页
    movl %cr0, %ecx
    orl $((1 << 31) | (1 << 15)), %ecx
    movl %ecx, %cr0
gdt_setup:
    lgdt gdtdesc
    ljmp $KERN_CS, $gdt_flush
gdt_flush:
    movw $KERN_DS, %cx
    movw %cx, %ds
    movw %cx, %es
    movw %cx, %fs
    movw %cx, %gs
    movw %cx, %ss
 call_bootstrap:
    movl $(kstack_top), %esp
    pushl %ebx
    pushl %eax
    call kern_bootstrap
.type kern_spin, @function
kern_spin:
    jmp kern_spin
# -------------------------------------------------------
