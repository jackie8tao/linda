#include <trap.h>
#include <types.h>
#include <generic.h>
#include <drivers/pic.h>
#include <kprintf.h>
#include <string.h>

#define TRAP_HANDLER(trapno) (uint_t)trap_handler_##trapno
#define IDT_GATE_SET(trapno, handler) \
    idt[trapno].offset_lower=((handler) & 0xFFFF); \
    idt[trapno].segment=0x08; \
    idt[trapno].zero = 0; \
    idt[trapno].flags = 0x8E; \
    idt[trapno].offset_higher = (((handler) >> 16) & 0xFFFF);

static struct idt_pointer idtr;
static struct idt_entry idt[IDT_MAX_COUNT];
static trap_handler_t trap_handlers[IDT_MAX_COUNT];

static uint_t allowed_traps[] = {
    CPU_DE, CPU_DB, CPU_NMI, CPU_BP, CPU_OF,
    CPU_BR, CPU_UD, CPU_NM, CPU_DF, CPU_NOT_USE1,
    CPU_TS, CPU_NP, CPU_SS, CPU_GP, CPU_PF,
    CPU_NOT_USE2, CPU_MF, CPU_AC, CPU_MC,
    CPU_XM, IRQ0, IRQ1, IRQ2, IRQ3, IRQ4,
    IRQ5, IRQ6, IRQ7, IRQ8, IRQ9, IRQ10,
    IRQ11, IRQ12, IRQ13, IRQ14, IRQ15
};

void
trap_init()
{
    memset((void*)idt, 0, sizeof(idt));
    memset((void*)trap_handlers, 0, sizeof(trap_handlers));

    pic_remap(0x20, 0x28);

    {
        // it`s so ugly!
        IDT_GATE_SET(0, TRAP_HANDLER(0));
        IDT_GATE_SET(1, TRAP_HANDLER(2));
        IDT_GATE_SET(2, TRAP_HANDLER(2));
        IDT_GATE_SET(3, TRAP_HANDLER(3));
        IDT_GATE_SET(4, TRAP_HANDLER(4));
        IDT_GATE_SET(5, TRAP_HANDLER(5));
        IDT_GATE_SET(6, TRAP_HANDLER(6));
        IDT_GATE_SET(7, TRAP_HANDLER(7));
        IDT_GATE_SET(8, TRAP_HANDLER(8));
        IDT_GATE_SET(9, TRAP_HANDLER(9));
        IDT_GATE_SET(10, TRAP_HANDLER(10));
        IDT_GATE_SET(11, TRAP_HANDLER(11));
        IDT_GATE_SET(12, TRAP_HANDLER(12));
        IDT_GATE_SET(13, TRAP_HANDLER(13));
        IDT_GATE_SET(14, TRAP_HANDLER(14));
        IDT_GATE_SET(15, TRAP_HANDLER(15));
        IDT_GATE_SET(16, TRAP_HANDLER(16));
        IDT_GATE_SET(17, TRAP_HANDLER(17));
        IDT_GATE_SET(18, TRAP_HANDLER(18));
        IDT_GATE_SET(19, TRAP_HANDLER(19));
        IDT_GATE_SET(32, TRAP_HANDLER(32));
        IDT_GATE_SET(33, TRAP_HANDLER(33));
        IDT_GATE_SET(34, TRAP_HANDLER(34));
        IDT_GATE_SET(35, TRAP_HANDLER(35));
        IDT_GATE_SET(36, TRAP_HANDLER(36));
        IDT_GATE_SET(37, TRAP_HANDLER(37));
        IDT_GATE_SET(38, TRAP_HANDLER(38));
        IDT_GATE_SET(39, TRAP_HANDLER(39));
        IDT_GATE_SET(40, TRAP_HANDLER(40));
        IDT_GATE_SET(41, TRAP_HANDLER(41));
        IDT_GATE_SET(42, TRAP_HANDLER(42));
        IDT_GATE_SET(43, TRAP_HANDLER(43));
        IDT_GATE_SET(44, TRAP_HANDLER(44));
        IDT_GATE_SET(45, TRAP_HANDLER(45));
        IDT_GATE_SET(46, TRAP_HANDLER(46));
        IDT_GATE_SET(47, TRAP_HANDLER(47));
        IDT_GATE_SET(64, TRAP_HANDLER(64));
    }

    idtr.limit = sizeof(idt) - 1;
    idtr.address = (uint_t)(&idt);
    asm volatile("lidt (%0)"::"r"(&idtr));

    kprintf("interrupt table initialize finished!\n");
}

void
trap_handler_dispatcher(struct trapframe *frame)
{
    sti();
    if(frame->trapno >=40){
        outb(0xA0, 0x20);
    }
    outb(0x20, 0x20);

    trap_handler_t handler = trap_handlers[frame->trapno];
    if(!handler){
        if(frame->trapno != 39){
            kprintf("Trap[%d] has no handler!\n", frame->trapno);
        }
        cli();
        return;
    }
    handler(frame);
    cli();
}

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
        kprintf("The trap[%d] is not allowed\n", trapno);
    }
}

#undef TRAP_HANDLER
