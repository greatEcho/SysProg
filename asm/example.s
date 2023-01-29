.text
.globl _start

_start:
    movl $7, -16(%rsp)
    mov %rdi, %rax
    imul 8(%rsp), %rax
    add %rsi, %rax
    ret
