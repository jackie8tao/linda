#include <syscall.h>
#include <process.h>
#include <scheduler.h>
#include <kprintf.h>
#include <trap.h>
#include <types.h>
#include <sysapi.h>

static int (*syscalls[])() = {
    [SYS_PRINT] = print
};

// 获取cpu当前正在执行的进程
static inline struct process*
curproc()
{
    struct cpu *curcpu = current_cpu();

    return curcpu->curprocess;
}

void
syscall_handler(struct trapframe *tf)
{
    syscall();
}

void
syscall_init()
{
    register_trap_handler(IRQ_SYSCALL, syscall_handler);
}

// 获取addr处的int参数值，
// 成功返回0，失败返回-1
int
fetch_int(uint_t addr, int *res)
{
    struct process *proc = curproc();
    if(addr>=proc->size || (addr + 4)>proc->size){
        return -1;
    }

    *res = *(int*)addr;

    return 0;
}

// 获取addr处字符串，
// 成功返回字符串长度，失败返回-1
int
fetch_str(uint_t addr, char **res)
{
    struct process *proc = curproc();
    if(addr>=proc->size){
        return -1;
    }

    *res = (char*)addr;

    char *s, *ep;
    ep = (char*)proc->size;
    for(s=*res; s<ep; s++){
        if(*s==0){
            return s - *res;
        }
    }

    return -1;
}

// 第n个int参数
int
argint(int n, int *res)
{
    struct process *proc = curproc();

    return fetch_int((proc->tf->esp)+n*4+4, res);
}

// 第n个指针参数
int
argptr(int n, char **res, int size)
{
    int addr;
    struct process *proc = curproc();
    if(argint(n, &addr)<0){
        return -1;
    }

    if(size<0 || (uint_t)addr>=proc->size || (uint_t)addr+size>proc->size){
        return -1;
    }

    *res = (char*)addr;

    return 0;
}

// 第n个字符串参数
int
argstr(int n, char **res)
{
    int addr;
    if(argint(n, &addr)<0){
        return -1;
    }

    return fetch_str(addr, res);
}

void
syscall()
{
    struct cpu *curcpu = current_cpu();
    struct process *curproc = curcpu->curprocess;

    int idx = curproc->tf->eax;
    if(idx > 0 && syscalls[idx]){
        curproc->tf->eax = syscalls[idx]();
    }else{
        kprintf("%d %s: unknown system call %d!\n",
            curproc->pid, curproc->name, idx);

        curproc->tf->eax = -1;
    }
}
