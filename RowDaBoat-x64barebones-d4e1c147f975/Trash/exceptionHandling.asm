GLOBAL printRegisters

extern goToUserland
;extern ncNewline
extern printInteger
extern printError
extern print
extern sleep

section .text

%macro pushState 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro	popState 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

printRegisters:


    pushState
    mov rdi, regax
    call printError
popState
    pushState
    mov rdi, rax
    call printInteger
    mov rdi, newline
call print
popState

    ;"RBX: "
    pushState
    mov rdi, regbx
    call printError
popState
    ;(El valor de rbx)
    pushState
    mov rdi, rbx
    call printInteger
    mov rdi, newline
call print
popState

pushState
    mov rdi, regcx
    call printError
popState
    pushState
    mov rdi, rcx
    call printInteger
    mov rdi, newline
call print
popState


pushState
    mov rdi, regdx
    call printError
popState
    pushState
    mov rdi, rdx
    call printInteger
    mov rdi, newline
call print
popState


pushState
    mov rdi, regbp
    call printError
popState
    pushState
    mov rdi, rbp
    call printInteger
    mov rdi, newline
call print
popState

pushState
    mov rdi, regsp
    call printError
popState
    pushState
    mov rdi, rsp
    call printInteger
    mov rdi, newline
call print
popState


pushState
    mov rdi, regdi
    call printError
popState
    pushState
    mov rdi, rdi
    call printInteger
    mov rdi, newline
call print
popState

pushState
    mov rdi, regsi
    call printError
popState
    pushState
    mov rdi, rsi
    call printInteger
    mov rdi, newline
call print
popState

pushState
    mov rdi, reg8
    call printError
popState
    pushState
    mov rdi, r8
    call printInteger
    mov rdi, newline
call print
popState

pushState
    mov rdi, reg9
    call printError
popState
    pushState
    mov rdi, r9
    call printInteger
    mov rdi, newline
call print
popState

mov rdi, 1000
call sleep      ;dar tiempo para que se puedan leer los registros


ret

section .rodata ;Esto es para los nombres de los registros...

regax db "RAX: ", 0
regbx db "RBX: ", 0
regcx db "RCX: ", 0
regdx db "RDX: ", 0
regbp db "RBP: ", 0
regsp db "RSP: ", 0
regdi db "RDI: ", 0
regsi db "RSI: ", 0
reg8 db "R8: ", 0
reg9 db "R9: ", 0
newline db 10, 0