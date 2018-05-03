/*
 * 中断管理，仅限pic硬件
 *
 * @author Jackie Tao <taodingfei@gmail.com>
 * @date 2018-03-21 22:50
 */

#ifndef __LINDA_TRAP_H
#define __LINDA_TRAP_H

#include <types.h>
#include <mach.h>

// CPU中断号定义
#define CPU_DE          0                       // 0 #DE 除 0 异常
#define CPU_DB          1                       // 1 #DB 调试异常
#define CPU_NMI         2                       // 2 NMI
#define CPU_BP          3                       // 3 BP 断点异常
#define CPU_OF          4                       // 4 #OF 溢出
#define CPU_BR          5                       // 5 #BR 对数组的引用超出边界
#define CPU_UD          6                       // 6 #UD 无效或未定义的操作码
#define CPU_NM          7                       // 7 #NM 设备不可用(无数学协处理器)
#define CPU_DF          8                       // 8 #DF 双重故障(有错误代码)
#define CPU_NOT_USE1    9                       // 9 协处理器跨段操作
#define CPU_TS          10                      // 10 #TS 无效TSS(有错误代码)
#define CPU_NP          11                      // 11 #NP 段不存在(有错误代码)
#define CPU_SS          12                      // 12 #SS 栈错误(有错误代码)
#define CPU_GP          13                      // 13 #GP 常规保护(有错误代码)
#define CPU_PF          14                      // 14 #PF 页故障(有错误代码)
#define CPU_NOT_USE2    15                      // 15 CPU 保留
#define CPU_MF          16                      // 16 #MF 浮点处理单元错误
#define CPU_AC          17                      // 17 #AC 对齐检查(有错误代码)
#define CPU_MC          18                      // 18 #MC 机器检查// 注册中断处理函数
#define CPU_XM          19                      // 19 #XM SIMD(单指令多数据)浮点异常

// 中断请求号定义
#define IRQ0            32                      // 电脑系统计时器
#define IRQ1            33                      // 键盘
#define IRQ2            34                      // 与 IRQ9 相接，MPU-401 MD 使用
#define IRQ3            35                      // 串口设备
#define IRQ4            36                      // 串口设备
#define IRQ5            37                      // 建议声卡使用
#define IRQ6            38                      // 软驱传输控制使用
#define IRQ7            39                      // 打印机传输控制使用
#define IRQ8            40                      // 即时时钟
#define IRQ9            41                      // 与 IRQ2 相接，可设定给其他硬件
#define IRQ10           42                      // 建议网卡使用
#define IRQ11           43                      // 建议 AGP 显卡使用
#define IRQ12           44                      // 接 PS/2 鼠标，也可设定给其他硬件
#define IRQ13           45                      // 协处理器使用
#define IRQ14           46                      // IDE0 传输控制使用
#define IRQ15           47                      // IDE1 传输控制使用

#define IRQ_SYSCALL     64                      // 系统调用中断

#define NIDT            256                     // idt表项的最大数量


typedef void (*trap_handler_t)(struct trapframe*);

void trap_init();

void trap_handler_dispatcher(struct trapframe *frame);

void register_trap_handler(uint_t trapno, trap_handler_t handler);

#endif
