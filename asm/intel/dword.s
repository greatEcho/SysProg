section .data
    input   DD 7
    output  DD 5

section .text
    global _start

_start:

    mov rdx, [input] ; store value in register
cycle:
    mov rax, rdx
    and rax, 0x01 ; find if LSB == 1
    add [output], rax ; count ones
    shr rdx, 1
    cmp rdx, 0
    jge cycle
cycle_end:
    ;add [output], dword 10
    mov rdi, [output]


    ; print
;    add [output], dword 0x31
;    mov rsi, [output]
;    mov rax, 1
;    mov rdi, 1
;    mov rdx, 1
;    syscall





end:
    mov rax, 60
;    mov rdi, 0
;    mov rsi, 5
;    mov rdi, 3
;    add rdi, rsi
    syscall

