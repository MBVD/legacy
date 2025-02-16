%include "io64.inc"
section .text
global CMAIN
CMAIN:

    mov rbp, rsp ; for correct debugging

    GET_DEC 8, rax
    push rax
    call digsum
    add rsp, 8
    PRINT_DEC 8, rax
    xor rax, rax
    ret

digsum:
    enter 0, 0
    xor rsi, rsi
    mov rcx, qword[rsp + 16]
.while:
    test rcx, rcx
    jz .end
    mov rax, rcx
    mov rdx, 0
    mov rdi, 10
    div rdi
    
    add rsi, rdx
    mov rcx, rax
    jmp .while
.end:
    mov rax, rsi
    leave
    ret