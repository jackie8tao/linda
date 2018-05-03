# 进程上下文切换

# void switch(struct context **old, struct context *new);
.code32
.global switch_context
switch_context:
    movl 4(%esp), %eax
    movl 8(%esp), %edx

    pushl %ebp;
    pushl %ebx;
    pushl %esi;
    pushl %edi;

    # 更新旧栈指针
    movl %esp, (%eax)
    movl %edx, %esp

    popl %edi
    popl %esi
    popl %ebx
    popl %ebp
    ret
