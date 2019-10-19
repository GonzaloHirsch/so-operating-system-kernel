//
// Created by IR on 2/10/2019.
//

#include "../include/scheduler.h"
#include "../include/processes.h"
#include "../include/memManager.h"
#include <lib.h>
#include <console.h>

extern void _force_change_process();
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

static PCBList theProcessList /*= {NULL, NULL, NULL}*/;

void newPCB(Process process){

    PCB aux = mAlloc(sizeof(PCBCDT)); //TODO VER CON NUESTRO ALLOCATOR
    //PCB aux = mem_alloc(sizeof(PCBCDT));
    aux->process = process;
    theProcessList.processCount++;
    aux->currentPriority=0;
    if(theProcessList.processCount==1){
        theProcessList.currentPCB = aux;
        theProcessList.tail = aux;
        aux->next = aux->prev = aux;
        startProcess(aux->process);
    }
    else {
        theProcessList.tail->next->prev = aux;
        aux->next = theProcessList.tail->next;
        aux->prev = theProcessList.tail;
        theProcessList.tail->next = aux;
        theProcessList.tail = aux;
    }


}

void startProcess(Process process){
    setProcessState(process, STATE_RUNNING);
    _popaqIretq(getStackPointer(process)); // [interrupts.asm jaja saludos]
}

void initializeScheduler(){
    theProcessList.currentPCB = NULL;
    theProcessList.tail = NULL;
    theProcessList.processCount = 0;
}

Process getCurrentProcess(){
    return theProcessList.currentPCB->process;
}

void deleteCurrentProcessPCB(){
    if(theProcessList.currentPCB != NULL) {
        theProcessList.processCount--;
        PCB aux = theProcessList.currentPCB;
        theProcessList.currentPCB = aux->next;

        aux->next->prev = aux->prev;
        aux->prev->next = aux->next;
        removeProcess(aux->process);
        mFree(aux); //todo hacer con nuestro allocator
        //free_mem(aux);
        //_popaqIretq(getStackPointer(theProcessList.currentPCB->process));
    }
}

uint64_t getNextProcess(uint64_t currentProcessStack){


        if (theProcessList.processCount > 1) {

            if (theProcessList.currentPCB->currentPriority < getPriority(theProcessList.currentPCB->process)) {
                theProcessList.currentPCB->currentPriority++;
                return currentProcessStack;
            } else {
                theProcessList.currentPCB->currentPriority=0; //resetteo la cantidad de cuantos a 0
                if (getProcessState(theProcessList.currentPCB->process) != STATE_TERMINATED) {

                    setStackPointer(theProcessList.currentPCB->process, currentProcessStack);

                } else {

                }
                // Se settea el nuevo current process...
                enum State state;

                for (state = getProcessState(theProcessList.currentPCB->process);
                     state != STATE_READY; state = getProcessState(theProcessList.currentPCB->process)) {


                    switch (state) {

                        // si el proceso actual esta bloqueado, sigo con el proximo
                        case STATE_BLOCKED:
                            print("blocked\n");
                            break;
                            // si el proceso actual esta terminado, hay que eliminarlo y seguir
                        case STATE_TERMINATED:
                            //print("Process %s : terminated\n", getProcessName(theProcessList.currentPCB->process));
                            deleteCurrentProcessPCB();
                            break;
                        case STATE_RUNNING:
                            //print("Process %s : running\n", getProcessName(theProcessList.currentPCB->process));
                            setProcessState(theProcessList.currentPCB->process, STATE_READY);
                            break;
                        default:
                            break;
                    }

                    theProcessList.currentPCB = theProcessList.currentPCB->next;
                }

                //print("Current process PID: %d", getPid(theProcessList.currentPCB->process));
                //sleep(2);

                setProcessState(theProcessList.currentPCB->process, STATE_RUNNING);


                // Se devuelve el stack pointer del proximo proceso
                return getStackPointer(theProcessList.currentPCB->process);
            }
        }
        else {
            return currentProcessStack;
        }
    }