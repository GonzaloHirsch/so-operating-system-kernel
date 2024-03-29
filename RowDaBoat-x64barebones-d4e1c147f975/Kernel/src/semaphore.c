#include "../include/semaphore.h"
#include "../include/memManager.h"
#include "../include/intQueue.h"
#include "../include/scheduler.h"
#include "../include/console.h"
#include "../include/lib.h"
#include "../include/interrupts.h"
#include "../include/fileDescriptor.h"

extern void forceChangeProcess();

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
    if(theSemaphoreList[*semaphore]->value <= 0 || !isEmpty(theSemaphoreList[*semaphore]->waitingProcesses)){

        setProcessState(getCurrentProcess(), STATE_BLOCKED);
        int pid = getProcessPid(getCurrentProcess());
        enqueue(theSemaphoreList[*semaphore]->waitingProcesses, pid);
        forceChangeProcess();
        theSemaphoreList[*semaphore]->value--;
        dequeue(theSemaphoreList[*semaphore]->waitingProcesses);
    }

    else{theSemaphoreList[*semaphore]->value--;}
}

void semPost(const sem *semaphore) {
    Semaphore aux = theSemaphoreList[*semaphore];
    if(!isEmpty(aux->waitingProcesses)){
        int pid = peep(aux->waitingProcesses);
        setProcessStateByPid(pid, STATE_READY);
        aux->value++;
    }

    else{aux->value++;}
}

void printAllSemaphores() {
    Semaphore aux;
    if(getProcessCount()==0) {
        for (int i = 0; i < highestSemId; i++) {
            aux = theSemaphoreList[i];
            print("Semaphore %s\n    Value: %d\nWaiting Processes: ", aux->name, aux->value);
            printQueue(aux->waitingProcesses);
        }
    }
    else{
        char buffer[10];
        for (int i = 0; i < highestSemId; i++) {
            aux = theSemaphoreList[i];
            write(1, "Semaphore ", strlen("Semaphore "));
            write(1, aux->name, strlen(aux->name));
            write(1, "\n    Value: ", strlen("\n    Value: "));
            itoa(aux->value, buffer, 10);
            write(1, buffer, strlen(buffer));
            printQueue(aux->waitingProcesses);
        }
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

void closeSemaphoreById(int semId) {
    closeSemaphore((sem *)theSemaphoreList[semId]);
}

void setSemValue(const sem * semaphore, int newVal){
    ((Semaphore)semaphore)->value = newVal;
}
