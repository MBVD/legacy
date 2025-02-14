%include "io64.inc"

section .data  
    x dq 1.2
    y dq  2
section .text
global CMAIN
CMAIN:
    enter 0, 0
    
    movsd xmm0, [x]
    movq rax, xmm0
    movq xmm1, rax
    
    xor rax, rax
    leave
    ret
    
    ;sub rsp, 8
    ;movsd xmm0, [x]
    ;movsd [rsp], xmm0
    ;add rsp, 8