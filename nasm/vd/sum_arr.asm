%include "io64.inc"
section .bss
    arr resq 10
section .data
    n dq 10
section .text
global main
main:
    mov rbp, rsp; for correct debugging
    lea rdi, [arr]
    xor rcx, rcx
for:
    cmp rcx, qword[n]
    jge end
    GET_DEC 8, rax
    stosq
    inc rcx
    jmp for
end:
    lea rax, [arr]
    push qword[n]
    push rax
    call sum_arr
    PRINT_DEC 8, rax
    ret
sum_arr:
    enter 0, 0
    xor rax, rax
    mov rdi, [rbp+16]
    xor rcx, rcx
.for:
    cmp rcx, [rbp+24]
    jge .end_for
    add rax, [rdi + 8 * rcx]
    inc rcx
    jmp .for
.end_for:
    leave
    ret