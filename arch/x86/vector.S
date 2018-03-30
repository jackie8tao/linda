# 中断处理函数
# 
# @author Jackie Tao <taodingfei@gmail.com>
# @date 2018-03-22 23:48

.code32
.macro save_regs
    pushal
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs
.endm

.macro restore_regs
    popl %gs
    popl %fs
    popl %es
    popl %ds
    popal
.endm

.macro setup_kern_data_segment
    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
.endm

.macro trap_handler_with_errno trapno
.global trap_handler_\trapno
trap_handler_\trapno:
#    xchgw %bx, %bx
    cli
    pushl $\trapno
    save_regs
    pushl %esp
    setup_kern_data_segment
    call trap_handler_dispatcher
    addl $4, %esp
    restore_regs
    addl $8, %esp
    iret
.endm

.macro trap_handler_without_errno trapno
.global trap_handler_\trapno
trap_handler_\trapno:
#    xchgw %bx, %bx
    cli
    pushl $0
    pushl $\trapno
    save_regs
    pushl %esp
    setup_kern_data_segment
    call trap_handler_dispatcher
    addl $4, %esp
    restore_regs
    addl $8, %esp
    iret
.endm

# 处理函数宏定义表, it`s so ugly
trap_handler_without_errno 0
trap_handler_without_errno 1
trap_handler_without_errno 2
trap_handler_without_errno 3
trap_handler_without_errno 4
trap_handler_without_errno 5
trap_handler_without_errno 6
trap_handler_without_errno 7
trap_handler_with_errno 8
trap_handler_without_errno 9
trap_handler_with_errno 10
trap_handler_with_errno 11
trap_handler_with_errno 12
trap_handler_with_errno 13
trap_handler_with_errno 14
trap_handler_without_errno 15
trap_handler_without_errno 16
trap_handler_without_errno 17
trap_handler_without_errno 18
trap_handler_without_errno 19
trap_handler_without_errno 32
trap_handler_without_errno 33
trap_handler_without_errno 34
trap_handler_without_errno 35
trap_handler_without_errno 36
trap_handler_without_errno 37
trap_handler_without_errno 38
trap_handler_without_errno 39
trap_handler_without_errno 40
trap_handler_without_errno 41
trap_handler_without_errno 42
trap_handler_without_errno 43
trap_handler_without_errno 44
trap_handler_without_errno 45
trap_handler_without_errno 46
trap_handler_without_errno 47
trap_handler_without_errno 64
# 处理函数宏定义表
