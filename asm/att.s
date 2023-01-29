.global _start
.text
_start:
    push %eax
    movl $25, %eax
    syscall
    ret
