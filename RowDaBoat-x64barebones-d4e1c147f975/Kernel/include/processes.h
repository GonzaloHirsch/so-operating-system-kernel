//
// Created by IR on 2/10/2019.
//

/*
 * Dear reader,
 *              **** Process.c/.h ****
 *              Lo que hay que saber de procesos: primero, aca se identifica a un proceso con una
 *              funcion de partida (como si fuera el main, pero puede ser en realidad cualquier funcion).
 *              Lo primero que se hace es crear(se) el ADT del proceso (Process), a partir de la funcion newProcess.
 *              Con esta funcion se le asigna un stack al proceso, to-do esto (el Process y el Stack) se allocan en
 *              memoria dinamica. En el stack se aniade el interrupt frame y arriba de eso los registros.
 *              En el stack frame se settea el RIP al address de la funcion wrapper (se supone que cuando se haga iretq,
 *              se ejecute la funcion wrapper, que lo que hace es ejecutar el "main" del proceso). Este wrapper (se llama
 *              EntryPoint)necesita parametros: functionAddress y pid, el ultimo siendo para poder terminar el proceso una
 *              vez ejecutada la funcion; los parametros se guardan en los registros rdi y rsi por razones obvias.
 *              En process.c hay un array de Process, para poder referenciarlos directamente por PID.
 *
 *
 *              **** Scheduler.c/.h ****
 *              To-do bien con los procesos creados, pero tambien se necesita schedulearlos, y por lo tanto se necesita un lugar
 *              donde guardar el PCB de cada proceso. En realidad, los contenidos de un PCB (stack pointer, comienzo del stack,
 *              estado, etc.) se guardan en el adt Process. Los PCB son mas un ADT wrapper para poder hacer una linked list y asi
 *              elegir al proceso siguiente con el algoritmo que se quiera, separando el comportamiento de un process del del scheduler.
 *              Cuestion, cada vez que creas un proceso con newProcess, tambien lo aniadis al scheduler.
 *
 *              Lo mas importante del scheduler es la funcion getNextProcess, que recibe el stack del proceso actual y te devuelve el
 *              stack del proximo proceso que este ready. Literalmente lo que hace es recorrer la linkedlist hasta encontrar un proceso
 *              cuyo estado sea READY.
 *              Notar que esta ultima funcion es llamada por el handler del interrupt 0x70, el del RTC.
 *
 *
 *
 *              Ahora si, corregi y agrega lo que quieras. Tschuss
 */

#ifndef SO_TP2_PROCESS_H
#define SO_TP2_PROCESS_H

#define MAX_NAME_LENGTH 128
#define PROCESS_STACK_SIZE 1024
#define MAX_PROCESS_COUNT 256

#include <stddef.h>
#include <stdint.h>


typedef struct ProcessCDT * Process;

enum State{STATE_READY, STATE_RUNNING, STATE_BLOCKED, STATE_TERMINATED};

void initProcesses();

// constructors
// Crea un proceso con el nombre especificado (limitarse al nombre del
// programa al que pertenecen) y la agrega al scheduler.
Process newProcess(char * processName, uint64_t functionAddress);


// destructors
void removeProcess(Process process);

// getters / setters
uint64_t getStackPointer(Process process);
void setStackPointer(Process process, uint64_t stackPtr);

enum State getProcessState(Process process);
void setProcessState(Process process, enum State state);

#endif //SO_TP2_PROCESS_H
