global _start       ; делаем метку метку _start видимой извне

section .text       ; объявление секции кода
_start:             ; метка _start - точка входа в программу
    mov rax, [eax + eax * 0xffffff]    ; помещаем в регистр rax код возврата - 23 
    ret            ; выход из программы