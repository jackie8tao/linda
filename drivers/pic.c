#include <drivers/pic.h>
#include <types.h>
#include <generic.h>

#define MASTER_CMD_PORT     0x20
#define MASTER_DATA_PORT    0x21
#define SLAVE_CMD_PORT      0xA0
#define SLAVE_DATA_PORT     0xA1

#define ICW1_ICW4	        0x01		/* ICW4 (not) needed */
#define ICW1_SINGLE	        0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	    0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	        0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	        0x10		/* Initialization - required! */
 
#define ICW4_8086	        0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	        0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	    0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	    0x0C		/* Buffered mode/master */
#define ICW4_SFNM	        0x10		/* Special fully nested (not) */

void 
pic_remap(ushort_t master_offset, ushort_t slave_offset)
{   
    outb(MASTER_CMD_PORT, ICW1_INIT+ICW1_ICW4);
	outb(SLAVE_CMD_PORT, ICW1_INIT+ICW1_ICW4);

	outb(MASTER_DATA_PORT, master_offset);
	outb(SLAVE_DATA_PORT, slave_offset);

	outb(MASTER_DATA_PORT, 4);
	outb(SLAVE_DATA_PORT, 2);
 
	outb(MASTER_DATA_PORT, ICW4_8086);
	outb(SLAVE_DATA_PORT, ICW4_8086);
}

void 
pic_disable()
{
    asm volatile(
        "movl %0, %%eax\n\t"
        "outb %%al, $0xA1\n\t"
        "outb %%al, $0x21\n\t"
        :
        : "r"(0xFF)
    );
}