.code32

.macro IPT_NERRNO idx
    .global ipt_handler\idx
    .type ipt_handler\idx, @function
    ipt_handler\idx:
        push $0x0                           # 压入无效的错误代码
        push $\idx                          # 压入中断号
        jmp ipt_engine                      # 跳入到中断处理引擎中处理中断
.endm

.macro IPT_WERRNO idx
    .global ipt_handler\idx
    .type ipt_handler\idx, @function
    ipt_handler\idx:
        push $\idx                          # 压入中断号
        jmp ipt_engine                      # 跳入到中断处理引擎中处理中断
.endm

.macro IRQ seq, idx
    .global irq\seq
    .type irq\seq, @function
    irq\seq:
        push $0x0                           # 压入无效错误码
        push $\idx                          # 压入中断号
        jmp irq_engine                      # 跳转到irq处理程序
.endm

.section .text
.type ipt_engine, @function
ipt_engine:
    pusha                                   # 顺序压入eax, ecx, edx, ebx, esp, ebp, esi, edi

    movw %ds, %ax
    pushl %eax                              # 保存数据段
    cli
    movw $0x10, %ax                         # 加载内核段
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss

    pushl %esp
    call ipt_man                            # 调用各个中断的实际处理程序
    addl $0x4, %esp

    #xchgw %bx, %bx
    popl %ebx
    movw %bx, %ds
    movw %bx, %es
    movw %bx, %fs
    movw %bx, %gs
    movw %bx, %ss

    popa

    addl $0x8, %esp
    sti
    iret

.type irq_engine, @function
irq_engine:
    pusha

    movw %ds, %ax
    pushl %eax                              # 保存数据段
    cli
    movw $0x10, %ax                         # 加载内核段
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss

    pushl %esp
    call irq_man                            # 调用各个中断的实际处理程序
    addl $0x4, %esp

    #xchgw %bx, %bx
    popl %ebx
    movw %bx, %ds
    movw %bx, %es
    movw %bx, %fs
    movw %bx, %gs
    movw %bx, %ss

    popa

    addl $0x8, %esp
    sti
    iret

# 定义中断处理函数
IPT_NERRNO  0                               # 0 #DE 除 0 异常
IPT_NERRNO  1                               # 1 #DB 调试异常
IPT_NERRNO  2                               # 2 NMI
IPT_NERRNO  3                               # 3 BP 断点异常
IPT_NERRNO  4                               # 4 #OF 溢出
IPT_NERRNO  5                               # 5 #BR 对数组的引用超出边界
IPT_NERRNO  6                               # 6 #UD 无效或未定义的操作码
IPT_NERRNO  7                               # 7 #NM 设备不可用(无数学协处理器)
IPT_WERRNO  8                               # 8 #DF 双重故障(有错误代码)
IPT_NERRNO  9                               # 9 协处理器跨段操作
IPT_WERRNO  10                              # 10 #TS 无效TSS(有错误代码)
IPT_WERRNO  11                              # 11 #NP 段不存在(有错误代码)
IPT_WERRNO  12                              # 12 #SS 栈错误(有错误代码)
IPT_WERRNO  13                              # 13 #GP 常规保护(有错误代码)
IPT_WERRNO  14                              # 14 #PF 页故障(有错误代码)
IPT_NERRNO  15                              # 15 CPU 保留
IPT_NERRNO  16                              # 16 #MF 浮点处理单元错误
IPT_WERRNO  17                              # 17 #AC 对齐检查
IPT_NERRNO  18                              # 18 #MC 机器检查
IPT_NERRNO  19                              # 19 #XM SIMD(单指令多数据)浮点异常

# 20 ~ 31 Intel 保留
IPT_NERRNO 20
IPT_NERRNO 21
IPT_NERRNO 22
IPT_NERRNO 23
IPT_NERRNO 24
IPT_NERRNO 25
IPT_NERRNO 26
IPT_NERRNO 27
IPT_NERRNO 28
IPT_NERRNO 29
IPT_NERRNO 30
IPT_NERRNO 31

# IRQ
IRQ   0,    32                              # 电脑系统计时器
IRQ   1,    33                              # 键盘
IRQ   2,    34                              # 与 IRQ9 相接，MPU-401 MD 使用
IRQ   3,    35                              # 串口设备
IRQ   4,    36                              # 串口设备
IRQ   5,    37                              # 建议声卡使用
IRQ   6,    38                              # 软驱传输控制使用
IRQ   7,    39                              # 打印机传输控制使用
IRQ   8,    40                              # 即时时钟
IRQ   9,    41                              # 与 IRQ2 相接，可设定给其他硬件
IRQ  10,    42                              # 建议网卡使用
IRQ  11,    43                              # 建议 AGP 显卡使用
IRQ  12,    44                              # 接 PS/2 鼠标，也可设定给其他硬件
IRQ  13,    45                              # 协处理器使用
IRQ  14,    46                              # IDE0 传输控制使用
IRQ  15,    47                              # IDE1 传输控制使用