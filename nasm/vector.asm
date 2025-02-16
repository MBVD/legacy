section .data
    arr dd 1, 2, 3, 4


section .text
           
    global main

    main:
        enter 0, 0

        movdqu xmm0, [arr]
        movdqu xmm1, [arr]

        paddd xmm0, xmm1

        xor rax, rax
        leave
        ret