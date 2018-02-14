#ifndef __LINDA_IDT_H
#define __LINDA_IDT_H

#include <types.h>

struct ipt_cxt {
    // 手动压入
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

    // 处理器自动压入
    uint_t eip;
    uint_t cs;
    uint_t eflags;
    uint_t useresp;
    uint_t ss;
};

struct idt_entry {
    ushort_t offset_lower;
    ushort_t segment;
    uchar_t zero;
    uchar_t flags;
    ushort_t offset_higher;
}__attribute__((packed));

struct idt_pointer {
    ushort_t limit;
    uint_t address;
}__attribute__((packed));

typedef void (*ipt_handler_t)(struct ipt_cxt*);
typedef void (*idt_desct_t)();

// idt表初始化
void idt_init();

// 注册ipt中断处理程序
void ipt_hdl_rgst(int idx, ipt_handler_t hdl);

// 注册中断项信息
void idt_desct_rgst(int idx, idt_desct_t desct);

// CPU异常中断
void ipt_handler0();        // 0 #DE 除 0 异常
void ipt_handler1();        // 1 #DB 调试异常
void ipt_handler2();        // 2 NMI
void ipt_handler3();        // 3 BP 断点异常
void ipt_handler4();        // 4 #OF 溢出
void ipt_handler5();        // 5 #BR 对数组的引用超出边界
void ipt_handler6();        // 6 #UD 无效或未定义的操作码
void ipt_handler7();        // 7 #NM 设备不可用(无数学协处理器)
void ipt_handler8();        // 8 #DF 双重故障(有错误代码)
void ipt_handler9();        // 9 协处理器跨段操作
void ipt_handler10();       // 10 #TS 无效TSS(有错误代码)
void ipt_handler11();       // 11 #NP 段不存在(有错误代码)
void ipt_handler12();       // 12 #SS 栈错误(有错误代码)
void ipt_handler13();       // 13 #GP 常规保护(有错误代码)
void ipt_handler14();       // 14 #PF 页故障(有错误代码)
void ipt_handler15();       // 15 CPU 保留
void ipt_handler16();       // 16 #MF 浮点处理单元错误
void ipt_handler17();       // 17 #AC 对齐检查
void ipt_handler18();       // 18 #MC 机器检查
void ipt_handler19();       // 19 #XM SIMD(单指令多数据)浮点异常

// 20 ~ 31 Intel 保留
void ipt_handler20();
void ipt_handler21();
void ipt_handler22();
void ipt_handler23();
void ipt_handler24();
void ipt_handler25();
void ipt_handler26();
void ipt_handler27();
void ipt_handler28();
void ipt_handler29();
void ipt_handler30();
void ipt_handler31();

#endif
