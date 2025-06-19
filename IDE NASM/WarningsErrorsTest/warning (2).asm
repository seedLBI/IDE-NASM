%macro duplicate 1
    nop
%endmacro

%macro duplicate 1    ; Ошибка: макрос "duplicate" уже определён
    nop
%endmacro

section .text
global _start
_start:
    duplicate 5