.global myarr
.data
rows: .long 0
cols: .long 0
count: .long 0
.text
# %rax - return register
# %rdi - 1st input register - pointer (1st pointer to first element)
# %rsi - 2nd - number of rows
# %rdx - 3rd - number of cols

myarr:
    mov %rsi, rows
    mov %rdx, cols
    mov $0, %r9 # sum

    mov (%rdi), %r8 # put pointer to A[0,0] in r8
my_loop:
    add (%r8), %r9
    add $4, %r8 # go to the next element

    incl count
    cmp %rdx, count
    jne my_loop

    mov %r9, %rax

    ret



