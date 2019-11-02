#include "../include/processes.h"
#include "../include/memManager.h"
#include "../include/lib.h"
#include "../include/scheduler.h"
#include "../include/intQueue.h"
#include "../include/processStack.h"
#include <lib.h>
#include <stdint.h>
#include <console.h>
#include <time.h>
#include <fileDescriptor.h>


extern void forceChangeProcess();

struct ProcessCDT{

    int pid;
    int ppid;
    int priority;
    enum State state;
    enum Visibility isForeground;
    uint64_t stackBaseAddress;
    uint64_t stackPointer;
    char name[MAX_NAME_LENGTH];
    IntQueue semaphores;
    int filesDescriptors[2]; //0 STDIN, 1 STDOUT.

};

static int pidCounter;
static Process theProcessList[MAX_PROCESS_COUNT];

//todo esto esta ok
void initProcesses(){
    pidCounter = 0;
    for(int i = 0; i<MAX_PROCESS_COUNT; i++){
        theProcessList[i] = NULL;
    }

    return;
}

Process newProcess(char *processName, uint64_t functionAddress, int priority, enum Visibility isForeground) {


    Process aux = (Process) mAlloc(sizeof(struct ProcessCDT));
    strcpy(aux->name, processName);

    aux->pid = pidCounter;
    aux->priority = priority;
    aux->isForeground = isForeground;
    aux->ppid = (pidCounter>0) ? getCurrentProcess()->pid : -1;
    aux->stackBaseAddress = (uint64_t) mAlloc(PROCESS_STACK_SIZE);
    aux->stackPointer = initializeProcessStack(aux->stackBaseAddress, functionAddress, (uint64_t) aux);
    aux->state = STATE_READY;
    theProcessList[pidCounter++] = aux;
    aux->semaphores = newQueue(MAX_SEMAPHORE_COUNT);
    //File descriptors default.
    aux->filesDescriptors[0] = 0;aux->filesDescriptors[1] = 1;
    return aux;
}


// destructors

void removeProcess(Process process){
    // se libera el espacio reservado para el stack
    theProcessList[process->pid] = NULL;
    mFree((void*) process->stackBaseAddress);
    while(!isEmpty(process->semaphores)){
        closeSemaphoreById(dequeue(process->semaphores));
    }
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

void setProcessStateByPid(int pid, enum State state){
    theProcessList[pid]->state = state;
}

int getProcessPid(Process process){
    return process->pid;
}

int getPriority(Process process){
    return process->priority;
}

int getPid(){
   return getCurrentProcess()->pid;
}

void listProcesses(){
    Process aux;
    char * resp;
    if(getProcessCount()==0) {
        for (int i = 0; i < pidCounter; i++) {
            if ((aux = theProcessList[i]) != NULL) {
                print("Process %s\n);", aux->name);
                print("State: ");
                switch(aux->state){
                    case STATE_TERMINATED:
                        write(1, "Terminated", strlen("Terminated"));
                        break;
                    case STATE_BLOCKED:
                        write(1, "Blocked", strlen("Blocked"));
                        break;
                    case STATE_READY:
                        write(1, "Ready", strlen("Ready"));
                        break;
                    case STATE_RUNNING:
                        write(1, "Running", strlen("Running"));
                        break;
                }
                print("\n    PID: %d\n    Priority: %d\n    StackPointer: %d\n    Foreground? ", aux->pid, aux->stackPointer);
                resp = (aux->isForeground) ? "No" : "Yes";
                print(resp);
                print("\n");
            }
        }
    }
    else {
        char buffer[20];
        for (int i = 0; i < pidCounter; i++) {
            if ((aux = theProcessList[i]) != NULL) {
                write(1, "Process ", strlen("Process "));
                strcpy(buffer, aux->name);
                write(1, buffer, strlen(buffer));
                write(1, "\n    State: ", strlen("\n    State: "));
                switch(aux->state){
                    case STATE_TERMINATED:
                        write(1, "Terminated", strlen("Terminated"));
                        break;
                    case STATE_BLOCKED:
                        write(1, "Blocked", strlen("Blocked"));
                        break;
                    case STATE_READY:
                        write(1, "Ready", strlen("Ready"));
                        break;
                    case STATE_RUNNING:
                        write(1, "Running", strlen("Running"));
                        break;
                }
                write(1, "\n    PID: ", strlen("\n    PID: "));
                itoa(aux->pid, buffer, 10);
                write(1, buffer, strlen(buffer));
                write(1, "\n    Priority: ", strlen("\n    Priority: "));
                itoa(aux->priority, buffer, 10);
                write(1, buffer, strlen(buffer));
                write(1, "\n    StackPointer: ", strlen("\n    StackPointer: "));
                itoa(aux->stackPointer, buffer, 10);
                write(1, buffer, strlen(buffer));
                write(1, "\n    Foreground: ", strlen("\n    Foreground: "));
                resp = (aux->isForeground) ? "No" : "Yes";
                write(1, resp, strlen(resp));
                print("\n");
            }
        }
    }
}

void setProcessPriorityByPid(int pid, int priority){
    theProcessList[pid]->priority = priority;
}

enum Visibility getProcessVisibility(Process process) {
    return process->isForeground;
}

void addSemaphoreById(int pid, sem semaphore) {
    enqueue(theProcessList[pid]->semaphores, semaphore);
}

int removeSemaphoreById(int pid, sem semaphore) {
    return findAndDequeue(theProcessList[pid]->semaphores, semaphore);
}

int setProcessFd(int pid, int fdPosition, int fd){
    if(fdPosition >= FD_COUNT || fdPosition < 0)
        return -1;

    Process aux = theProcessList[pid];
    if(aux == NULL)
        return -1;

    aux->filesDescriptors[fdPosition] = fd;
    return 0;
}

int getProcessFd(int pid, int fdPosition) {
    return theProcessList[pid]->filesDescriptors[fdPosition];
}

int getParentPid(int pid){
    return theProcessList[pid]->ppid;
}

Process getProcessByPid(int pid){
  return theProcessList[pid];
}
