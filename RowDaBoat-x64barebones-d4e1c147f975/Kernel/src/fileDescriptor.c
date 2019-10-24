#include <fileDescriptor.h>
#include <memManager.h>
#include <pipes.h>
#include <console.h>
#include <keyboard.h>

typedef struct fileDescriptorCDT{
    int number;
    int type;
    int pipe; //Si es del tipo pipe, es decir usa un queue buffer. No me parecio bien poner el queueBuffer directamente, que se ocupe el pipe
    void (*altRead)(char *, int);
    void (*altWrite)(char *, int);
} fileDescriptorCDT;

static fds fileList[MAX_FILE_DESCRIPTORS] = {NULL};
static int highestFileId;

int createFds(int type, int pipe, void (*altRead)(char *, int),void (*altWrite)(char *, int)){
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

int read(int fd, char * dest, int count){
    if(fd > MAX_FILE_DESCRIPTORS || fileList[fd] == NULL)
        return -1;

    fds fileDesc = fileList[fd];

    if(fileDesc->type == STD_FD){
        //Uso el que estaba antes en la syscalls..
        for (int i = 0; i < count; i++){
		    *(dest + i) = getChar();
	    }
    }
    else if(fileDesc->type == PIPE_FD){
        return readPipe(fileDesc->pipe, dest, count);
    }
    else if(fileDesc->type == ALT_FD){
        (*fileDesc->altRead)(dest,count);
    }
    else{
        return -1;
    }

    return 0;

   
}

int write(int fd, char * src, int count){
    if(fd > MAX_FILE_DESCRIPTORS || fileList[fd] == NULL)
        return -1;

    fds fileDesc = fileList[fd];

    if(fileDesc->type == STD_FD){
        //Uso el que estaba en syscalls
        print_N(src, count);
    }
    else if(fileDesc->type == PIPE_FD){
        return writePipe(fileDesc->pipe, src, count);
    }
    else if(fileDesc->type == ALT_FD){
        (*fileDesc->altWrite)(src,count);
    }
    else{
        return -1;
    }

    return 0;

}







