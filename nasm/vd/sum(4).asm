section .text

    global sum_ymm
    global max_ymm

    sum_ymm:
        enter 0, 0

        push rbx

        xor rbx, rbx
        vxorpd ymm0, ymm0, ymm0
    .cycle:
        cmp rbx, rsi
        jnl .end_cycle

        vmovups ymm1, [rdi + 4 * rbx]
        vaddps ymm0, ymm0, ymm1

        add rbx, 8
        jmp .cycle
    .end_cycle:

        vhaddps ymm0, ymm0, ymm0
        vhaddps ymm0, ymm0, ymm0
        vextractf128 xmm1, ymm0, 1
        vaddss xmm0, xmm0, xmm1

        pop rbx
        leave
        ret

    
    max_ymm:
        enter 0, 0

        push rbx

        xor rbx, rbx
        vmovups ymm0, [rdi + 4 * rbx]
        add rbx, 8
    .cycle:
        cmp rbx, rsi
        jnl .end_cycle

        vmovups ymm1, [rdi + 4 * rbx]
        vmaxps ymm0, ymm0, ymm1

        add rbx, 8
        jmp .cycle
    .end_cycle:

        vextractf128 xmm1, ymm0, 1
        vmaxps xmm1, xmm0, xmm1
        
        movss xmm0, xmm1
        psrldq xmm1, 4
        maxss xmm0, xmm1
        psrldq xmm1, 4
        maxss xmm0, xmm1
        psrldq xmm1, 4
        maxss xmm0, xmm1


        pop rbx
        leave
        ret
        