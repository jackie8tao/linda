# 测试专用
.global start
start:
    pushl $msg
    pushl $0
    movl $22, %eax
    int $64
    addl $8, %esp
    // xchg %bx, %bx
    jmp start
msg:
    .string "B\0"
