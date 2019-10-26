//
// Created by click on 19/10/19.
//

#include "../include/semaphore.h"
#include "../include/memManager.h"
#include "../include/intQueue.h"
#include "../include/scheduler.h"
#include "../include/console.h"
#include "../include/lib.h"
#include "../include/interrupts.h"

extern forceChangeProcess();

typedef struct SemaphoreCDT{

    sem semId;
    char name[MAX_SEMAPHORE_NAME];
    int value;

    IntQueue waitingProcesses;
    int attachedProcessCount;
}SemaphoreCDT;


static Semaphore theSemaphoreList[MAX_SEMAPHORE_COUNT];
static int highestSemId;


void initializeSemaphores() {
    for(int i = 0; i<MAX_SEMAPHORE_COUNT; i++){
        theSemaphoreList[i] = NULL;
    }
    highestSemId = 0;
};

const sem * openSemaphore(char *name) {

    //si el semaforo esta, devolver puntero a semId
    int i;
    for(i = 0; i<MAX_SEMAPHORE_COUNT; i++){
        if(theSemaphoreList[i] != NULL && strcmp(name, theSemaphoreList[i]->name) == 0){
            theSemaphoreList[i]->attachedProcessCount++;
            addSemaphoreById(getPid(), i);
            return &theSemaphoreList[i]->semId;
        }
    }

    //si no, busco el menor slot nulo para asignarle un nuevo semaphore... Es O(n) pero es mas prolijo que tener
    //un contador global que nunca se resettea...
    for(i = 0; i<MAX_SEMAPHORE_COUNT && theSemaphoreList[i]!=NULL; i++);
    Semaphore aux = mAlloc(sizeof(struct SemaphoreCDT));
    aux->semId = i;
    if(i>highestSemId) highestSemId = i;
    strcpy(aux->name, name);
    aux->value = 1;
    aux->waitingProcesses = newQueue(MAX_WAITING_PROCESSES);
    aux->attachedProcessCount=1;
    addSemaphoreById(getPid(), i);
    theSemaphoreList[i] = aux;
    return &aux->semId;
}

int semGetValue(const sem *semaphore) {
    return theSemaphoreList[*semaphore]->value;
}

void semWait(const sem *semaphore) {
    //_cli();
    if(theSemaphoreList[*semaphore]->value <= 0 || !isEmpty(theSemaphoreList[*semaphore]->waitingProcesses)){

        setProcessState(getCurrentProcess(), STATE_BLOCKED);
        int pid = getProcessPid(getCurrentProcess());
        enqueue(theSemaphoreList[*semaphore]->waitingProcesses, pid);
        forceChangeProcess();
        theSemaphoreList[*semaphore]->value--;
        dequeue(theSemaphoreList[*semaphore]->waitingProcesses);
    }

    //print("decreasing semaphore, current process: %s\n", getProcessName(getCurrentProcess()));
    else{theSemaphoreList[*semaphore]->value--;}
    //_sti();
    //print("current semaphore value: %d\n", theSemaphoreList[*semaphore]->value);
}

void semPost(const sem *semaphore) {
    //_cli();
    Semaphore aux = theSemaphoreList[*semaphore];
    if(!isEmpty(aux->waitingProcesses)){
        int pid = peep(aux->waitingProcesses);
        setProcessStateByPid(pid, STATE_READY);
        aux->value++;
        //forceChangeProcess();
    }

    //print("increasing semaphore, current process: %s\n", getProcessName(getCurrentProcess()));
    else{aux->value++;}
    //_sti();
    //print("current semaphore value: %d\n", theSemaphoreList[*semaphore]->value);
}

void printAllSemaphores() {
    Semaphore aux;
    for(int i = 0; i<highestSemId; i++){
        aux = theSemaphoreList[i];
        print("Semaphore %s\n    Value: %d\n", aux->name, aux->value);
    }
}

static void removeSemaphore(const sem * id){
    Semaphore aux = theSemaphoreList[*id];
    theSemaphoreList[*id] = NULL;
    freeQueue(aux->waitingProcesses);
    mFree((void*) id);
}

void closeSemaphore(const sem *id) {

    removeSemaphoreById(getPid(), *id);
    theSemaphoreList[*id]->attachedProcessCount--;
    if(theSemaphoreList[*id]->attachedProcessCount==0){
        removeSemaphore(id);
    }
}

void semPostById(int semId) {
    semPost(&theSemaphoreList[semId]->semId);
}

sem *getSemaphoreById(int id) {
    return &theSemaphoreList[id]->semId;
}

void semWaitById(int semId) {
    semWait(&theSemaphoreList[semId]->semId);
}

void closeSemaphoreById(int semId) {
    closeSemaphore(theSemaphoreList[semId]);
}

