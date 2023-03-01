.global myarr
.data
rows: .long 0
cols: .long 0
.text
# %rax - return register
# %rdi - 1st input register - pointer (1st pointer to first element)
# %rsi - 2nd - number of rows
# %rdx - 3rd - number of cols

myarr:
    # save values
    mov %rsi, rows
    mov %rdx, cols
    mov $0, %eax # sum

row:
    mov (%rdi), %r8 # put pointer to A[0,0] in r8
    col:
        addl (%r8), %eax # summation
        add $4, %r8 # shift to the next elemnt
        dec %rdx # count cols
        cmp $0, %rdx 
        jne col 
    add $8, %rdi # go to the next row
    mov cols, %rdx
    dec %rsi # count rows
    cmp $0, %rsi
    jne row
    # division
    movl cols, %ebx
    cdq # EDX:EAX eax expands to 64bit
    idivl %ebx

    ret


