//
// Created by IR on 2/10/2019.
//

#include "../include/scheduler.h"
#include "../include/processes.h"
#include "../include/memManager.h"

static void startProcess(Process process);

typedef struct ProcessControlBlockCDT{

    Process process;

    struct ProcessControlBlockCDT * prev;
    struct ProcessControlBlockCDT * next;

} PCBCDT;

typedef struct PCBListCDT{

    PCB currentProcess;
    PCB tail;
    int processCount;
}PCBList;

static PCBList theProcessList /*= {NULL, NULL, NULL}*/;

void newPCB(Process process){

    PCB aux = mAlloc(sizeof(PCBCDT));
    aux->process = process;
    theProcessList.processCount++;
    if(theProcessList.processCount==1){
        theProcessList.currentProcess = aux;
        theProcessList.tail = aux;
        aux->next = aux->prev = aux;
        startProcess(aux->process);
    }
    else {
        theProcessList.tail->next->prev = aux;
        aux->next = theProcessList.tail->next;
        aux->prev = theProcessList.tail;
        theProcessList.tail->next = aux;
    }


}

void startProcess(Process process){
    setProcessState(process, STATE_RUNNING);
    _popaqIretq(getStackPointer(process)); // [interrupts.asm jaja saludos]
}

void initializeScheduler(){
    theProcessList.currentProcess = NULL;
    theProcessList.tail = NULL;
    theProcessList.processCount = 0;
}

Process getCurrentProcess(){
    return theProcessList.currentProcess->process;
}

void deleteCurrentProcessPCB(){
    if(theProcessList.currentProcess != NULL) {
        theProcessList.processCount--;
        PCB aux = theProcessList.currentProcess;
        // aca es donde efectivamente se cambia el proceso
        theProcessList.currentProcess = aux->next;

        //elimino el nodo de la lista
        aux->next->prev = aux->prev;
        aux->prev->next = aux->next;
        // Se libera el espacio que se habia reservado para el PCB del Process
        removeProcess(aux->process);
        mFree(aux);
    }
}

uint64_t getNextProcess(uint64_t currentProcessStack){

    // Se guarda el stack pointer del proceso que se esta ejecutando ahora,
    // el estado pasa de STATE_RUNNING a STATE_READY
    setStackPointer(theProcessList.currentProcess->process, currentProcessStack);
    setProcessState(theProcessList.currentProcess->process, STATE_READY);

    // Se settea el nuevo current process...
    enum State state;
    for(state = getProcessState(theProcessList.currentProcess->process); state != STATE_READY; state = getProcessState(theProcessList.currentProcess->process)){

        switch(state){

            // si el proceso actual esta bloqueado, sigo con el proximo
            case STATE_BLOCKED:
                theProcessList.currentProcess = theProcessList.currentProcess->next;
                break;
            // si el proceso actual esta terminado, hay que eliminarlo y seguir
            case STATE_TERMINATED:
                deleteCurrentProcessPCB();
                break;
            default:
                break;
        }

    }

    setProcessState(theProcessList.currentProcess->process, STATE_RUNNING);


    // Se devuelve el stack pointer del proximo proceso
    return getStackPointer(theProcessList.currentProcess->process);
}