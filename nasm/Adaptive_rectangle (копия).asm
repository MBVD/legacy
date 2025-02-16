%include "float_macro.inc"

section .data
    a dq 0.0
    b dq 1.0
    n dq 1000
    eps dq 1e-6
    printf_format db "Integral is %f", 10, 0
    N equ 8

section .text

extern sin
extern fabs
extern exp
extern printf

    global main

    main:
        enter 0, 0

        movsd xmm0, [a]
        movsd xmm1, [b]
        mov rdi, [n]
        lea rsi, [g]
        call rectangle


        lea rdi, [printf_format]
        mov rax, 1
        call printf

        movsd xmm0, [a]
        movsd xmm1, [b]
        movsd xmm2, [eps]
        lea rdi, [g]
        call addaptive_rectangle


        lea rdi, [printf_format]
        mov rax, 1
        call printf

        xor rax, rax

        leave
        ret

    ;sin(x) + x

    ;{int (sin(x) + x) dx, x=a..b} = (x^2/2 - cos(x)) {x = a..b} = (b^2-a^2)/2 - (cos(b) - cos(a))

    ;double x - xmm0
    f:
        enter 8, 0
        movsd [rbp - 8], xmm0
        call sin ; sin(x)
        addsd xmm0, [rbp - 8] ; sin(x) + x
        leave
        ret

    ;2x^2 + sin(x * e^x)
    g:
        enter 16, 0
        movsd [rbp - 8], xmm0
        mulsd xmm0, xmm0 ; x^2
        mov rax, 2
        cvtsi2sd xmm1, rax ; 2.0
        mulsd xmm0, xmm1 ; 2x^2

        movsd [rbp - 16], xmm0  ; 2x^2

        movsd xmm0, [rbp - 8]
        call exp
        mulsd xmm0, [rbp - 8]

        call sin

        addsd xmm0, [rbp - 16] ; sin(x * e^x) + 2x^2

        leave
        ret






    ;double a, b, int n, double (*f)(double) - xmm0, xmm1, rdi, rsi
    rectangle:
        enter 48, 0

        push rbx

        sub rsp, 8
        movsd [rsp], xmm8

        movsd [rbp - 8], xmm0 ; a
        movsd [rbp - 16], xmm1 ; b
        mov [rbp - 24], rdi ; n
        mov [rbp - 32], rsi ; f

        movsd xmm0, [rbp - 16]
        subsd xmm0, [rbp - 8]; b - a
        cvtsi2sd xmm1, [rbp - 24] ; (double)n
        divsd xmm0, xmm1 ; h = (b-a)/n
        movsd [rbp - 40], xmm0; h

        movsd xmm0, [rbp - 8]
        movsd xmm1, [rbp - 40]
        mov rbx, 2
        cvtsi2sd xmm2, rbx
        divsd xmm1, xmm2 ; h/2
        addsd xmm0, xmm1; x1 = a + h/2
        movsd xmm8, xmm0
        

        xor rbx, rbx

        mov [rbp - 48], rbx; result
    .cycle:
        cmp rbx, [rbp - 24]; i < n
        jnl .end_cycle
        
        movsd xmm0, xmm8
        call [rbp - 32]; f(xi)
        movsd xmm1, [rbp - 48]
        addsd xmm1, xmm0
        movsd [rbp - 48], xmm1

        addsd xmm8, [rbp - 40]
        inc rbx
        jmp .cycle
    .end_cycle:


        movsd xmm0, [rbp - 48]
        mulsd xmm0, [rbp - 40]

        movsd xmm8, [rsp]
        add rsp, 8
        pop rbx
        leave
        ret


    addaptive_rectangle:
        enter 72, 0

        push rbx

        movsd [rbp - 8], xmm0 ; double a
        movsd [rbp - 16], xmm1 ; double b
        movsd [rbp - 24], xmm2 ; double eps
        mov [rbp - 32], rdi ; f
        
        xorpd xmm0, xmm0
        movsd [rbp - 40], xmm0 ; I1 = 0

        mov qword[rbp - 48], N ; n = 8

    .cycle:
        movsd xmm0, [rbp - 16] ; xmm0 = b
        subsd xmm0, [rbp - 8] ; b - a
        MOVI2D xmm1, [rbp - 48] ; transfered n into xmm1
        divsd xmm0, xmm1
        movsd [rbp - 56], xmm0 ; h

        movsd xmm0, [rbp - 56] ; xmm0 = h
        MOVI2D xmm1, 2 ; transfered 2 into xmm1
        divsd xmm0, xmm1
        addsd xmm0, [rbp - 8]
        movsd [rbp - 64], xmm0 ; x
        xorpd xmm0, xmm0
        movsd [rbp - 72], xmm0 ; I2 = 0

        xor rbx, rbx
    .sum:
        cmp rbx, [rbp - 48]
        jnl .end_sum

        movsd xmm0, [rbp - 64]
        call [rbp - 32] ; f(xi)

        addsd xmm0, [rbp - 72]
        movsd [rbp - 72], xmm0 ; I2 += f(xi)

        movsd xmm0, [rbp - 64]
        addsd xmm0, [rbp - 56]
        movsd [rbp - 64], xmm0 ; xi += h

        inc rbx 
        jmp .sum
    .end_sum:
        movsd xmm0, [rbp - 72]
        mulsd xmm0, [rbp - 56] ; I2 *= h
        movsd [rbp - 72], xmm0

        subsd xmm0, [rbp - 40]
        call fabs                   ;modul
        comisd xmm0, [rbp - 24]     ;if (abs(I1 - I2) < eps)
        jb .end                     ;return I2;


        movsd xmm0, [rbp - 72]
        movsd [rbp - 40], xmm0
        shl qword[rbp - 48], 1
        jmp .cycle
    .end:
        movsd xmm0, [rbp - 72]
        pop rbx
        leave
        ret
