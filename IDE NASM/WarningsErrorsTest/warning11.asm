global _start
resb 10  ; Ошибка: резервирование памяти вне секции
section .text
_start:
    mov eax, 1