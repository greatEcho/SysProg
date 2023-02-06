.global _start
.data
number: .byte 0x30
.text
_start:
    addl $1, number
    
    mov $60, %rax
    mov $0, %rdi
    syscall
