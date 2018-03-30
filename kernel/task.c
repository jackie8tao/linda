#include <task.h>
#include <types.h>
#include <kprintf.h>
#include <mm/kmalloc.h>

#define STACK_SIZE 4096         // 进程初始化时的栈大小

// 系统进程号计数器
static uint_t sys_pid = 0;

struct task* 
task_create()
{
    struct task *task = kmalloc(sizeof(struct task));
    if(!task){
        kprintf("there is no memory to create task block control!\n");
        return NULL;
    }
    task->pid = ++sys_pid;
    task->size = 4096;
    task->parent = NULL;
    task->state = RUNABLE;
    
}

int 
task_sleep()
{

}

int 
task_run()
{

}

