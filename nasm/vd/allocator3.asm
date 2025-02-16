%include "io64.inc"
section .bss
    brk resq 0            ; Текущий адрес вершины кучи
section .data
    size dq 0 
    brk_size dq 0
head:
    dq 0; used
    dq 0; adressaa
    dq 0; size
    dq 0; next
tail:
    dq 0
    dq 0 
    dq 0
    dq 0
section .text
global main

sbrk:; сколько памяти мы в принципе выделяем нужно увеличивать, если у нас уже все остальное +- забито возвращаем в rax свободный адресс памяти
    enter 0, 0
    mov rdx, [rbp + 16]
    mov rcx, [brk_size]
    inc rcx
    lea rax, [rbk]
    add rax, [brk_size]
    add rcx, rdx
    mov [brk_size], rcx
    leave
    ret
get_free_block: ;get_free_block(size)
    enter 0, 0
    mov rcx, [rbp + 16]; размер
    mov r8, [head]; is_free || head
    mov r9, [head + 8]; obj index
    mov r10, [head + 16]; obj size
    mov r11, [head + 24]; next
.while:
    cmp r9, qword[brk_size]; если адресс больше чем у нас в принципе выделенно под программу 
    jge .end_while
    cmp r10, rcx
    jl .end_if
    cmp r8, 1
    jne .end_if
    lea rax, [brk]; аддрес массив
    add rax, r9; указатель на массив где свободно
    leave
    ret
.end_if:
    mov r8, r11
    mov r9, [r8 + 8]
    mov r10, [r8 + 16]
    mov r11, [r8 + 24]
    jmp .while 
.end_while:
    mov rax, -1
    leave
    ret
init:
    enter 0, 0
    lea rax, [head]
    mov qword[head + 24], rax
    add qword[head + 24], 8
    PRINT_DEC 8, [head + 8]
    PRINT_CHAR ' '
    lea rax, [head]
    mov qword[tail + 24], rax
    add qword[tail + 24], 8
    PRINT_DEC 8, [tail + 8]
    leave 
    ret
malloc:; malloc(size)
    mov rcx, [rbp + 16]; size параметр
    push rcx
    call get_free_block
    add rsp, 8
    cmp rax, -1
    jne .return
    jmp .if
.return:
    leave
    ret
.if:
    mov rcx, [rbp + 16]; size параметр
    push rcx
    call sbrk
    add [tail + 24], rax
    add [rail + 8], 
main:
    mov rbp, rsp; for correct debugging
    call init
    mov rax, 2
    push rax
    call get_free_block
    add rsp, 8
    ret
    
