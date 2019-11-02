#include "../include/scheduler.h"
#include "../include/processes.h"
#include "../include/memManager.h"
#include <lib.h>
#include <console.h>

extern void _popaqIretq(uint64_t stackPtr);

static void startProcess(Process process);

typedef struct ProcessControlBlockCDT{

    Process process;
    int currentPriority;

    struct ProcessControlBlockCDT * prev;
    struct ProcessControlBlockCDT * next;

} PCBCDT;

typedef struct PCBListCDT{

    PCB currentPCB;
    PCB tail;
    int processCount;
}PCBList;

static PCBList thePCBList;

void newPCB(Process process){

    PCB aux = mAlloc(sizeof(PCBCDT));
    aux->process = process;
    thePCBList.processCount++;
    aux->currentPriority=0;
    if(thePCBList.processCount==1){
        thePCBList.currentPCB = aux;
        thePCBList.tail = aux;
        aux->next = aux->prev = aux;
        startProcess(aux->process);
    }
    else {
        thePCBList.tail->next->prev = aux;
        aux->next = thePCBList.tail->next;
        aux->prev = thePCBList.tail;
        thePCBList.tail->next = aux;
        thePCBList.tail = aux;
    }


}

void startProcess(Process process){
    setProcessState(process, STATE_RUNNING);
    _popaqIretq(getStackPointer(process));
}

void initializeScheduler(){
    thePCBList.currentPCB = NULL;
    thePCBList.tail = NULL;
    thePCBList.processCount = 0;
}

Process getCurrentProcess(){
    return thePCBList.currentPCB->process;
}

void deleteCurrentProcessPCB(){
    if(thePCBList.currentPCB != NULL) {
        if(thePCBList.currentPCB == thePCBList.tail) thePCBList.tail = thePCBList.tail->prev;
        thePCBList.processCount--;
        PCB aux = thePCBList.currentPCB;
        thePCBList.currentPCB = aux->next;

        aux->next->prev = aux->prev;
        aux->prev->next = aux->next;
        removeProcess(aux->process);
        mFree(aux);
    }
}

int getProcessCount() {
    return thePCBList.processCount;
}

uint64_t getNextProcess(uint64_t currentProcessStack){



        if (thePCBList.processCount > 1) {


            {



                if (getProcessState(thePCBList.currentPCB->process) != STATE_TERMINATED) {
                    setStackPointer(thePCBList.currentPCB->process, currentProcessStack);
                } else {

                }
                // Se settea el nuevo current process...
                enum State state;


                for (state = getProcessState(thePCBList.currentPCB->process);
                     state != STATE_READY; state = getProcessState(thePCBList.currentPCB->process)) {

                    switch (state) {
                        case STATE_BLOCKED:
                            thePCBList.currentPCB = thePCBList.currentPCB->next;
                            break;
                        case STATE_TERMINATED:
                            deleteCurrentProcessPCB();
                            break;
                        case STATE_RUNNING:

                            if (thePCBList.currentPCB->currentPriority < getPriority(thePCBList.currentPCB->process)) {
                                thePCBList.currentPCB->currentPriority++;
                                return currentProcessStack;
                            }
                            else{
                                thePCBList.currentPCB->currentPriority=0; //resetteo la cantidad de cuantos a 0
                                setProcessState(thePCBList.currentPCB->process, STATE_READY);
                                thePCBList.currentPCB = thePCBList.currentPCB->next;
                                }
                            break;
                        default:
                            break;
                    }


                }

                setProcessState(thePCBList.currentPCB->process, STATE_RUNNING);


                // Se devuelve el stack pointer del proximo proceso
                return getStackPointer(thePCBList.currentPCB->process);

        }
        }
        else {
            return currentProcessStack;
        }
    }