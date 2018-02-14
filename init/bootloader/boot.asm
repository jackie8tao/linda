; bootloader的引导代码
; 设置段，A20总线，引导载入代码


; ==============================标志区===========================================

; 保护模式开启标志位
.set CR0_PE,    0x1 << 0

; 保护模式下内核代码段和数据段
.set KSEG_CODE, 0x1 << 3
.set KSEG_DATA, 0x2 << 3

; ==============================================================================
.code16
.global boot16_start, boot32_start, loader_start
.type boot16_start, @function
boot16_start:
    ; 关闭中断
    cli

    ; 段设置
    xorw    %ax, %ax
    movw    %ax, %ds
    movw    %ax, %es
    movw    %ax, %ss

    ; 激活A20总线
seta20_a:
    inb     $0x64, %al
    testb   $0x2, %al
    jnz     seta20_a

    movb    $0xd1, %al
    outb    %al, $0x64

seta20_b:
    inb     $0x64, %al
    testb   $0x2, %al
    jnz     seta20_b

    movb    $0xdf, %al
    outb    %al, $0x60    

    ; 载入gdt表并进入保护模式
    lgdt    boot_gdtdesc
    movl    %cr0, %eax
    orl     $CR0_PE, %eax
    movl    %eax, %cr0

    ; 跳转到保护模式
    ljmp    $KSEG_CODE, $boot32_start

.code32
.type boot32_start, @function
boot32_start:
    ; 设置保护模式下段选择子
    movw    $KSEG_DATA, %ax
    movw    %ax, %ds
    movw    %ax, %es
    movw    %ax, %ss
    movw    %ax, %fs
    movw    %ax, %gs

    ; 设置栈并调用loader_start函数
    movl    $stack_bottom, %esp
    call    loader_start
spin:
    jmp     spin

; bootloader栈
stack_top:
    .space 0x10, 0x0
stack_bottom:

; 引导部分的gdt表
.p2align 2
boot_gdt:
    ; null段
    .long   0, 0

    ; 内核code段
    .word   0xFFFF, 0x0000
    .byte   0x00, 0x9A, 0xCF, 0x00
    
    ; 内核数据段
    .word   0xFFFF, 0x0000
    .byte   0x00, 0x92, 0xCF, 0x00

boot_gdtdesc:
    .word   (boot_gdtdesc - boot_gdt -1)
    .long   boot_gdt