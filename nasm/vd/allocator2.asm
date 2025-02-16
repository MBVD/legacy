%include "io64.inc"
section .bss
    brk resq 0            ; Текущий адрес вершины кучи
section .data
    size dq 0 
head:
    db 0; used
    dq 0; adress
    dq 0; size
    
section .text
global main

sbrk:; сколько памяти мы в принципе выделяем нужно увеличивать, если у нас уже все остальное +- забито
    enter 0, 0
    mov rdx, [rbp + 16]
    mov rcx, [brk]
    mov rax, [brk]
    inc rcx
    inc rax; - адресс начала элемента
    add rcx, rdx
    mov [brk], rcx
    leave
    ret
malloc:
    enter 0, 0
    mov r8, [rbp + 16]; передаваемое значение size
    ; поиск свободного места для этого нужно пробежаться по стеку
    mov r9, [rbp + 24]; значение начала стека, где мы храним переменные будет start_stack
    mov r10, [rbp + 32]; конец стека
    mov r11, [r9] ; берем 1 элемент стека tmp
    add r9, 8
.for_find:; доработки
    cmp r9, r10
    jge .end_for_find
    sub r11, [r9]; tmp - [r9] - размер
    cmp r11, r8; cmp размер, size(в параметрах фунции) и в r9 лежит адреес в хипе куда можно перезаписать 
    ; todo надо чекнуть используем ли мы этот адресс или уже он пустой
    jle .end_for_find
    mov r11, [r9]
    
    jmp .for_find
.addr9:
    add r9, 8
    jmp .for_find
.end_for_find:
    ; как то надо проверить что адресс не занят под переменную
    ;lea rdi, [used + r9]
    ;mov dl, byte [used + r9] 
    ;cmp byte[rdi + r9], 1
    je .addr9
    cmp r9, r10
    je .sbrk_call ; не нашли пустого места
    ; нашли пустое место
    mov rax, [r9]; возвращаем наше свободное место в хипе можно его заполнять спокойно
    mov rcx, 1
    jmp .return0
.sbrk_call:
    push r8
    call sbrk
    mov rcx, 0 ; что программа успешно выполненна это значит что потом нужно в стэк запушить нашу новый адресс rax
.return0:
     leave
     ret
main:
    mov rbp, rsp; for correct debugging
    GET_DEC 8, [size]
    mov rax, [size]
    push rsp
    push rsp
    push rax
    call malloc
    add rsp, 8
    ret
