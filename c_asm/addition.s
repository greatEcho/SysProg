.global addition
.text
# %rax - return register
# %rdi - 1st input register
addition:
    mov $10, %r8
    mov $2, %r9
    add %r9, %r8
    mov %r8, %rax
    ret
