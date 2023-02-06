section .data
    input DD 65
    output DD 0

section .text
    global _start

_start:

cycle:
    mov rax, input
    and rax, 0x04
    mov rdi, input
    mov rdi, 3

;    ; print
;    mov rax, 1
;    mov rdi, 1
;    mov rsi, input
;    add [rsi], dword 2
;    mov rdx, 14
;    syscall





end:
    mov rax, 60
;    mov rdi, 0
;    mov rsi, 5
;    mov rdi, 3
;    add rdi, rsi
    syscall

