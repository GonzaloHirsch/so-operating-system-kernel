//
// Created by IR on 2/10/2019.
//

#include "../include/processes.h"
#include <stdint.h>


//Settea el stack del proceso (con registros, frame de interrupt)
//Devuelve el current rsp del proceso
static uint64_t initializeProcessStack(uint64_t stackBaseAddress);

struct ProcessCDT{

    int pid;
    int ppid;
    enum State state;
    uint64_t stackBaseAddress;
    uint64_t stackPointer;
    uint64_t functionAddress;
    char name[MAX_NAME_LENGTH];
};

typedef struct ProcessStack{


    uint64_t gs;
    uint64_t fs;
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t rbp;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;

    uint64_t rip;
    uint64_t cs;
    uint64_t eflags;
    uint64_t rsp;
    uint64_t ss;

    uint64_t firstLine;
} ProcessStack;

static int pidCounter;
static Process processList[MAX_PROCESS_COUNT];

initProcesses(){
    pidCounter = 0;
    processList = {0};
}

Process newProcess(char * process_name, uint64_t functionAddress){

    Process aux = (Process) mAlloc(sizeof(struct ProcessCDT));
    strcpy(aux->name, process_name);
    aux->pid = pidCounter;
    aux->functionAddress = functionAddress;
    aux->stackBaseAddress = (uint64_t) mAlloc(PROCESS_STACK_SIZE);
    aux->stackPointer = initializeProcessStack(aux->stackBaseAddress);
    return aux;
}

uint64_t initializeProcessStack(uint64_t stackBaseAddress){

    ProcessStack * processStack = (ProcessStack *)(stackBaseAddress + PROCESS_STACK_SIZE - sizeof(ProcessStack) - 1);
    memset(processStack->gs, 0x0000, sizeof(uint32_t)*2*17); //setteo a 0 todos los registros en el stack
    processStack->rip =
    processStack->cs = 0x8;
    processStack->eflags = 0x202;

    // setteo el stack pointer del proceso para que
    // apunte abajo del frame del iretq (el stack crece para arriba towards valores mas bajos de memoria)
    processStack->rsp = (uint64_t) (&processStack->firstLine);
    processStack->ss = 0x0;
    processStack->firstLine = 0x0;

    //devuelvo puntero al top del stack
    return (uint64_t) &processStack->gs;
}


void killProcess(int pid){


}