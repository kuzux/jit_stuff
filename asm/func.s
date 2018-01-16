.intel_syntax noprefix

test:
push rbp
mov rbp, rsp

mov eax, 3
mov ebx, 5
add eax, ebx

mov rsp, rbp
pop rbp

ret

