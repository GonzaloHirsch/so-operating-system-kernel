GLOBAL cpuVendor
GLOBAL getSeconds
GLOBAL getKeyboard

GLOBAL getVideoBaseAddress
GLOBAL getVideoX
GLOBAL getVideoY
GLOBAL getVideoDepth

section .text

cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

getSeconds:
	push rbp
	mov rbp, rsp

	mov al, 0
	out 70h, al
	in ax, 71h

	mov rsp, rbp
	pop rbp
	ret

;lee del puerto 64h hasta que el primer bit esta en 1
;lo hace por pooling
getKeyboard:
	push rbp
	mov rbp, rsp
_loop:
	in al, 64h				;lee del puerto 64h
	and al, 01h				;le hace un AND con 0000 0001 para que quede en 1
	cmp al, 01h				;compara para ver que haya quedado en 0000 0001
	jz _show
	jmp _loop
_show:
	in al, 60h				;lee del puerto 60h para conseguir la tecla
	mov rsp, rbp
	pop rbp
	ret
