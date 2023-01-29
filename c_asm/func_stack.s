.global _start
.text
_start:
    .macro pushall
        push %rax
        push %rdi
    .endm
    .macro popall
        pop %rdi
        pop %rax
    .endm

    jmp output_end
output:
    mov $1, %rax
    mov $1, %rdi
    mov $buffer, %rsi
    mov $14, %rdx
    syscall
    ret
temp:
    mov $10, %rcx
    mov $5, %r8
    add %r8, %rcx
    syscall
    ret
output_end:
    mov $5, %rdx
    pushall
    call output
    call temp
    popall

    mov $60, %rax
    mov $0, %rdi
    syscall

.data
buffer:
    .ascii "Hello, World!\n"
