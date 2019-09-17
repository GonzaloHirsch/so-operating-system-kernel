#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <stdint.h>

//siguiendo el protocolo de funcion para 64 bit
/*
	Parametros:
		uint64_t - rdi: Codigo de la syscall
		uint64_t - rsi: Puntero a un file descriptor
		uint64_t - rdx: Puntero a un string
		uint64_t - rcx: Longitud del string
		El resto quedan para que los usemos si los necesitamos, sino quedan en 0
*/
extern uint64_t _int80(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);

void _cli();
void _sti();
void _hlt();

#endif
