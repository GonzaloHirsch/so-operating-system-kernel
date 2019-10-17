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

extern void _force_change_process();

//Settea el stack del proceso (con registros, frame de interrupt)
//Devuelve el current rsp del proceso
static uint64_t initializeProcessStack(uint64_t stackBaseAddress, uint64_t functionAddress, int pid);
//El wrapper para ejecutar las funciones que son los procesos
static void entryPoint(uint64_t functionAddress, int pid);

struct ProcessCDT{

    int pid;
    int ppid;
    int priority;
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

//todo esto esta ok
void initProcesses(){
    pidCounter = 0;
    //memset(&processList, 0x0, MAX_PROCESS_COUNT*sizeof(Process));
    for(int i = 0; i<MAX_PROCESS_COUNT; i++){
        processList[i] = NULL;
   }
    return;
}

Process newProcess(char *processName, uint64_t functionAddress, int priority) {


    Process aux = (Process) mAlloc(sizeof(struct ProcessCDT));
    strcpy(aux->name, processName);
    //todo tests
    print(aux->name);

    aux->pid = pidCounter;
    aux->priority = priority;
    aux->ppid = (pidCounter!=0) ? getCurrentProcess()->pid : 0;
    aux->functionAddress = functionAddress;
    aux->stackBaseAddress = (uint64_t) mAlloc(PROCESS_STACK_SIZE);
    aux->stackPointer = initializeProcessStack(aux->stackBaseAddress, functionAddress, pidCounter);
    aux->state = STATE_READY;
    processList[pidCounter++] = aux;
    return aux;
}

static uint64_t initializeProcessStack(uint64_t stackBaseAddress, uint64_t functionAddress, int pid){

    ProcessStack * processStack = (ProcessStack *)(stackBaseAddress + PROCESS_STACK_SIZE - sizeof(ProcessStack) - 1);
    //todo memset(&processStack->gs, (uint32_t) 0x0000, sizeof(uint32_t)*2*10); //setteo a 0 todos los registros gs a r8
    //argumentos del wrapper
    processStack->rdi = functionAddress; //Esto es para tener como parametro al functionAddress cuando llame al wrapper (entryPoint)
    processStack->rsi = pid;
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


static void entryPoint(uint64_t functionAddress, int pid){

    // ejecuto  la funcion en functionAddress
    ((void*(*)()) functionAddress)();

    // setteo el estado del proceso como terminado una vez finalizada
    // la ejecucion de la funcion
    processList[pid]->state = STATE_TERMINATED;
    for(int i =0; i<50; i++){
        print("fin del programa\n");
    }
    //_force_change_process();
    sleep(2000);
    deleteCurrentProcessPCB();
    print("changed Process\n");
    sleep(2000);
}

// destructors

void removeProcess(Process process){
    processList[process->pid] = NULL;
    // se libera el espacio reservado para el stack
    mFree((void*) process->stackBaseAddress);
    // se libera el espacio reservado para el ADT de Process
    mFree(process);
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

int getPid(Process process){
    return process->pid;
}

void setPid(Process process, int pid){
    process->pid = pid;
}