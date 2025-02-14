section .bss
    result resb 256

section .data
    str1 db '123456789123456789', 0
    str2 db '987654321987654321', 0
    str2_len equ $ - str2

section .text
    global _start

_start:
    ; Адреса строк для сложения
    mov rsi, str1
    mov rdi, str2
    mov rdx, result
    call add_strings

    ; Реверс результата для правильного отображения
    mov rsi, result
    call reverse_string

    ; Вывод результата на экран
    mov rsi, result
    call print_string

    ; Завершение программы
    mov rax, 60       ; syscall: exit
    xor rdi, rdi      ; статус выхода 0
    syscall

; Функция для реверсирования строки
reverse_string:
    mov rdi, rsi
    call strlen
    dec rax
    mov rcx, rax
    shr rcx, 1
    jz reverse_done

reverse_loop:
    mov al, [rsi]
    mov bl, [rsi + rax]
    mov [rsi], bl
    mov [rsi + rax], al
    inc rsi
    dec rax
    loop reverse_loop

reverse_done:
    ret

; Функция для вычисления длины строки
strlen:
    xor rax, rax
strlen_loop:
    cmp byte [rdi + rax], 0
    je strlen_done
    inc rax
    jmp strlen_loop
strlen_done:
    ret

; Функция для сложения двух строк
add_strings:
    xor r8, r8         ; Перенос (carry)
    mov rcx, str2_len
    std                ; Установить направление строк на декремент
    add rsi, rcx       ; Переместить указатель к концу строки
    add rdi, rcx
    add rdx, rcx       ; Переместить указатель к концу результата
    inc rdx
    dec rsi
    dec rdi

add_loop:
    mov al, [rsi]
    sub al, '0'
    mov bl, [rdi]
    sub bl, '0'
    add al, bl
    add al, r8
    cmp al, 10
    jl no_carry
    sub al, 10
    mov r8, 1
    jmp store_result
no_carry:
    xor r8, r8
store_result:
    add al, '0'
    stosb
    dec rsi
    dec rdi
    loop add_loop

    cld                ; Сброс направления строк на инкремент

    test r8, r8
    jz add_done
    mov al, '1'
    stosb

add_done:
    mov byte [rdx], 0  ; Завершаем строку нулевым байтом
    ret

; Функция для вывода строки
print_string:
    mov rdx, rsi
    call strlen
    mov rdx, rax
    mov rax, 1
    mov rdi, 1
    syscall
    ret
