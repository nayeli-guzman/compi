.data
print_fmt: .string "%ld \n"
.text
.globl main
main:
 pushq %rbp
 movq %rsp, %rbp
 subq $8, %rsp
 movq $10, %rax
 movq %rax, -8(%rbp)
 movq 0(%rbp), %rax
 movq %rax, 0(%rbp)
.end_main:
leave
ret
.section .note.GNU-stack,"",@progbits
