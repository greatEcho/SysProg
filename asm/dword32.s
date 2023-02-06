.global _main
.data
number: .long 31
.text
_main:
    movl number, %edx # return code
    mov $0, %edi
cycle:
    mov %edx, %esi # temporary store
    and $0x01, %esi # find if LSB == 1
    add %esi, %edi # count ones
    shr $1, %edx
    cmp $0, %edx
    jne cycle
cycle_end:
    mov $60, %eax # exit
    syscall
