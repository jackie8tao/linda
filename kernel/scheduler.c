#include <scheduler.h>
#include <spinlock.h>
#include <mach.h>
#include <types.h>
#include <kprintf.h>
#include <mm/mmap.h>
#include <process.h>
#include <generic.h>
#include <drivers/console.h>

static struct cpu curcpu;

// 进程调度器
void
scheduler()
{
    console_clear();
    struct process *proc;
    for(;;){
        acquire(&ptable.lock);
        for(proc=ptable.process; proc<&ptable.process[NPROC]; proc++){
            if(proc->state != RUNABLE){
                continue;
            }
            curcpu.curprocess = proc;
            proc->state = RUNNING;

            switch_to_uvm(proc);
            // asm volatile("xchg %bx, %bx");
            switch_context(&(curcpu.scheduler), proc->context);
            switch_to_kvm();
        }
        release(&ptable.lock);
    }
}

// 进入调度器代码
void
goto_scheduler()
{
    int intena;
    struct process *proc = curcpu.curprocess;

    if(!holding(&ptable.lock)){
        panic("goto_scheduler: ptable is locked\n");
    }

    if(curcpu.ncli != 1){
        panic("goto_scheduler: sched locks, ncli:%d\n", curcpu.ncli);
    }

    if(readeflags() & FL_IF){
        panic("goto_scheduler: sched interruptible\n");
    }

    if(proc->state == RUNNING){
        panic("goto_scheduler: process is running\n");
    }

    intena = curcpu.intena;
    // asm volatile("xchg %bx, %bx");
    switch_context(&(proc->context), curcpu.scheduler);
    curcpu.intena = intena;
}

// 释放进程当前占有的cpu
void
yield_cpu()
{
    acquire(&ptable.lock);
    curcpu.curprocess->state = RUNABLE;
    goto_scheduler();
    release(&ptable.lock);
}

// 获取当前cpu
struct cpu*
current_cpu()
{
    return &curcpu;
}
