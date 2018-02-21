/*
 * 这里并没有使用APIC功能，也就是未加入SMP功能.
 * 这么做主要是为了简化前期的开发工作，关注重点，
 * 不仅仅纠结于某几个硬件
 */

#ifndef __LINDA_APIC_H
#define __LINDA_APIC_H

#include <types.h>

#define IDTSIZE 256

struct gate_descriptor {
    ushort_t offset_lower;      // 处理程序地址的低16位
    ushort_t segment;           // 处理程序所在段
    uchar_t always0;            // 全部为0
    uchar_t flags;              // 标志
    ushort_t offset_higher;     // 处理程序地址的高16位
} __attribute__((packed));

struct idt_descriptor {
    ushort_t limit;
    uint_t base;
} __attribute__((packed));

struct cpu_context {
    uint_t ds;
    uint_t edi;
    uint_t esi;
    uint_t ebp;
    uint_t esp;
    uint_t ebx;
    uint_t edx;
    uint_t ecx;
    uint_t eax;
    uint_t iptno;
    uint_t errno;
    uint_t eip;
    uint_t cs;
    uint_t eflags;
    uint_t useresp;
    uint_t ss;
} __attribute__((packed));

typedef void (*ipt_handler_t)(struct cpu_context*);

void apic_init();

void register_ipt_handler(uint_t ipt_no, ipt_handler_t handler);

#ifndef _APIC_IRQ
#define _APIC_IRQ

#define IRQ0    32          // 电脑系统计时器
#define IRQ1     33        // 键盘
#define IRQ2     34        // 与 IRQ9 相接，MPU-401 MD 使用
#define IRQ3     35        // 串口设备
#define IRQ4     36        // 串口设备
#define IRQ5     37        // 建议声卡使用
#define IRQ6     38        // 软驱传输控制使用
#define IRQ7     39        // 打印机传输控制使用
#define IRQ8     40        // 即时时钟
#define IRQ9     41        // 与 IRQ2 相接，可设定给其他硬件
#define IRQ10    42        // 建议网卡使用
#define IRQ11    43        // 建议 AGP 显卡使用
#define IRQ12    44        // 接 PS/2 鼠标，也可设定给其他硬件
#define IRQ13    45        // 协处理器使用
#define IRQ14    46        // IDE0 传输控制使用
#define IRQ15    47        // IDE1 传输控制使用

#endif

#ifndef _APIC_HW
#define _APIC_HW

#define HW0     0           // #DE 除 0 异常
#define HW1     1           // #DB 调试异常
#define HW2     2           // NMI
#define HW3     3           // BP 断点异常
#define HW4     4           // #OF 溢出
#define HW5     5           // #BR 对数组的引用超出边界
#define HW6     6           // #UD 无效或未定义的操作码
#define HW7     7           // #NM 设备不可用(无数学协处理器)
#define HW8     8           // #DF 双重故障(有错误代码)
#define HW9     9           // 协处理器跨段操作
#define HW10    10          // #TS 无效TSS(有错误代码)
#define HW11    11          // #NP 段不存在(有错误代码)
#define HW12    12          // #SS 栈错误(有错误代码)
#define HW13    13          // #GP 常规保护(有错误代码)
#define HW14    14          // #PF 页故障(有错误代码)
#define HW15    15          // CPU 保留
#define HW16    16          // #MF 浮点处理单元错误
#define HW17    17          // #AC 对齐检查
#define HW18    18          // #MC 机器检查
#define HW19    19          // #XM SIMD(单指令多数据)浮点异常

#endif

#endif
