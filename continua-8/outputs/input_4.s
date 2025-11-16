.data
print_fmt: .string "%ld \n"
.text
.globl imprimir
imprimir:
 pushq %rbp
 movq %rsp, %rbp
 subq $8, %rsp
 movq %rdi,-8(%rbp)
 movq -8(%rbp), %rax
 movq %rax, %rsi
 leaq print_fmt(%rip), %rdi
 movl $0, %eax
 call printf@PLT
.end_imprimir:
leave
ret
.globl main
main:
 pushq %rbp
 movq %rsp, %rbp
 subq $16, %rsp
 movq $3, %rax
 mov %rax, %rdi
call imprimir
 movq $4, %rax
 mov %rax, %rdi
call imprimir
 movq $0, %rax
 jmp .end_main
.end_main:
leave
ret
.section .note.GNU-stack,"",@progbits
