/*
 * 系统调用
 *
 * @author Jackie Tao <taodingfei@gmail.com>
 * @date 2018-03-23 12:48
 */

#ifndef __LINDA_SYSCALL_H
#define __LINDA_SYSCALL_H

#include <types.h>

// 系统调用函数id
#define SYS_FORK    1
#define SYS_EXIT    2
#define SYS_WAIT    3
#define SYS_PIPE    4
#define SYS_READ    5
#define SYS_KILL    6
#define SYS_EXEC    7
#define SYS_FSTAT   8
#define SYS_CHDIR   9
#define SYS_DUP     10
#define SYS_GETPID  11
#define SYS_SBRK    12
#define SYS_SLEEP   13
#define SYS_UPTIME  14
#define SYS_OPEN    15
#define SYS_WRITE   16
#define SYS_MKNOD   17
#define SYS_UNLINK  18
#define SYS_LINK    19
#define SYS_MKDIR   20
#define SYS_CLOSE   21
#define SYS_PRINT   22

void syscall_init();

int argint(int, int*);
int argptr(int, char**, int);
int argstr(int, char**);
int fetch_int(uint_t, int*);
int fetch_str(uint_t, char**);

void syscall();

#endif
