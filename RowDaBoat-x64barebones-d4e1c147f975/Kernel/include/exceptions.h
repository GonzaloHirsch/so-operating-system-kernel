//
// Created by root on 6/12/19.
//

#ifndef ARQUI_TPE_EXCEPTIONDISPATCHER_H
#define ARQUI_TPE_EXCEPTIONDISPATCHER_H

#include <stdint.h>

void exceptionDispatcher(uint64_t type);
void printRegisters();

void _exception00Handler();
void _exception06Handler();
void _exception13Handler();
void _exception14Handler();


//Del este struct solo uso rip, pero lo pongo por si
//llegamos a implementar algo mas con las excepciones
//Estaria interesante poder hacer al rip igual al guardado
//sumarle 1 (o ir a la proxima instr correspondiente) y
//seguir de largo como si nada.
typedef struct {
    uint64_t stack_segment;
    uint64_t stack_ptr;
    uint64_t rflags;
    uint64_t cs;
    uint64_t rip;
    uint64_t errCode;

}ExceptionStackFrame;

typedef struct {

    uint64_t rax;
    uint64_t rbx;
    uint64_t rcx;
    uint64_t rdx;
    uint64_t rdi;
    uint64_t rsi;
    uint64_t rbp;
    uint64_t rsp;
    uint64_t r8;
    uint64_t r9;
}RegisterSet;

void setVerifying();
void clearVerifying();
void setExceptionAssert();
void loadExceptions();

#endif //ARQUI_TPE_EXCEPTIONDISPATCHER_H
