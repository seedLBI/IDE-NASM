section .text
    global _start
_start:
    mov ax, 0xffffffffff
    int 0x80