%include "io64.inc"

section .data
    vector1 dd 1.0, 2.0, 4.0, 8.0, 2.0, 5.0, 6.0, 7.5
    vector2 dd 2.0, 3.0, 4.0, 5.0, 1.0, 2.0, 1.0, 2.0
    print_format db "%lf", 10, 0
    
section .text
global CMAIN
CMAIN:
    mov rbp, rsp; for correct debugging
    mov rdx, 8      ;n
    xor rcx, rcx    ;i
    lea rdi, [vector1]
    lea rsi, [vector2]
    
    call scalar
    
    ;vextractf128 xmm0, ymm0, 0
    cvtss2sd xmm0, xmm0
    lea rdi, [print_format]
    mov rax, 1
    
    ALIGN_STACK
    call printf
    UNALIGN_STACK
    
    xor rax, rax
    ret
    
scalar:
    enter 8, 0
    
    xorps xmm4, xmm4
    jmp .cycle
    
.cycle:
    cmp rcx, rdx
    jge .end
    
    vmovupd ymm1, [rdi + 4 * rcx]
    vmovupd ymm2, [rsi + 4 * rcx]
    
    vmulps ymm0, ymm1, ymm2
    vhaddps ymm0, ymm0, ymm0
    vhaddps ymm0, ymm0, ymm0
    
    vextractf128 xmm3, ymm0, 1
    vaddps xmm3, xmm0
    vpsrldq xmm3, xmm3, 12
    
    vaddps ymm4, ymm4, ymm3
    add rcx, 8
    jmp .cycle
    
.end:
    vmovups ymm0, ymm4
    
    leave
    ret

  