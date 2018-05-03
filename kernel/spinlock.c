#include <types.h>
#include <spinlock.h>
#include <generic.h>
#include <kprintf.h>
#include <scheduler.h>
#include <mach.h>

// 初始化指定锁
void
initlock(struct spinlock *lock, char *name)
{
    lock->name = name;
    lock->locked = 0;
}

// 判断锁是否已被拥有
int
holding(struct spinlock *lock)
{
    return lock->locked;
}

// 获取锁
void
acquire(struct spinlock *lock)
{
    pushcli();
    if(holding(lock)){
        // kprintf("acquire lock: %d\n", lock->locked);
        panic("acquire: %s is locked!\n", lock->name);
    }

    while(xchg(&lock->locked, 1) !=0);
}

// 释放锁
void
release(struct spinlock *lock)
{
    if(!holding(lock)){
        panic("release: %s lock is not locked!\n", lock->name);
    }

    asm volatile("movl $0, %0" :"+m"(lock->locked):);
    popcli();
}


void
pushcli()
{
    uint_t eflags = readeflags();
    struct cpu *curcpu = current_cpu();

    cli();
    if(curcpu->ncli == 0){
        curcpu->intena = eflags & FL_IF;
    }
    curcpu->ncli += 1;
}

void
popcli()
{
    uint_t eflags = readeflags();
    struct cpu *curcpu = current_cpu();
    if(eflags & FL_IF){
        panic("popcli: interrupt has enable!\n");
    }

    if(--curcpu->ncli < 0){
        panic("popcli: no cli!\n");
    }

    if(curcpu->ncli==0 && curcpu->intena){
        sti();
    }
}
