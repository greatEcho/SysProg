.globl maxofthree
.text
# %rax - return register
# %rdi - 1st input register
# %rsi - 2nd
# %rdx - 3rd
maxofthree:
    # jmp maxofthree_end
    mov %rdi, %rax
    cmp %rsi, %rax
    cmovl %rsi, %rax
    cmp %rdx, %rax
    cmovl %rdx, %rax
maxofthree_end:

addition:
    # mov %rdi, %rax
    add $1, %rax
    ret
    
