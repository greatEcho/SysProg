.global _main
.data
input:  .long 4 #65535
output: .long 0
txt:    .long 0
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
#    mov output, %rdi
#    mov $60, %rax # exit
#    syscall


flag:
    mov $0, %r10
_start:
	mov $1, %rax	# system call 1 is write
	mov $1, %rdi
    #addl $0x01, output
    mov output, %r8
    mov $16, %r9
print:
    mov %r8, %rsi # temporary store
    shr $60, %rsi
    and $0x0F, %rsi # find if lsb == 1
    
    # print %rsi
    mov %rsi, txt

    # if (txt > 0x09) then A<=txt<=F else 0<=txt<=9
if:
    cmpl $0x09, txt
    jge AF
    jmp numeric # else
AF:
    addl $0x37, txt # to get range from A to F
    jmp endif
numeric:
    addl $0x30, txt # to get range from 0 to 9
endif:
    mov $txt, %rsi
    #mov $0x31, %rsi # output text
    mov $1, %rax # syscall 1 = write
    mov $1, %rdi # file handle 1 = stdout
    # mov $letter, %rsi # output text
    mov $1, %rdx # length of the text
    syscall

    shl $0x04, %r8
    sub $1, %r9
    cmp $0, %r9
    jne print


	mov $60, %rax
	xor %rdi, %rdi
	syscall

