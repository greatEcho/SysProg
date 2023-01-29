.global _start
.text

_start:
    push $2
    push $4
    push $8

    mov $60, %rax # exit
    mov $10, %r8
    mov $2, %r9
    add %r9, %r8
    push %r8
    pop %rdi
    syscall

