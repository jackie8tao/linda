#include <drivers/timer.h>
#include <trap.h>
#include <types.h>
#include <generic.h>

#define TIMER_FREQUENCY 10000

void 
timer_setup(trap_handler_t handler)
{
    register_trap_handler(IRQ0, handler);

    uint_t divisor = 1193180/TIMER_FREQUENCY;

    outb(0x43, 0x36);
    outb(0x40, divisor & 0xFF);
    outb(0x40, (divisor >> 8) & 0xFF);
}