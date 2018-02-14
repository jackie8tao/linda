#include <idt.h>
#include <panic.h>

#define IDT_SIZE 256
#define GATE_GENERATE(idx) cnt_set((idx), (uint_t)ipt_handler##idx)

struct idt_entry idtable[IDT_SIZE];
struct idt_pointer idtptr;
ipt_handler_t ipt_handler_pool[IDT_SIZE];

// 设置表项内容
static void
cnt_set(int idx, uint_t offset)
{
    idtable[idx].offset_lower = (ushort_t)(offset & 0xFFFF);
    idtable[idx].segment = 0x10;
    idtable[idx].zero = 0x0;
    idtable[idx].flags = 0x8E;
    idtable[idx].offset_higher = (ushort_t)((offset >> 16) & 0xFFFF);
}

// 创建idt表项
static void
idt_create()
{
    idtptr.limit = sizeof(struct idt_entry) * IDT_SIZE - 1;
    idtptr.address = (uint_t)&idtable;

    // 注册中断处理函数到idt表
    GATE_GENERATE(0);
    GATE_GENERATE(1);
    GATE_GENERATE(2);
    GATE_GENERATE(3);
    GATE_GENERATE(4);
    GATE_GENERATE(5);
    GATE_GENERATE(6);
    GATE_GENERATE(7);
    GATE_GENERATE(8);
    GATE_GENERATE(9);
    GATE_GENERATE(10);
    GATE_GENERATE(11);
    GATE_GENERATE(12);
    GATE_GENERATE(13);
    GATE_GENERATE(14);
    GATE_GENERATE(15);
    GATE_GENERATE(16);
    GATE_GENERATE(17);
    GATE_GENERATE(18);
    GATE_GENERATE(19);
    GATE_GENERATE(20);
    GATE_GENERATE(21);
    GATE_GENERATE(22);
    GATE_GENERATE(23);
    GATE_GENERATE(24);
    GATE_GENERATE(25);
    GATE_GENERATE(26);
    GATE_GENERATE(27);
    GATE_GENERATE(28);
    GATE_GENERATE(29);
    GATE_GENERATE(30);
    GATE_GENERATE(31);
}

// 注册idt表到cpu
static void
idt_register()
{
    asm volatile(
    "lidt (%%eax)"
    :
    :"a"(&idtptr)
    );
}

// ipt的处理者
void
ipt_man(struct ipt_cxt* cxt)
{
    if (ipt_handler_pool[cxt->iptno]) {
        ipt_handler_pool[cxt->iptno](cxt);
    } else {
        panic("Cannot handle the interrupt!");
    }
}

void
idt_desct_rgst(int idx, idt_desct_t desct)
{
    cnt_set(idx, (uint_t)desct);
}

void
ipt_hdl_rgst(int idx, ipt_handler_t hdl)
{
    ipt_handler_pool[idx] = hdl;
}

void
idt_init()
{
    idt_create();
    idt_register();
}