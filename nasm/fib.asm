%include "io64.inc"
section .text
global CMAIN
CMAIN:
    mov rbp, rsp; for correct debugging
	GET_DEC 8, rax
	push rax
	call fib
	add rsp, 8

	PRINT_DEC 8, rax
	ret
fib:
	enter 16, 0
	cmp qword[rbp + 16], 0
	je .end_zero

	cmp qword[rbp + 16], 1
	je .end_one

	push qword[rbp + 16]
	pop qword[rbp - 8]
	dec qword[rbp - 8]
	
	push qword[rbp - 8]
	call fib
	add rsp, 8
	mov [rbp - 16], rax

	dec qword[rbp - 8]
        push qword[rbp - 8]
	call fib
	add rsp, 8
	add rax, [rbp - 16]
	jmp .end 

.end_zero:
	xor rax, rax
	jmp .end
.end_one:
	mov rax, 1
.end:
	leave
	ret