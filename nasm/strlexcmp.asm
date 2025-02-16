%include "io64.inc"
section .data
    string1 db "owl"
    string2 db "Mycar"
section .text

global main
main:
    xor rbx, rbx ;i
    xor rax, rax
while:
    cmp byte[string1 +8*rbx], `\0`
    je zerores
    cmp byte[string2 +8*rbx], `\0`
    je zerores
    mov rcx, [string2 +8*rbx]
    cmp [string1 +8*rbx], rcx
    je inc_rbx
    mov rax, [string1 +8*rbx]
    sub rax, [string2 +8*rbx]
    jmp result
inc_rbx:
    inc rbx
    jmp while
zerores:
    mov rax, 0
result:
    cmp rax, 0
    jl str1less
    jg str1great
    PRINT_STRING "Equal"
    ret
str1less:
    PRINT_STRING "Str1<Str2"
    ret
str1great:
    PRINT_STRING "Str1>Str2"
    ret


