#include <process.h>
#include <spinlock.h>
#include <types.h>
#include <mm/kmalloc.h>
#include <scheduler.h>
#include <mm/mmap.h>
#include <string.h>
#include <kprintf.h>
#include <mach.h>
#include <mm/vmmgr.h>
#include <generic.h>

struct process_table ptable;

// 内核初始化进程
static struct process *initproc;

extern void forkret();
extern void trapret();

// 全局进程id号记录
static int nextpid = 1;

void
process_init()
{
    initlock(&ptable.lock, "ptable");
}

static struct process*
current_process()
{
    struct cpu *curcpu = current_cpu();

    return curcpu->curprocess;
}

// 设置用户空间init进程
void
uinitproc_setup()
{
    struct process *proc;
    extern char _binary_processA_start[], _binary_processA_size[];

    proc = alloc_process();
    initproc = proc;
    if(!(proc->pgdir=umap_init())){
        panic("uinitproc_setup: cannot setup user pgdir!\n");
    }
    uvm_init(proc->pgdir, _binary_processA_start, _binary_processA_size);
    proc->size = PGSIZE; // 这里限制了init进程的大小
    memset(proc->tf, 0, sizeof(struct trapframe));

    proc->tf->cs = (SEG_UCODE << 3) | DPL_USER;
    proc->tf->ds = (SEG_UDATA << 3) | DPL_USER;
    proc->tf->es = proc->tf->ds;
    proc->tf->ss = proc->tf->ds;
    proc->tf->eflags = FL_IF;
    proc->tf->esp = PGSIZE; // 用户栈从末尾开始
    proc->tf->eip = 0;

    strncpy(proc->name, "initproc", sizeof(proc->name));

    acquire(&ptable.lock);
    proc->state = RUNABLE;
    release(&ptable.lock);
}

// 从进程列表中获取一个UNUSED进程并初始化
struct process*
alloc_process()
{
    acquire(&ptable.lock);

    struct process *p; char *sp;
    for(p=ptable.process; p<&ptable.process[NPROC]; p++){
        if(p->state == UNUSED){
            p->state = EMBRYO;
            p->pid = nextpid++;
            release(&ptable.lock);

            if(!(p->kstack=kvm_alloc_page())){
                // 未成功分配进程内核栈
                p->state = UNUSED;
                return NULL;
            }

            sp = p->kstack + KSTACKSIZE;
            sp -= sizeof(struct trapframe);
            p->tf = (struct trapframe*)sp;

            sp -= 4;
            *((uint_t*)sp) = (uint_t)trapret;

            sp -= sizeof(struct context);
            p->context = (struct context*)sp;

            memset(p->context, 0, sizeof(struct context));
            p->context->eip = (uint_t)forkret;

            return p;
        }
    }

    release(&ptable.lock);
    return NULL;
}

// int
// fork_process()
// {
//     struct process *nextproc, *curproc;
//     curproc = current_process();
//
//     if(!(nextproc = alloc_process())){
//         // 未获取进程
//         return -1;
//     }
//
//     if(!(nextproc->pgdir=copy_uvm(curproc->pgdir, curproc->size))){
//         kmfree(nextproc->kstack);
//
//         // 恢复进程初始状态
//         nextproc->kstack = 0;
//         nextproc->state = UNUSED;
//
//         return -1;
//     }
//
//     nextproc->size = curproc->size;
//     nextproc->parent = curproc;
//     *(nextproc->tf) = *(curproc->tf);
//     nextproc->tf->eax = 0;
//
//     // @TODO 其它信息和资源复制
//
//     acquire(&ptable.lock);
//     nextproc->state = RUNABLE;
//     release(&ptable.lock);
//
//     return nextproc->pid;
// }
//
// void
// exit_process()
// {
//     struct process *curproc = current_process(), *proc;
//     if(curproc == initproc){
//         panic("init should`t exist!\n");
//     }
//
//     // @TODO 关闭所有文件
//     wakeup_process(curproc->parent);
//     for(proc=ptable.process; proc<&ptable.process[NPROC]; proc++){
//         if(proc->parent==curproc){
//             proc->parent = initproc;
//             if(proc->state==ZOMBIE){
//                 wakeup_process(initproc);
//             }
//         }
//     }
//
//     curproc->state = ZOMBIE;
//     goto_scheduler();
//     // 该函数后面应该永远不执行
//     panic("zombie process exit!\n");
// }
//
// int
// wait_process()
// {
//     struct process *proc, *curproc;
//     int havekids, pid;
//
//     curproc = current_process();
//     acquire(&ptable.lock);
//     for(;;){
//         havekids = 0;
//         for(proc=ptable.process; proc<&ptable.process[NPROC]; proc++){
//             if(proc->parent != curproc){
//                 continue;
//             }
//
//             havekids = 1;
//             if(proc->state == ZOMBIE){
//                 pid = proc->pid;
//                 kmfree(proc->kstack);
//                 proc->kstack = 0;
//                 // @TODO free pgdir
//                 proc->pid = 0;
//                 proc->parent = 0;
//                 proc->name[0] = 0;
//                 proc->state = UNUSED;
//                 release(&ptable.lock);
//                 return pid;
//             }
//         }
//
//         if(!havekids || curproc->killed){
//             release(&ptable.lock);
//             return -1;
//         }
//
//         sleep_process(curproc, &ptable.lock);
//     }
//
// }
//
// void
// sleep_process()
// {
//
// }
//
// void
// wakeup_process()
// {
//
// }
//
// int
// kill_process(uint_t pid)
// {
//     struct process *proc;
//
//     acquire(&ptable.lock);
//     for(proc=ptable.process; proc<&ptable.process[NPROC]; proc++){
//         if(proc->pid == pid){
//             proc->killed = 1;
//             if(proc->state == SLEEPING){
//                 proc->state = RUNABLE;
//             }
//             release(&ptable.lock);
//             return 0;
//         }
//     }
//     release(&ptable.lock);
//     return -1;
// }

void
forkret()
{
    release(&ptable.lock);
}

// 调试用，打印出所有进程
void
dump_process()
{
    char *state[] = {
        [UNUSED] = "unused",
        [EMBRYO] = "embryo",
        [RUNABLE] = "runable",
        [RUNNING] = "running",
        [SLEEPING] = "sleeping",
        [ZOMBIE] = "zombie"
    };
    struct process *proc;
    for(proc=ptable.process; proc<&ptable.process[NPROC]; proc++){
        if(proc->pid != 0){
            kprintf("proc: %d, %s\n", proc->pid, state[proc->state]);
        }
    }
}
