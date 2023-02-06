.global _main
.data
input: .long 65535
output: .long 0
.text
_main:
    mov input, %r8
cycle:
    mov %r8, %rsi # temporary store
    and $0x01, %rsi # find if LSB == 1
    add %rsi, output # count ones
    shr $1, %r8
    cmp $0, %r8
    jne cycle
cycle_end:

    mov output, %rdi
    mov $60, %rax # exit
    syscall
