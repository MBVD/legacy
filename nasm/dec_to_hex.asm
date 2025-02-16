%include "io64.inc"
section .bss
    arr resq 32
section .text
global CMAIN
CMAIN:
    mov rbp, rsp; for correct debugging
    GET_DEC 8, rax
    xor rcx, rcx
    push rax ;[rbp + 16]
    call decToHex
    add rsp, 8
    PRINT_DEC 8, rax
decToHex:
    enter 8, 0 ; [rbp - 8]
    cmp qword[rbp + 16], 0
    je .end
    mov rax, [rbp + 16]
    xor rdx, rdx
    mov rdi, 16
    div rdi
    mov [arr + 8 * rcx], rdx ;rem rcx is increasing
    mov [rbp + 16], rax ;dec
    inc rcx
    push qword[rbp + 16]
    pop qword[rbp - 8]
    push qword[rbp - 8]
    call decToHex
    add rsp, 8
.case_A:
    cmp qword[arr + 8 * rcx], 10
    jne .case_B
    PRINT_CHAR "A"
    jmp .end
.case_B:
    cmp qword[arr + 8 * rcx], 11
    jne .case_C
    PRINT_CHAR "B"
    jmp .end
.case_C:
    cmp qword[arr + 8 * rcx], 12
    jne .case_D
    PRINT_CHAR "C"
    jmp .end
.case_D:
    cmp qword[arr + 8 * rcx], 13
    jne .case_E
    PRINT_CHAR "D"
    jmp .end
.case_E:
    cmp qword[arr + 8 * rcx], 14
    jne .case_F
    PRINT_CHAR "E"
    jmp .end
.case_F:
    cmp qword[arr + 8 * rcx], 15
    jne .base
    PRINT_CHAR "F"
    jmp .end
.base:
    PRINT_DEC 1, [arr + 8 * rcx]
.end: 
    dec rcx
    leave
    ret