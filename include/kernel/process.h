/*
 * 进程
 *
 * @author Jackie Tao <jackie8tao@gmail.com>
 */

#ifndef __LINDA_PROCESS_H
#define __LINDA_PROCESS_H

#include <types.h>
#include <mach.h>
#include <spinlock.h>

// 最大64个进程
#define NPROC 64

enum pstate{UNUSED, EMBRYO, RUNABLE, RUNNING, SLEEPING, ZOMBIE};

// %eax, %ecx, %edx are saved by caller.
struct context {
    uint_t edi;
    uint_t esi;
    uint_t ebx;
    uint_t ebp;
    uint_t eip;
};

struct process {
    uint_t size;                    // 进程大小
    uint_t pid;
    struct context *context;
    void *kstack;
    pde_t *pgdir;
    enum pstate state;
    struct process *parent;
    struct trapframe *tf;
    int killed;                     // 0表示未kill，其它则未killed
    char name[64];
};

struct process_table {
    struct spinlock lock;
    struct process process[NPROC];  // 需要注意的是这里已经预设好了所有进程
};

extern struct process_table ptable;

void process_init();

struct process* alloc_process();

void uinitproc_setup();

// int fork_process();
//
// void exit_process();
//
// int wait_process();
//
// void sleep_process();
//
// void wakeup_process();
//
// int kill_process(uint_t pid);

void switch_context(struct context **old, struct context *new);

void dump_process();

#endif
