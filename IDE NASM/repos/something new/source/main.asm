section .data
a dd 2.0
b dd 2.56
fmt db "%f",10,0
section .text
extern printf
global main
main:
sub rsp, 32
movss xmm0, [a]
addss xmm0, [b]
cvtss2sd xmm0, xmm0
movq rdx, xmm0
lea rcx, [fmt]
call printf
add rsp, 32
mov eax, 228
ret
