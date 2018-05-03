#include <sysapi.h>
#include <kprintf.h>
#include <syscall.h>
#include <drivers/console.h>

// 简单的打印字符串，用于测试
int
print()
{
    char *msg;
    if(argstr(0, &msg)<0){
        kprintf("syscall[print]: arg error!\n");
        return -1;
    }
    // console_clear();
    return kprintf(msg);
}
