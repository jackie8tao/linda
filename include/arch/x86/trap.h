/*
 * 中断管理，包括pic和apic部分
 *
 * @author Jackie Tao <taodingfei@gmail.com>
 * @date 2018-03-21 22:50
 */

#ifndef __LINDA_TRAP_H
#define __LINDA_TRAP_H

#include <types.h>

// CPU中断号定义
#define CPU_DE        0                     // 0 #DE 除 0 异常
#define CPU_DB        1                     // 1 #DB 调试异常
#define CPU_NMI       2                     // 2 NMI
#define CPU_BP        3                     // 3 BP 断点异常
#define CPU_OF        4                     // 4 #OF 溢出
#define CPU_BR        5                     // 5 #BR 对数组的引用超出边界
#define CPU_UD        6                     // 6 #UD 无效或未定义的操作码
#define CPU_NM        7                     // 7 #NM 设备不可用(无数学协处理器)
#define CPU_DF        8                     // 8 #DF 双重故障(有错误代码)
#define CPU_NOT_USE1  9                     // 9 协处理器跨段操作
#define CPU_TS        10                    // 10 #TS 无效TSS(有错误代码)
#define CPU_NP        11                    // 11 #NP 段不存在(有错误代码)
#define CPU_SS        12                    // 12 #SS 栈错误(有错误代码)
#define CPU_GP        13                    // 13 #GP 常规保护(有错误代码)
#define CPU_PF        14                    // 14 #PF 页故障(有错误代码)
#define CPU_NOT_USE2  15                    // 15 CPU 保留
#define CPU_MF        16                    // 16 #MF 浮点处理单元错误
#define CPU_AC        17                    // 17 #AC 对齐检查
#define CPU_MC        18                    // 18 #MC 机器检查
#define CPU_XM        19                    // 19 #XM SIMD(单指令多数据)浮点异常

// 中断请求号定义
#define IRQ0          32                    // 电脑系统计时器
#define IRQ1          33                    // 键盘
#define IRQ2          34                    // 与 IRQ9 相接，MPU-401 MD 使用
#define IRQ3          35                    // 串口设备
#define IRQ4          36                    // 串口设备
#define IRQ5          37                    // 建议声卡使用
#define IRQ6          38                    // 软驱传输控制使用
#define IRQ7          39                    // 打印机传输控制使用
#define IRQ8          40                    // 即时时钟
#define IRQ9          41                    // 与 IRQ2 相接，可设定给其他硬件
#define IRQ10         42                    // 建议网卡使用
#define IRQ11         43                    // 建议 AGP 显卡使用
#define IRQ12         44                    // 接 PS/2 鼠标，也可设定给其他硬件
#define IRQ13         45                    // 协处理器使用
#define IRQ14         46                    // IDE0 传输控制使用
#define IRQ15         47                    // IDE1 传输控制使用

#define IRQ_SYS       64                    // 系统调用中断

#define IDT_MAX_COUNT 256                   // idt表项的最大数量

#define TRAP_HANDLER_DECLARE(trapno) \
extern void trap_handler_##trapno();

struct trapframe {
    uint_t gs;
    uint_t fs;
    uint_t es;
    uint_t ds;
    uint_t edi;
    uint_t esi;
    uint_t ebp;
    uint_t esp;
    uint_t ebx;
    uint_t edx;
    uint_t ecx;
    uint_t eax;
    uint_t trapno;
    uint_t errno;
    uint_t eip;
    uint_t cs;
    uint_t eflags;
    uint_t useresp;
    uint_t ss;
} __attribute__((packed));

struct idt_entry {
    ushort_t offset_lower;
    ushort_t segment;
    uchar_t zero;
    uchar_t flags;
    ushort_t offset_higher;
} __attribute__((packed));

struct idt_pointer {
    ushort_t limit;
    uint_t address;
} __attribute__((packed));

typedef void (*trap_handler_t)(struct trapframe*);

// 中断初始化
void trap_init();

// 中断分配函数
void trap_handler_dispatcher(struct trapframe *frame);

// 注册中断处理函数
void register_trap_handler(uint_t trapno, trap_handler_t handler);

// 中断处理函数, it`s so ugly
TRAP_HANDLER_DECLARE(0);
TRAP_HANDLER_DECLARE(1);
TRAP_HANDLER_DECLARE(2);
TRAP_HANDLER_DECLARE(3);
TRAP_HANDLER_DECLARE(4);
TRAP_HANDLER_DECLARE(5);
TRAP_HANDLER_DECLARE(6);
TRAP_HANDLER_DECLARE(7);
TRAP_HANDLER_DECLARE(8);
TRAP_HANDLER_DECLARE(9);
TRAP_HANDLER_DECLARE(10);
TRAP_HANDLER_DECLARE(11);
TRAP_HANDLER_DECLARE(12);
TRAP_HANDLER_DECLARE(13);
TRAP_HANDLER_DECLARE(14);
TRAP_HANDLER_DECLARE(15);
TRAP_HANDLER_DECLARE(16);
TRAP_HANDLER_DECLARE(17);
TRAP_HANDLER_DECLARE(18);
TRAP_HANDLER_DECLARE(19);
TRAP_HANDLER_DECLARE(32);
TRAP_HANDLER_DECLARE(33);
TRAP_HANDLER_DECLARE(34);
TRAP_HANDLER_DECLARE(35);
TRAP_HANDLER_DECLARE(36);
TRAP_HANDLER_DECLARE(37);
TRAP_HANDLER_DECLARE(38);
TRAP_HANDLER_DECLARE(39);
TRAP_HANDLER_DECLARE(40);
TRAP_HANDLER_DECLARE(41);
TRAP_HANDLER_DECLARE(42);
TRAP_HANDLER_DECLARE(43);
TRAP_HANDLER_DECLARE(44);
TRAP_HANDLER_DECLARE(45);
TRAP_HANDLER_DECLARE(46);
TRAP_HANDLER_DECLARE(47);
TRAP_HANDLER_DECLARE(64);
// 中断处理函数

#endif
