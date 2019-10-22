//
// Created by IR on 2/10/2019.
//

#ifndef SO_TP2_SCHEDULER_H
#define SO_TP2_SCHEDULER_H

#include "processes.h"

typedef struct ProcessControlBlockCDT * PCB;

void initializeScheduler();
// aniade un proceso al scheduler
void newPCB(Process process);
Process getCurrentProcess();
// Elimina el PCB del proceso actual (que deberia estar en
// STATE_TERMINATED) y devuelve el PCB del proximo
void deleteCurrentProcessPCB();
// Switchea el proceso (segun el algoritmo del scheduler) y devuelve
// el stack pointer del nuevo proceso [ver interrupts.asm]
uint64_t getNextProcess();
int getProcessCount();




#endif //SO_TP2_SCHEDULER_H
