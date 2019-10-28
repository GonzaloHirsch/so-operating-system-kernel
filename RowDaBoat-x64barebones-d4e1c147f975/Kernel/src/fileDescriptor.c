#include <fileDescriptor.h>
#include <memManager.h>
#include <pipes.h>
#include <console.h>
#include <keyboard.h>
#include <stddef.h>
#include "../include/fileDescriptor.h"
#include "../include/keyboard.h"
#include "../include/pipes.h"
#include "../include/console.h"
#include "../include/memManager.h"
#include "../include/semaphore.h"
#include "../include/readerDaemon.h"
#include "../include/processes.h"


typedef struct fileDescriptorCDT{
    int number;
    int type;
    int pipe; //Si es del tipo pipe, es decir usa un queue buffer. No me parecio bien poner el queueBuffer directamente, que se ocupe el pipe
    void (*altRead)(char *, int);
    void (*altWrite)(char *, int);
} fileDescriptorCDT;

static fds fileList[MAX_FILE_DESCRIPTORS] = {NULL};
static int highestFileId;



int createFds(enum FdType type, int pipe, void (*altRead)(char *, int),void (*altWrite)(char *, int)){
    int i;

    //Buscamos hasta el que halla alguno null
    for(i=0;i<MAX_FILE_DESCRIPTORS && fileList[i] != NULL;i++){}

    if(i>MAX_FILE_DESCRIPTORS)
        return -1;
    if(i>highestFileId) highestFileId = i;

    fds aux = mAlloc(sizeof(struct fileDescriptorCDT));
    aux->type = type;
    aux->number = i;
    aux->pipe = pipe;
    aux->altRead = altRead;
    aux->altWrite = altWrite;

    fileList[i] = aux;

    return i;
}

int createDefaultFds(){
     createFds(STDIN_FD,0,NULL,NULL);
     createFds(STDOUT_FD,0,NULL,NULL);
}

int read(int fd, char * dest, int count){
    if(fd > MAX_FILE_DESCRIPTORS || fileList[fd] == NULL)
        return -1;

    fds fileDesc = fileList[fd];
    int pfd;

    switch (fileDesc->type)
    {
    case STDIN_FD:

        if((pfd=getProcessFd(getPid(), STDIN_FD)) == 0) {
            for (int i = 0; i < count; i++) {
                *(dest + i) = getChar();
            }
        }
        else{
            return read(pfd, dest, count);
        }

        //Uso el que estaba antes en la syscalls..


        /*
        //primer semaforo: para evitar concurrencia al settear length y buffer
        semWait(getSemaphoreById(0));   //los semId s 0 y 1 son reservados para el readerDaemon
            setCurrentLength(count);
            setCurrentBuffer(dest);
            //segundo semaforo: bloquea el proceso actual,
            wakeUpDaemon();
            semWait(getSemaphoreById(1));
        */
         break;
    case PIPE_FD:
        return readPipe(fileDesc->pipe, dest, count);
        break;
    case ALT_FD:
        (*fileDesc->altRead)(dest,count);
        break;
    default:
        return -1;
    }
    return 0;

   
}

int write(int fd, char * src, int count){
    if(fd > MAX_FILE_DESCRIPTORS || fileList[fd] == NULL)
        return -1;

    fds fileDesc = fileList[fd];
    int pfd;

    switch (fileDesc->type)
    {
    case STDOUT_FD:
        if((pfd=getProcessFd(getPid(), STDOUT_FD)) == 1)
            print_N(src, count);
        else{
            return write(pfd, src, count);
        }
        break;
    case PIPE_FD:
        return writePipe(fileDesc->pipe, src, count);
        break;
    case ALT_FD:
        (*fileDesc->altWrite)(src,count);
        break;
    default:
        return -1;
    }

    return 0;

}

int freeFd(int fd){

    //El file descriptor 0 y 1 no se van a poder liberar.
    if(fd <= 1 || fd > MAX_FILE_DESCRIPTORS)
        return -1;

    fds aux = fileList[fd];
    if(aux == NULL)
        return -1;

    //Si es un pipe liberamos el pipe
    if(aux->type == PIPE_FD){
        freePipe(aux->pipe);
    }

    //Ese fd ya no esta ocupado.
    fileList[fd] = NULL;
    //Liberamos el espacio del fd per se.
    return mFree(aux);
}

void closeFd(int fd) {
    fds aux = fileList[fd];
    if(aux->type == PIPE_FD){
        closePipe(aux->pipe);
    }
}







