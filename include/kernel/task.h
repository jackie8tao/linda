/* 
 * 进程
 *
 * @author Jackie Tao <taodingfei@gmail.com>
 * @date 2018-03-23 15:32
 */

#ifndef __LINDA_TASK_H
#define __LINDA_TASK_H

#include <types.h>
#include <mm/mmap.h>

enum task_state {
    RUNABLE, RUNNING, SLEEPING
}

struct task_context {
    uint_t eip;
    uint_t esp;
    uint_t ebp;
    uint_t eax;
    uint_t ebx;
    uint_t ecx;
    uint_t edx;
    uint_t edi;
    uint_t esi;
};

struct task {
    uint_t size;
    uint_t pid;
    pde_t pgdir[NPDENTRIES];
    struct task *parent;
    enum task_state state;
};

// 创建用户态进程
struct task* task_create();

// 用户态进程睡眠
int task_sleep();

// 执行用户态进程
int task_run();

// 用户态进程切换
void task_switch();

#endif