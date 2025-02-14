%include "io64.inc"
section .data
    size dq 0
    size_of dq 8
section .text
global main
extern GetProcessHeap
extern HeapAlloc
extern ExitProcess
malloc:
    enter 0, 0
    call GetProcessHeap
    mov rcx, rax  ; rcx = дескриптор кучи
    mov rdx, [rbp + 16]; size
    push rdx
    push 8; флаг
    push rax
    call HeapAlloc
    add rsp, 24
    test rax, rax  ; Проверяем успешность выделения памяти
    jz .error       ; Если память не выделена, переходим к обработке ошибки
    ; В регистре rax теперь содержится адрес выделенной памяти
    xor rcx, rcx; код возврата 0
    PRINT_STRING "memmory allocated"
    PRINT_CHAR `\n`
    leave 
    ret
.error:
    ; Обработка ошибки
    ; Вывод сообщения об ошибке или другие действия
    ; Завершение программы с кодом ошибки
    mov rcx, 1   ; Код возврата 1 (ошибка)
    PRINT_STRING "error"
    PRINT_CHAR `\n`
    call ExitProcess
    leave
main:
    mov rbp, rsp; for correct debugging
    ;write your code here
    xor rax, rax
    GET_DEC 8, [size_of]
    GET_DEC 8, [size]
    mov rbx, [size]
    imul rbx, [size_of] 
    push rbx
    call malloc
    add rsp, 8
    xor rcx, rcx
    mov rdi, rax
.for_get:
    cmp rcx, [size] ; элементы массива
    jge .end_for_get
    GET_DEC 8, [rdi + 8 * rcx]
    inc rcx
    jmp .for_get
.end_for_get:
    xor rcx, rcx
.for_print:
    cmp rcx, [size]
    jge .end_for_print
    PRINT_DEC 8, [rdi + 8 * rcx]
    PRINT_CHAR ' '
    inc rcx
    jmp .for_print
.end_for_print:
    ret