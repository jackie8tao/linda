# 中断处理程序trap_handlers_table

.code32
.section .data
.global trap_handler_dispatcher
.global trap_handlers
trap_handlers:
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs
    pushal

    # 设置内核段寄存器
    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs

    # 压入trapframe指针参数，并调用trap_handler_dispatcher
    pushl %esp
    call interrupt_handler_dispatcher
    addl $4, %esp

.global trapret
trapret:
    popal
    popl %gs
    popl %fs
    popl %es
    popl %ds
    addl $8, %esp
    iret
