# 测试专用
.global start
start:
    pushl $msg
    pushl $0
    movl $22, %eax
    int $64
    addl $8, %esp
spin:
    jmp spin
msg:
    .string "A\n\0"
