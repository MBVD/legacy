%include "io64.inc"

section .data  
    x dq 1.2
    y dq  2
    
section .text
global CMAIN
CMAIN:
    enter 0, 0
    
    mov rax, [y]
    cvtsi2sd xmm0, rax
    
    movsd xmm1, [x]
    mulsd xmm0, xmm1
    
    xor rax, rax
    leave
    ret