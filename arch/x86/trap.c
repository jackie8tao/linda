#include <trap.h>
#include <types.h>
#include <generic.h>
#include <drivers/pic.h>
#include <kprintf.h>
#include <string.h>
#include <mach.h>

// 中断向量表
static struct gatedesc idt[NIDT];

// 中断处理函数
static trap_handler_t trap_handlers[NIDT];

// 内核允许使用的中断
static uint_t allowed_traps[] = {
    CPU_DE, CPU_DB, CPU_NMI, CPU_BP, CPU_OF,
    CPU_BR, CPU_UD, CPU_NM, CPU_DF, CPU_NOT_USE1,
    CPU_TS, CPU_NP, CPU_SS, CPU_GP, CPU_PF,
    CPU_NOT_USE2, CPU_MF, CPU_AC, CPU_MC,
    CPU_XM, IRQ0, IRQ1, IRQ2, IRQ3, IRQ4,
    IRQ5, IRQ6, IRQ7, IRQ8, IRQ9, IRQ10,
    IRQ11, IRQ12, IRQ13, IRQ14, IRQ15,
    IRQ_SYSCALL
};

// vectors.py中生成的中断处理程序表
extern uint_t trap_handlers_table[];

// 初始化中断相关
void
trap_init()
{
    memset((void*)idt, 0, sizeof(idt));
    memset((void*)trap_handlers, 0, sizeof(trap_handlers));

    pic_remap(0x20, 0x28);

    for(int i=0; i<NIDT; ++i){
        SETGATE(idt[i], 0, 0x08, trap_handlers_table[i], DPL_KERN);
    }
    SETGATE(idt[IRQ_SYSCALL], 1, 0x08, trap_handlers_table[IRQ_SYSCALL], DPL_USER);

    lidt(idt, sizeof(idt));

    kprintf("interrupt table initialize finished!\n");
}

// 中断调配程序
void
interrupt_handler_dispatcher(struct trapframe *frame)
{
    if(frame->trapno>=40){
        // asm volatile("outb %0, %1": :"a"(0x20), "d"(0xA0));
        outb(0xA0, 0x20);
    }
    asm volatile("outb %0, $0x20": :"al"(0x20));
    // outb(0x20, 0x20);
    if(frame->trapno == IRQ7){
        return;
    }

    trap_handler_t handler = trap_handlers[frame->trapno];
    if(!handler){
        panic("Trap[%d] has no handler, errno: %d, eip: 0x%x!\n",
            frame->trapno, frame->err, frame->eip);
        return;
    }

    handler(frame);
}

// 注册中断处理函数
void
register_trap_handler(uint_t trapno, trap_handler_t handler)
{
    int allowed_traps_count = sizeof(allowed_traps)/sizeof(uint_t);

    int found = 0;
    for(int i=allowed_traps_count-1; i>=0; --i){
        if(allowed_traps[i]==trapno){
            found = 1;
            break;
        }
    }

    if(found){
        trap_handlers[trapno] = handler;
    }else{
        panic("The trap[%d] is not allowed\n", trapno);
    }
}
