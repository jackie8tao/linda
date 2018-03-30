#include <types.h>
#include <spinlock.h>
#include <generic.h>
#include <kprintf.h>

void 
initlock(struct spinlock *lock, char *name)
{
    lock->name = name;
    lock->locked = 0;
}

int 
holding(struct spinlock *lock)
{
    return lock->locked;
}

void 
acquire(struct spinlock *lock)
{
    cli();
    if(holding(lock)){
        kprintf("%s is locked!", lock->name);
        return;
    }

    while(xchg(&lock->locked, 1) !=0);
    sti();
}

void 
release(struct spinlock *lock)
{
    cli();
    if(!holding(lock)){
        kprintf("%s lock is not locked!", lock->name);
        return;
    }

    asm volatile("movl $0, %0" :"+m"(lock->locked):);
    sti();
}