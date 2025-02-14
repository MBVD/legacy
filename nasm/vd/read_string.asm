%include "io64.inc"
section .bss
    arr resb 256
    mas resq 256
section .text
global main
main:
    mov rax, `\n`
    lea rbx, [arr]
    push rax
    push rbx
    call get_string
    add rsp, 16
    PRINT_STRING arr
    
    GET_UDEC 8, rax
    lea rbx, [mas]
    push rbx
    push rax
    call get_array
    add rsp, 16
    ret
    
get_string:
    enter 0, 0
    push rdi
    push rcx
    
    mov rdi, [rbp+16]
    xor rcx, rcx
.cycle:
    GET_CHAR al
    cmp al, [rbp + 24]
    je .end_cycle
    inc rcx
    stosb
    jmp .cycle
.end_cycle:
    mov byte[rdi], `\0`
    mov rax, rcx
    
    pop rcx
    pop rdi
    leave
    ret
get_array:
    enter 0,0
    push rcx, 
    push rdi
    mov rdi, [rbp + 16]
    xor rcx, rcx
.cycle:
    cmp rcx, [rbp+ 24]
    jnl .end_cycle
    
    GET_DEC 8, rax
    mov [rdi + 8 * rcx], rax
    inc rcx
    jmp .cycle
    
.end_cycle:
    pop rdi
    pop rcx
    leave 
    ret