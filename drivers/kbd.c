#include <drivers/kbd.h>
#include <trap.h>
#include <drivers/console.h>
#include <types.h>
#include <generic.h>

static void
kbd_trap_handler(struct trapframe *tf)
{
    char ch;
    if(-1 != (ch = (char)kbd_read())){
        console_put(ch);
    }
}

void
kbd_init()
{
    register_trap_handler(IRQ1, kbd_trap_handler);
}

int
kbd_read()
{
    static uint_t shift;
    static uchar_t *charcode[4] = {
        normalmap, shiftmap, ctlmap, ctlmap
    };

    uint_t st, data, ch;
    st = inb(KBSTATP);
    if((st & KBS_DIB) == 0){
        return -1;
    }
    data = inb(KBDATAP);
    if(0xE0 == data){
        shift |= E0ESC;
        return 0;
    }else if(0x80 & data){
        data = (shift & E0ESC)? data : (data & 0x7F);
        shift &= ~(shiftcode[data] | E0ESC);
        return 0;
    }else if(shift & E0ESC){
        data |= 0x80;
        shift &= ~E0ESC;
    }

    shift |= shiftcode[data];
    shift ^= togglecode[data];
    ch = charcode[shift & (CTL|SHIFT)][data];
    if(shift & CAPSLOCK){
        if('a' <= ch && ch <= 'z'){
            ch += 'A' - 'a';
        }else if('A' <= ch && ch <= 'Z'){
            ch += 'a' - 'A';
        }
    }

    return ch;
}
