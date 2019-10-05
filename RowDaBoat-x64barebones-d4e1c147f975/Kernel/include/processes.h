//
// Created by IR on 2/10/2019.
//

#ifndef SO_TP2_PROCESS_H
#define SO_TP2_PROCESS_H

#define MAX_NAME_LENGTH 128
#define PROCESS_STACK_SIZE 1024
#define MAX_PROCESS_COUNT 256

typedef struct ProcessCDT * Process;

enum State{STATE_READY, STATE_RUNNING, STATE_BLOCKED, STATE_TERMINATED};

// constructors
// Crea un proceso con el nombre especificado (limitarse al nombre del
// programa al que pertenecen) y la agrega al scheduler.
Process createProcess(char * processName, uint64_t functionAddress);

// destructors
void removeProcess(Process process);

// getters / setters
uint64_t getStackPointer(Process process);
void setStackPointer(Process process, uint64_t stackPtr);

enum State getProcessState(Process process);
void setProcessState(Process process, enum State state);

#endif //SO_TP2_PROCESS_H
