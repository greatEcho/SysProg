; rax - return register
; rcx - 1st input argument
; rdx - 2nd input argument
; r8 - 3rd input argument
.data
cols DD ?
rows DD ?

.code
masm_arr proc ; start of function

	mov rows, edx ; initialize rows
	mov rsi, r8
	mov cols, esi ; initialize cols
	mov rax, 0 ; sum = 0
col:
	mov r9, [rcx] ; get pointer to A[0][0]
	row:
		add rax, [r9]
		add r9, 4
		dec esi ; count cols
		cmp esi, 0
		jne row
	add rcx, 8 ; go to the next row
	mov esi, cols
	dec rdx
	cmp rdx, 0
	jne col

	; division
	mov ebx, cols
	idiv ebx

ret

masm_arr endp ; end of function
end