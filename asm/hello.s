.global _start
.data
number: .long 0x30
.text
_start:
	mov $1, %rax	# system call 1 is write
	mov $1, %rdi
    addl $0x1E, number
	mov $number, %rsi
	mov $13, %rdx
	syscall

	mov $60, %rax
	xor %rdi, %rdi
	syscall
message:
	.ascii "Hello, World!\n" 
