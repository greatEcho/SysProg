.global sarr
.text
# %rax - return register
# %rdi - 1st input register
sarr:
  mov (%rdi), %rax  
  ret

