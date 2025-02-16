section .data

section .text
global main
extern GetProcessHeap
extern HeapAlloc
extern ExitProcess

main:
    mov rbp, rsp; for correct debugging
    ; Получаем дескриптор кучи текущего процесса
    call GetProcessHeap
    mov rcx, rax  ; rcx = дескриптор кучи

    ; Выделяем 4096 байт (4 килобайта) из кучи
    mov rdx, 4096
    call HeapAlloc
    test rax, rax  ; Проверяем успешность выделения памяти
    jz error       ; Если память не выделена, переходим к обработке ошибки

    ; В регистре rax теперь содержится адрес выделенной памяти

    ; Используйте память, как вам нужно

    ; Освобождаем выделенную память
    mov rcx, rax   ; rcx = адрес выделенной памяти

    ; Завершаем программу
    xor eax, eax   ; Код возврата 0 (успешное завершение)
    call ExitProcess

error:
    ; Обработка ошибки
    ; Вывод сообщения об ошибке или другие действия
    ; Завершение программы с кодом ошибки
    xor eax, eax   ; Код возврата 1 (ошибка)
    call ExitProcess
    ret