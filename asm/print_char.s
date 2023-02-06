.global _start
.data
letter: .long 2
.text
_start:
    movb $0x31, %al # output text
    addl $0x30, letter
    mov $letter, %rsi # rsi will have a pointer
    #mov $0x31, %rsi

    mov $1, %rax # syscall 1 = write
    mov $1, %rdi # file handle 1 = stdout
    mov $1, %rdx # length of the text
    syscall

    mov $60, %rax
    mov $0, %rdi
    syscall


message:
    .ascii "Hello\n"
msg:
    .byte 0x31
