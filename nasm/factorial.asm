%include "io64.inc"

section .text
global CMAIN
CMAIN:
    GET_DEC 8, rax
    push rax
    call factorial
    add rsp, 8
    
    PRINT_DEC 8, rax
    ret
factorial:
    enter 0, 0
    cmp qword[rbp + 16], 0
    jz .end
    
    push qword[rbp + 16]    
    pop qword[rbp - 8] ;local perem
    dec qword[rbp - 8] 
    
    push qword[rbp-8]
    call factorial
    add rsp,8 
    
    imul rax, [rbp + 16]
    
    leave
    ret
    
.end:
    mov rax, 1    
    leave
    ret