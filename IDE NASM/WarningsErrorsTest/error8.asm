section .text
global _start
_start:
    mov eax, [eax + ebx * 2 + ecx]  ; Ошибка: некорректная адресация