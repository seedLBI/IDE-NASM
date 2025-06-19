section .text
    global _start
    _start:
        mov eax, [my_data]  ; Предупреждение: неопределенный символ
        mov ebx, 0
section .data
    my_data dd 1234