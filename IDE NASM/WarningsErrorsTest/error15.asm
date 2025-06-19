global start
 
extern WriteFile
extern GetStdHandle
 
section .data
message: db "Hello METANIT.COM!",10
message2: db "Hello METANIT.COM!",10
message3: db 10"Hello METANIT.COM!",10


SectioN .texxxxttt  t

someFunction2:


mov ah,10 ;IM GERE
ret

someFunction:

mov al,10
ret

start:
    sub  rsp, 40
    mov  rcx, -11
	call someFunction
	call someFunction2
    call GetStdHandle
    mov  rcx, rax
    mov  rdx, message
    mov  r8d, 18
    xor  r9, r9
    mov  qword [rsp + 32], 0
    call WriteFile 
    add  rsp, 40
    ret
	
section .data
message228: db "Hello METANIT.COM!",10