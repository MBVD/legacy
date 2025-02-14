section .data
    ; Определение входных параметров
    a dd 10
    b dd 20

section .text
    extern add      ; Объявление внешней функции
global main

main:
    ; Передача параметров в функцию
    mov eax, dword [a]
    mov ebx, dword [b]
    call add        ; Вызов функции

    ; Получение результата и вывод на экран
    mov ebx, eax    ; Результат хранится в eax
    mov eax, 1      ; Системный вызов для вывода
    int 0x80        ; Вызов ядра

    ; Завершение программы
    mov eax, 1      ; Системный вызов для выхода
    xor ebx, ebx    ; Код завершения
    int 0x80        ; Вызов ядра
    ret