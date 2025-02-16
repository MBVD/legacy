%include "io64.inc"
section .data
x: dq 22 
b: dq 7
n: dq 11
section .bss
decimal_fraction resb 100
temp resb 100
section .text
global main
main:
    mov rbp, rsp; for correct debugging
    
    xor rax, rax
    call print_decimal
    PRINT_STRING decimal_fraction
    xor rax, rax
    ret 
    
    ;void print_decimal(int x, int b, int n, char* decimal_fraction)
print_decimal:
    enter 0, 0
    xor rax, rax
    mov rdi, [x]; int x
    mov rsi, [b]; int b
    mov rdx, [n]; int n
    lea rcx, [decimal_fraction]; char* decimal_fraction
    xor r8, r8 ; negative_result = 0;
    
    ; Обработка отрицательных чисел
    test rdi, rdi
    jge .check_b
    neg rdi
    inc r8
.check_b:
    test rsi, rsi
    jge .int_part
    neg rsi
    xor r8, 1
    
.int_part:
    mov r9, rdx ; r9 = n (точность)
    xor rdx, rdx
    mov rax, rdi
    div rsi
    mov r15, rdx ; remainder для дробной части
    lea r10, [temp]
    xor r11, r11 ; temp_pos
    test rax, rax ; if (integer_part == 0) {
    jnz .reverse_write
    mov byte [r10 + r11], '0' ; Если целая часть = 0
    inc r11
    jmp .copy_int_part

.reverse_write:
    mov r12, 10
    xor rdx, rdx
    div r12
    add dl, '0'
    mov [r10 + r11], dl
    inc r11
    test rax, rax
    jnz .reverse_write

.copy_int_part:
    test r8, r8
    jz .copy_digits
    
    mov byte [rcx], '-' ; Если число отрицательное
    inc rcx

.copy_digits:
    dec r11
    js .add_dot
.copy_loop:
    mov al, [r10 + r11]
    mov [rcx], al
    inc rcx
    dec r11
    jns .copy_loop

.add_dot:
    mov byte [rcx], '.'
    inc rcx

.calculate_frac:
    ; Начало вычисления дробной части
    mov rax, r15 ; remainder
    test rax, rax
    jz .finish

.calculate_frac_loop:
    imul rax, 10 ; Умножаем остаток на 10
    xor rdx, rdx
    div rsi ; Делим на знаменатель
    
    add al, '0' ; Преобразуем результат в цифру
    mov [rcx], al
    mov rax, rdx
    inc rcx
    dec r9
    jnz .calculate_frac_loop
    
.finish:
    mov byte [rcx], 0
    leave
    ret
