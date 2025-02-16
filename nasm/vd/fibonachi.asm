%include "io64.inc"

section .text
global main
main:
    GET_DEC 8, rax
    push rax
    call factorial
    add rsp, 8
    PRINT_DEC 8, rax
    ret
factorial:
    enter 16, 0
    cmp qword[rbp+16], 0;
    je .end
    cmp qword[rbp+16], 1
    je .end1
    
    push qword[rbp + 16]
    pop qword[rbp - 8]
    dec qword[rbp-8]
    
    push qword[rbp-8]
    call factorial
     
    add rsp, 8
    mov [rbp-16], rax
    
    dec qword[rbp-8]
    push qword[rbp-8]
    call factorial
    add rsp, 8
    
    add rax, [rbp-16]
    leave
    ret
.end:
    mov rax, 0
    leave
    ret
.end1:
    mov rax, 1
    leave
    ret
    
;int factorial(int n){
 ;   if (n == 0){
  ;      return 1;
   ; }
    ;return factorial(n-1)*n;
;} 