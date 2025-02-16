section .data

    x dd 1.0, 2.0, 3.0, 4.0
    y dd 5.0, 6.0, 7.0, 8.0

section .text

    global main

    main:
        enter 0, 0

        vmovups xmm1, [x]

        psrldq xmm1, 4


        xor rax, rax
        leave
        ret