.global _start
.data
hi:.string "hi\n"
.text
_start:
    mov $5, %r8

    cmp $5, %r8

    jz if
    call exit
    
if:
    mov $1, %rax
    mov $1, %rdi
    mov $hi, %rsi
    mov $5, %rdx
    syscall
    jmp exit
exit:
    mov $60, %rax
    mov $0, %rdi
    syscall
        
