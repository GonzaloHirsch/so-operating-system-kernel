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

static PCBList thePCBList /*= {NULL, NULL, NULL}*/;

void newPCB(Process process){

    PCB aux = mAlloc(sizeof(PCBCDT)); //TODO VER CON NUESTRO ALLOCATOR
    //PCB aux = mem_alloc(sizeof(PCBCDT));
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
    _popaqIretq(getStackPointer(process)); // [interrupts.asm jaja saludos]
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
        mFree(aux); //todo hacer con nuestro allocator
        //free_mem(aux);
        //_popaqIretq(getStackPointer(thePCBList.currentPCB->process));
    }
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

                        // si el proceso actual esta bloqueado, sigo con el proximo
                        case STATE_BLOCKED:
                            //print("blocked\n");
                            thePCBList.currentPCB = thePCBList.currentPCB->next;
                            break;
                            // si el proceso actual esta terminado, hay que eliminarlo y seguir
                        case STATE_TERMINATED:
                            //print("Process %s : terminated\n", getProcessName(thePCBList.currentPCB->process));
                            deleteCurrentProcessPCB();
                            break;
                        case STATE_RUNNING:
                            //print("Process %s : running\n", getProcessName(thePCBList.currentPCB->process));
                            if (thePCBList.currentPCB->currentPriority < getPriority(thePCBList.currentPCB->process)) {
                                //if(0){
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

                //print("Current process PID: %d", getProcessPid(thePCBList.currentPCB->process));
                //sleep(2);

                setProcessState(thePCBList.currentPCB->process, STATE_RUNNING);


                // Se devuelve el stack pointer del proximo proceso
                return getStackPointer(thePCBList.currentPCB->process);

        }
        }
        else {
            return currentProcessStack;
        }
    }