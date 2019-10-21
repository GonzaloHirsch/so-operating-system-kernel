//
// Created by IR on 2/10/2019.
//

#include "../include/processes.h"
#include "../include/memManager.h"
#include "../include/lib.h"
#include "../include/scheduler.h"
#include <lib.h>
#include <stdint.h>
#include <console.h>
#include <time.h>

extern void forceChangeProcess();

//Settea el stack del proceso (con registros, frame de interrupt)
//Devuelve el current rsp del proceso
static uint64_t initializeProcessStack(uint64_t stackBaseAddress, uint64_t functionAddress, uint64_t pid);
//El wrapper para ejecutar las funciones que son los procesos
static void entryPoint(uint64_t functionAddress, uint64_t pid);

struct ProcessCDT{

    int pid;
    int ppid;
    int priority;
    enum State state;
    enum Visibility isForeground;
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
static Process foregroundProcess;
static Process theProcessList[MAX_PROCESS_COUNT];

//todo esto esta ok
void initProcesses(){
    pidCounter = 0;
    foregroundProcess = NULL;
    for(int i = 0; i<MAX_PROCESS_COUNT; i++){
        theProcessList[i] = NULL;
    }

    return;
}

Process newProcess(char *processName, uint64_t functionAddress, int priority, enum Visibility isForeground) {


    //Process aux = (Process) mem_alloc(sizeof(struct ProcessCDT));
    Process aux = (Process) mAlloc(sizeof(struct ProcessCDT));
    strcpy(aux->name, processName);
    //todo tests

    aux->pid = pidCounter;
    aux->priority = priority;
    aux->isForeground = isForeground;
    aux->ppid = (pidCounter>0) ? getCurrentProcess()->pid : -1;
    aux->functionAddress = functionAddress;
    //aux->stackBaseAddress = (uint64_t) mem_alloc(PROCESS_STACK_SIZE);
    aux->stackBaseAddress = (uint64_t) mAlloc(PROCESS_STACK_SIZE);
    aux->stackPointer = initializeProcessStack(aux->stackBaseAddress, functionAddress, (uint64_t) aux);
    aux->state = STATE_READY;
    theProcessList[pidCounter++] = aux;
    return aux;
}

static uint64_t initializeProcessStack(uint64_t stackBaseAddress, uint64_t functionAddress, uint64_t processPtr){

    ProcessStack * processStack = (ProcessStack *)(stackBaseAddress + PROCESS_STACK_SIZE - sizeof(ProcessStack) - 1);
    //todo memset(&processStack->gs, (uint32_t) 0x0000, sizeof(uint32_t)*2*10); //setteo a 0 todos los registros gs a r8
    //argumentos del wrapper
    processStack->rdi = functionAddress; //Esto es para tener como parametro al functionAddress cuando llame al wrapper (entryPoint)
    processStack->rsi = processPtr;
    //todo memset(&processStack->rsi, (uint32_t) 0x0000, sizeof(uint32_t)*2*5); //setteo los registros restantes a 0

    //setteo los registros no usados a 0
    processStack->cs = processStack->fs = processStack->r15=processStack->r14=processStack->r13=0x0;
    processStack->r12=processStack->r11=processStack->r10=processStack->r9=processStack->r8 = 0x0;
    processStack->rbp=processStack->rdx=processStack->rcx=processStack->rbx=processStack->rax = 0x0;

    //Interrupt Stack Frame
    processStack->rip = (uint64_t) &entryPoint;
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


static void entryPoint(uint64_t functionAddress, uint64_t processPtr){

    // ejecuto  la funcion en functionAddress
    ((void*(*)()) functionAddress)();

    // setteo el estado del proceso como terminado una vez finalizada
    // la ejecucion de la funcion
    ((Process)processPtr)->state = STATE_TERMINATED;
    forceChangeProcess();
}

// destructors

void removeProcess(Process process){
    // se libera el espacio reservado para el stack
    theProcessList[process->pid] = NULL;
    mFree((void*) process->stackBaseAddress);
    //free_mem((void*)process->stackBaseAddress);
    // se libera el espacio reservado para el ADT de Process
    mFree(process);
    //free_mem(process);
}

// getters / setters

uint64_t getStackPointer(Process process){
    return process->stackPointer;
}

void setStackPointer(Process process, uint64_t stackPtr){
    process->stackPointer = stackPtr;
}

enum State getProcessState(Process process){
    return process->state;
}

void setProcessState(Process process, enum State state){
    process->state = state;
}

void setProcessStateByPid(int pid, enum State state){
    theProcessList[pid]->state = state;
}

int getProcessPid(Process process){
    return process->pid;
}

void setPid(Process process, int pid){
    process->pid = pid;
}

char * getProcessName(Process process){
    return process->name;
}

int getPriority(Process process){
    return process->priority;
}

void setPriorityByPid(int pid, int priority){
    theProcessList[pid]->priority = priority;
}

int getPid(){
   return getCurrentProcess()->pid;
}

void listProcesses(){
    Process aux;
    for(int i = 0; i<pidCounter; i++){
        if((aux=theProcessList[i])!=NULL){
            print("Process %s\n    PID: %d\n    Priority: %d\n    StackPointer: %d\n    Foreground? %s\n", aux->name, aux->pid, aux->stackPointer, (aux->isForeground) ? "Yes" : "No");
        }
    }
}

void setProcessPriorityByPid(int pid, int priority){
    theProcessList[pid]->priority = priority;
}

enum Visibility getProcessVisibility(Process process) {
    return process->isForeground;
}

enum Visibility getProcessVisibilityById(int pid) {
    return theProcessList[pid]->isForeground;
}

