#include <fileDescriptor.h>
#include <stdio.h>
#include <stdlib.h>
#include <memManager.h>

typedef struct fileDescriptorCDT{
    void (*function)(char *, int);
    int number;
    int type;

} fileDescriptorCDT;

static fds fileList[MAX_FILE_DESCRIPTORS];
static int highestFileId;

int createFds(int type){
    int i;

    //Buscamos hasta el que halla alguno null
    for(i=0;i<MAX_FILE_DESCRIPTORS && fileList[i] != NULL;i++){}

    if(i>MAX_FILE_DESCRIPTORS){
        return -1;
    }

    fds aux = mAlloc(sizeof(struct fileDescriptorCDT));
    //aux->function = ???
    aux->number = i;
    aux->type = type;

    fileList[i] = aux;

    if(i>highestFileId) highestFileId = i;

    return i;
}


