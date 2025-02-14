section .text
global maximum
maximum:; float maximum(float* arr, int n)
    enter 16, 0
    mov [rbp - 8], rdi; arr
    mov [rbp - 16], rsi; int n
    mov r12, rsi
    add rsi, -4

    push rbx
    xor rbx, rbx
    vxorps xmm0, xmm0, xmm0; ans
.for:
    cmp rbx, rsi
    jge .end_for

    vmovups xmm1, [rdi + 4 * rbx]
    vmaxps xmm0, xmm1, xmm0

    add rbx, 4
    jmp .for
.end_for:
.rest:
    cmp rbx, rsi
    jge .end_rest
    vmaxss xmm0, xmm0, [rdi + 4 * rbx]
    inc rbx
    jmp .rest
.end_rest:

    pop rbx
    leave
    ret
