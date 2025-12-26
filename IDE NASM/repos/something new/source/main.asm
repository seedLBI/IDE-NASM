section .data
a dd 2.0
b dd 2.56
fmt db "%f",10,0

;d[]s..sdds


section .text
extern printf
global main

main:
	movss xmm0, [a]   ; initial sum = a
	mov ebx, 50000  ; loop counter (1,000,000 times)

loop_start:
	addss xmm0, [b]   ; accumulate: sum += b

	sub rsp, 32
	cvtss2sd xmm1, xmm0
	movq rdx, xmm1
	lea rcx, [fmt]
	call printf
	add rsp, 32

	dec ebx
	jnz loop_start    ; repeat until counter reaches 0

	mov eax, 1337
	ret
