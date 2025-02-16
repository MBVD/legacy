%include "io64.inc"
section .bss
    arr resq 32
    n resq 1
section .text

global main
main:
    mov rbp, rsp; for correct debugging
    xor rsi, rsi ;остаток
    xor rcx, rcx ;i
    xor rbx, rbx ;j
    GET_DEC 8, [n]
cycle:
    cmp qword[n], 0
    jng print
    mov rax, [n]
    xor rdx, rdx
    mov rdi, 2
    div rdi
    mov rsi, rdx
    mov [arr + 8 * rcx], rsi
    mov [n], rax
    inc rcx
    jmp cycle
print:
    mov rbx, rcx
    sub rbx, 1
cycle2:
    cmp rbx, 0
    jl end
    PRINT_DEC 8, [arr + 8 * rbx]
    dec rbx
    jmp cycle2
end:
    NEWLINE
    ret
    


