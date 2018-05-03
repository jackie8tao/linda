/*
 * 用户空间初始化进程
 *
 * @author Jackie Tao <jackie8tao@gmail.com>
 */

#include <syscall.h>
#include <trap.h>

// 系统调用，打印出字符串
int
print(char *msg)
{
    asm volatile(
        "movl $22, %%eax"
        "int $64"
        "ret"
    );
}

int
main(int argc, char **argv)
{
    print("this is first user process!\n");

    for(;;);

    return 0;
}
