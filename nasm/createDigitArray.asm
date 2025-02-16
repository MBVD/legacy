%include "io64.inc"
section .bss
    arr resq 32
section .text
global CMAIN
CMAIN:
    mov rbp, rsp; for correct debugging
    GET_DEC 8, rax
    push rax
    call createDigitArray
    add rsp, 8
    PRINT_DEC 8, rax
    xor rax, rax
    ret
createDigitArray:
    enter 8, 0
    xor rcx, rcx
    mov rbx, [rbp + 16]
.read_arr:
    cmp rcx, rbx
    jnl .end_read
    GET_DEC 8, rax
    mov [arr + 8 * rcx], rax
    inc rcx
    jmp .read_arr
.end_read:
    lea rax, [arr]
    pop rbx
    leave
    ret