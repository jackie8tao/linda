#include <irq.h>
#include <generic.h>

#define MP1 0x20                                                    // 主片端口0x20
#define MP2 0x21                                                    // 主片端口0x21
#define SP1 0xA0                                                    // 从片端口0xA0
#define SP2 0xA1                                                    // 从片端口0xA1

irq_handler_t irq_handler_pool[16];

// IRQ表重映射
static void
irq_remap()
{
    outb(MP1, 0x11), outb(SP1, 0x11);                               // 初始化主片和从片
    outb(MP2, 0x20), outb(SP2, 0x28);                               // 主片从32号，从片从40号开始中断号
    outb(MP2, 0x04), outb(SP2, 0x02);                               // 主片和从片建立关联
    outb(MP2, 0x01), outb(SP2, 0x01);                               // 设置8086工作模式
    outb(MP2, 0x00), outb(SP2, 0x00);                               // 主从片允许中断
}

// 注册IRQ到gdt
static void
irq_rgst()
{
    idt_desct_rgst(IRQ0, irq0);
    idt_desct_rgst(IRQ1, irq1);
    idt_desct_rgst(IRQ2, irq2);
    idt_desct_rgst(IRQ3, irq3);
    idt_desct_rgst(IRQ4, irq4);
    idt_desct_rgst(IRQ5, irq5);
    idt_desct_rgst(IRQ6, irq6);
    idt_desct_rgst(IRQ7, irq7);
    idt_desct_rgst(IRQ8, irq8);
    idt_desct_rgst(IRQ9, irq9);
    idt_desct_rgst(IRQ10, irq10);
    idt_desct_rgst(IRQ11, irq11);
    idt_desct_rgst(IRQ12, irq12);
    idt_desct_rgst(IRQ13, irq13);
    idt_desct_rgst(IRQ14, irq14);
    idt_desct_rgst(IRQ15, irq15);
}

void
irq_hdl_rgst(int seq, irq_handler_t hdl)
{
    irq_handler_pool[seq] = hdl;
}

void
irq_man(struct ipt_cxt* cxt)
{
    if (cxt->iptno >= IRQ8) {
        outb(SP1, 0x20);
    }
    outb(MP1, 0x20);

    if (irq_handler_pool[cxt->iptno]) {
        irq_handler_pool[cxt->iptno](cxt);
    }
}

void
irq_init()
{
    irq_remap();
    irq_rgst();
}