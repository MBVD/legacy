%include "io64.inc"
section .data
    number dq 1025
    result dq 0
    divv dq 0
section .text

global main
main:
    mov rbp, rsp; for correct debugging
    mov rbx, 0
    mov rcx, 8 ;count for div
    mov r8, 0
calc:
    cmp rcx, 0
    je over
    mov rax, 0
    mov rax, [number]
    shr rax, 1
    mov [number], rax
    jc CF_1
    jnc CF_0
CF_1:
    test rbx, rbx
    je inc_rbx
    jne shift
shift:
    mov rax, rbx
    shr rax, 1
    jc shift1
    jnc shift2
shift1:
    shl rbx, 1
    or rbx, 1
    dec rcx
    jmp calc
shift2:
    or rbx, 1
    dec rcx
    jmp calc
inc_rbx:
    mov rbx, 1
    test r8, r8
    jne counting
    dec rcx
    jmp calc
counting:
    test r8, r8
    je end_counting
    shl rbx, 1
    dec r8
    jmp counting
end_counting:
    jmp calc
CF_0:
    test rbx, rbx
    je inc_r8
    dec rcx
    jmp calc
inc_r8:
    inc r8
    dec rcx
    jmp calc
over:
    mov [divv], rbx
    PRINT_DEC 8, [divv]