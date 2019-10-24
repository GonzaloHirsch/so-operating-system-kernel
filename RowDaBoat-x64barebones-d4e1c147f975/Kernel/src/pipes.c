#include <pipes.h>
#include <fileDescriptor.h>
#include <memManager.h>
#include <lib.h>
#include <strings.h>
#include <queueBuffer.h>

typedef struct pipeCDT{

    char name[MAX_NAME_PIPE];
    int fd;
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
    aux->fd = createFds(PIPE_FD,aux->qb,NULL,NULL); 
    //Asignamos el nombre
    strcpy(aux->name,name);
    //Asignamos el espacio del pipe en el pipelist
    pipeList[firstNull] = aux;

    return aux->fd;

}

int writePipe(int pipeNumber, char * src, int length){
    if(pipeNumber > MAX_PIPES || pipeList[pipeNumber] == NULL)
        return -1;
    
    Pipe pipe = pipeList[pipeNumber];

    return putString(pipe->qb, src);
}

int readPipe(int pipeNumber, char * dest, int length){
    if(pipeNumber > MAX_PIPES || pipeList[pipeNumber] == NULL)
        return -1;
    
    Pipe pipe = pipeList[pipeNumber];

    return readString(pipe->qb, dest);
}













