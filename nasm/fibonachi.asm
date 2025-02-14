%include "io64.inc"

section .text
global CMAIN
CMAIN:
    mov rbp, rsp; for correct debugging
    GET_DEC 8, rax
    push rax
    call Fib
    add rsp, 8
    
    PRINT_DEC 8, rax  
    ret
Fib:
    enter 16, 0
    cmp qword[rbp+16], 0
    je .end_zero
    
    cmp qword[rbp+16], 1
    je .end_one
    
    ; n -> rbp + 16 -> rbp-8
    push qword[rbp + 16]; n-1
    pop qword[rbp -8]
    dec qword[rbp-8]
    
    push qword[rbp-8]
    call Fib
    add rsp, 8
    mov qword[rbp-16], rax
    
    
    dec qword[rbp-8]; n-2
    push qword[rbp-8]
    call Fib
    add rsp, 8

    
   
    add rax, [rbp-16]
    jmp .end
.end_zero:
    mov rax, 0
    jmp .end
.end_one:
    mov rax, 1
    
.end:
    leave
    ret
            
    
 ; n chislo fib = n-1 - n-2
;int Fib(int n)
 ;   if (n == 0){
  ;      return 0;
   ;  }else if(n == 1)
    ;    return 1;
     ;return Fib(n-1) + Fib(n-2)