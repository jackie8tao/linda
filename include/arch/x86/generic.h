#ifndef __LINDA_GENERIC_H
#define __LINDA_GENERIC_H

#include <types.h>
#include <mach.h>

static inline uchar_t
inb(ushort_t port)
{
    uchar_t res;
    asm volatile("inb %1, %0" :"=a"(res) :"d"(port));
    return res;
}

static inline void
outb(ushort_t port, uchar_t cnt)
{
    asm volatile("outb %1, %0" : : "d"(port), "a"(cnt));
}

static inline ushort_t
inw(ushort_t port)
{
    uchar_t res;
    asm volatile("inw %1, %0" :"=a"(res) :"r"(port));
    return res;
}

static inline void
outw(ushort_t port, ushort_t cnt)
{
    asm volatile("outw %1, %0" : : "d"(port), "a"(cnt));
}

static inline uint_t
inl(ushort_t port)
{
    uint_t res;
    asm volatile("inl %1, %0" :"=a"(res) :"r"(port));
    return res;
}

static inline void
outl(ushort_t port, uint_t cnt)
{
    asm volatile("outl %1, %0" : : "d"(port), "a"(cnt));
}

static inline void
insb(ushort_t port, void* dst, int count)
{
    asm volatile("cld;rep insl" : :"D"(dst), "c"(count), "d"(port) :"memory", "cc");
}

static inline void
insw(ushort_t port, void* dst, int count)
{
    asm volatile("cld;rep insl" : :"D"(dst), "c"(count), "d"(port) :"memory", "cc");
}

static inline void
insl(ushort_t port, void* dst, int count)
{
    asm volatile(
        "cld;rep insl"
        :
        :"D"(dst), "c"(count), "d"(port)
        :"memory", "cc"
    );
}

static inline void
outsb(ushort_t port, void* src, int count)
{
    asm volatile(
        "cld; rep outsb"
        :
        :"d"(port), "S"(src), "c"(count)
        :"memory", "cc"
    );
}

static inline void
outsw(ushort_t port, void* src, int count)
{
    asm volatile(
        "cld; rep outsw"
        :
        :"d"(port), "S"(src), "c"(count)
        :"memory", "cc"
    );
}

static inline void
outsl(ushort_t port, void* src, int count)
{
    asm volatile(
        "cld; rep outsl"
        :
        :"d"(port), "S"(src), "c"(count)
        :"memory", "cc"
    );
}

static inline void
movsb(void* src, void* dst, int count)
{
    asm volatile(
        "cld; rep movsb"
        :
        :"s"(src), "D"(dst), "c"(count)
        :"memory", "cc"
    );
}

static inline void
movsw(void* src, void* dst, int count)
{
    asm volatile(
        "cld; rep movsw"
        :
        :"s"(src), "D"(dst), "c"(count)
        :"memory", "cc"
    );
}

static inline void
movsl(void* src, void* dst, int count)
{
    asm volatile(
        "cld; rep movsl"
        :
        :"s"(src), "D"(dst), "c"(count)
        :"memory", "cc"
    );
}

static inline void
stosb(void* dst, uchar_t cnt, int count)
{
    asm volatile(
        "cld;rep stosb"
        :
        :"D"(dst), "c"(count), "a"(cnt)
        :"memory", "cc"
    );
}

static inline void
stosw(void* dst, ushort_t cnt, int count)
{
    asm volatile(
        "cld;rep stosw"
        :
        :"D"(dst), "c"(count), "a"(cnt)
        :"memory", "cc"
    );
}

static inline void
stosl(void* dst, uint_t cnt, int count)
{
    asm volatile(
        "cld;rep stosl"
        :
        :"D"(dst), "c"(count), "a"(cnt)
        :"memory", "cc"
    );
}

static inline uint_t
xchg(volatile uint_t *addr, uint_t newval)
{
    uint_t result;
    asm volatile(
        "lock; xchgl %0, %1"
        : "+m"(*addr), "=a"(result)
        : "1"(newval)
        : "cc"
    );
    return result;
}

// 载入中断描述表
static inline void
lidt(struct gatedesc *idt, int size)
{
    volatile ushort_t idtr[3];

    idtr[0] = size - 1;
    idtr[1] = ((uint_t)idt) & 0xFFFF;
    idtr[2] = ((uint_t)idt >> 16) & 0xFFFF;

    asm volatile ("lidt (%0)": :"r"(idtr));
}

// 载入进程段描述符
static inline void
ltr(ushort_t sel)
{
    asm volatile("ltr %0": :"r"(sel));
}

// 载入全局段描述表
static inline void
lgdt(struct segdesc *gdt, int size)
{
    volatile ushort_t gdtr[3];

    gdtr[0] = size -1;
    gdtr[1] = ((uint_t)gdt) & 0xFFFF;
    gdtr[2] = ((uint_t)gdt >> 16) & 0xFFFF;

    asm volatile("lgdt (%0)": :"r"(gdtr));
}

// 载入页目录
static inline void
lcr3(uint_t addr)
{
    asm volatile("movl %0, %%cr3": : "r"(addr));
}

// 读取eflags寄存器内容
static inline uint_t
readeflags()
{
    uint_t eflags;
    asm volatile(
        "pushfl\n\t"
        "popl %0"
        :"=r"(eflags)
    );

    return eflags;
}

// 读取cr2寄存器中的值
// cr2寄存器存储发生缺页中断时的线性地址
static inline uint_t
readcr2()
{
    uint_t cr2;
    asm volatile("movl %%cr2, %0": "=r"(cr2));
    return cr2;
}

// 打开硬件中断
static inline void
sti()
{
    asm volatile ("sti");
}

// 禁用硬件中断
static inline void
cli()
{
    asm volatile ("cli");
}

#endif
