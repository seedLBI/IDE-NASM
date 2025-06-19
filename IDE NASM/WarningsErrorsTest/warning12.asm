global start
 
extern WriteFile
extern GetStdHandle
 
section .data
db 
"10",10
message: db "Hello METANIT.COM!",10
message2: db "Hello METANIT.COM!",10
message3: db "Hello METANIT.COM!",10

; God damn FUCKING SECTIONS!!!!!

SectioN .texxxxttt  t

message1113: db "Hello METANIT.COM!",10


%iNcLuDe "testinclude.asm"
someFunction2:


mov ah,10 ;IM GERE
ret


message111333: db "Hello METANIT.COM!",10

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
	
message441: db "Hello METANIT.COM!",10
	
section .data
message228: db "Hello METANIT.COM!",10