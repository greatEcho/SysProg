.text
    .global f
    
    f:
        pushq %rdi
        pushq %rcx
        pushq %rbx

        movl $4, %ecx
        movl $0, %ebx

        cycle:
            movl $0, %eax
            addl (%rdi), %eax
            mull (%rdi)
            addq $4, %rdi
            addl %eax, %ebx
            loop cycle

        movl $0, %eax
        movl %ebx, %eax

        popq %rbx
        popq %rcx
        popq %rdi

        ret



