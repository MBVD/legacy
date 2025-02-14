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
    enter 8, 0
    cmp qword[rbp+16], 0
    je .end
    push qword[rbp + 16]
    pop qword[rbp - 8]
    dec qword[rbp-8]
    
    push qword[rbp-8]
    call factorial
     
    add rsp, 8
    
    imul rax, [rbp+16]
    leave
    ret
.end:
    mov rax, 1
    leave
    ret
    
;int factorial(int n){
 ;   if (n == 0){
  ;      return 1;
   ; }
    ;return factorial(n-1)*n;
;} 