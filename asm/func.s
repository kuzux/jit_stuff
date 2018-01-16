.intel_syntax noprefix

test:
push rbp
mov rbp, rsp

push rdi
push rsi

mov eax, 3
mov ebx, 5
add eax, ebx

pop rsi
pop rdi

mov rsp, rbp
pop rbp

ret

