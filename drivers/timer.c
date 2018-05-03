#include <drivers/timer.h>
#include <trap.h>
#include <types.h>
#include <generic.h>
#include <scheduler.h>
#include <process.h>
#include <spinlock.h>

#define TIMER_FREQUENCY 1000

// 这里将会处理进程调度问题
void
timer_handler(struct trapframe *tf)
{
    // kprintf("eip: 0x%x\n", tf->eip);
    struct cpu *curcpu = current_cpu();
    if(curcpu->curprocess != 0){
        // kprintf("0x%x ", curcpu->curprocess);
        if(curcpu->curprocess->state == RUNNING){
            yield_cpu();
        }
    }
}

// 设置时钟中断相关配置
void
timer_setup()
{
    uint_t divisor = 1193180/TIMER_FREQUENCY;

    outb(0x43, 0x36);
    outb(0x40, divisor & 0xFF);
    outb(0x40, (divisor >> 8) & 0xFF);

    register_trap_handler(IRQ0, timer_handler);
}
