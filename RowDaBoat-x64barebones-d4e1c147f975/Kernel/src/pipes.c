#include <pipes.h>
#include <fileDescriptor.h>
#include <memManager.h>
#include <lib.h>
#include <strings.h>
#include <queueBuffer.h>
#include "../include/pipes.h"
#include "../include/queueBuffer.h"
#include "../include/fileDescriptor.h"
#include "../include/memManager.h"
#include "../include/processes.h"
#include "../include/console.h"

extern void forceChangeProcess();

typedef struct pipeCDT{

    char name[MAX_NAME_PIPE];
    int fd;
    int beingAccessed;
    //suponemos que solo 2 procesos usan el mismo pipe, por lo tanto
    // en caso de bloqueo, hay 1 proceso esperando
    int waitingProcess;
    QueueBuffer qb;


}pipeCDT;

static Pipe pipeList[MAX_PIPES] = {NULL};


int pipeFifo(char * name){
    if(strlen(name) > MAX_NAME_PIPE)
        return -1;

    int i, firstNull = -1;
    //Buscamos si existe un pipe con el nombre deseado
    for(i=0;i<MAX_PIPES;i++){
        if(pipeList[i] != NULL && strcmp(name,pipeList[i]->name) == 0)
            return pipeList[i]->fd;

        if(firstNull == -1 && pipeList[i]==NULL)
            firstNull = i;

    }

    //En caso que no exista lo creamos.
    Pipe aux = mAlloc(sizeof(struct pipeCDT));
    //Creamos el queuebuffer donde escribe y lee datos
    aux->qb = createQueueBuffer();
    //Creamos el file descriptor que lo envuelve
    //Le pasamos que es de tipo pipe y el numero de pipe
    aux->fd = createFds(PIPE_FD,firstNull,NULL,NULL);
    //Asignamos el nombre
    strcpy(aux->name,name);
    //Asignamos el espacio del pipe en el pipelist
    pipeList[firstNull] = aux;
    aux->beingAccessed = 0;
    aux->waitingProcess = 0;

    return aux->fd;

}

int writePipe(int pipeNumber, char * src, int count){
    if(pipeNumber > MAX_PIPES || pipeList[pipeNumber] == NULL)
        return -1;

    Pipe pipe = pipeList[pipeNumber];

    //si el pipe esta siendo accedido, bloquear

    if(pipe->beingAccessed){
        int pid = getPid();
        pipe->waitingProcess = pid;
        setProcessStateByPid(pid, STATE_BLOCKED);
        forceChangeProcess();
        //cuando me desbloquean, hago lo siguiente...

        pipe->waitingProcess = 0;
    }
    pipe->beingAccessed = 1;

    int retVal = putString(pipe->qb, src, count);

    pipe->beingAccessed = 0;
    //si hay un proceso esperando para leer, lo desbloqueo

    if(pipe->waitingProcess){
        setProcessStateByPid(pipe->waitingProcess, STATE_READY);
    }

    return retVal;
}

int readPipe(int pipeNumber, char * dest, int count){
    if(pipeNumber > MAX_PIPES || pipeList[pipeNumber] == NULL)
        return -1;

    Pipe pipe = pipeList[pipeNumber];
     //todo bloqueo
    if(pipe->beingAccessed || isQueueBufferEmpty(pipe->qb)){
        int pid = getPid();
        pipe->waitingProcess = pid;
        setProcessStateByPid(getPid(), STATE_BLOCKED);
        forceChangeProcess();
        //cuando me desbloquean, hago lo siguiente...
        pipe->waitingProcess = 0;
    }

    pipe->beingAccessed = 1;

    int retVal = getString(pipe->qb, dest);
    pipe->beingAccessed = 0;
    //si hay un proceso esperando para leer, lo desbloqueo
    if(pipe->waitingProcess){
        setProcessStateByPid(pipe->waitingProcess, STATE_READY);
    }
    return retVal;
}

int freePipe(int pipe){
    if(pipe < 0 || pipe >= MAX_PIPES)
        return -1;

    Pipe aux = pipeList[pipe];
    if(aux == NULL)
        return -1;

    //Liberamos el queue buffer.
    mFree(aux->qb);
    //Liberamos el pipe per se.
    mFree(aux);
    //Ahora no hay nada en su antigua posicion.
    pipeList[pipe] = NULL;

    return 0;


}

void printPipes() {
    for(int i = 0; i<MAX_PIPES; i++){
        if(pipeList[i]!=NULL) {
            Pipe aux = pipeList[i];
            print("Pipe %d\n    Name: %s\n    Being Accessed: %d\n    Waiting Process:%d\n", aux->fd, aux->name, aux->beingAccessed, aux->waitingProcess);
        }

    }
}
