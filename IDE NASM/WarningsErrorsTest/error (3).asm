%macro test_macro 1
    mov eax, %1
; %endmacro missing here
section .text
global _start
_start:
    test_macro 5