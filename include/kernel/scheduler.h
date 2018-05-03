/*
 * 调度器
 *
 * @author Jackie Tao <jackie8tao@gmail.com>
 */

#ifndef __LINDA_SCHEDULER_H
#define __LINDA_SCHEDULER_H

#include <process.h>
#include <mach.h>

struct cpu {
    struct context *scheduler;
    struct process *curprocess;
    struct taskstate tss;
    struct segdesc gdt[NSEGS];
    int ncli;
    int intena; // 记录在使用pushcli前，中断是否打开过
};

void scheduler();

void goto_scheduler();

struct cpu* current_cpu();

#endif
