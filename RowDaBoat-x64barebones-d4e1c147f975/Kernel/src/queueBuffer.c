
#include <queueBuffer.h>
#include <stddef.h>
#include <strings.h>
#include <memManager.h>


typedef struct QueueBufferCDT{

    int head, tail;
    char buff[MAX_BUFFER_SIZE];
    int size;


}QueueBufferCDT;



QueueBuffer createQueueBuffer(){
    QueueBuffer aux = mAlloc(sizeof(struct QueueBufferCDT));
    aux->head = aux->tail = 0;
    aux->size = 0;
    return aux;
}


int putString(QueueBuffer qB, char * string){
    int size = qB->size, tail = qB->tail, i = 0;
    char * buff = qB->buff;
    while(size < MAX_BUFFER_SIZE){
        buff[tail%MAX_BUFFER_SIZE] = string[i++];
        tail = (tail + 1) % MAX_BUFFER_SIZE;
        size++;
    }

    qB->size = size;
    qB->tail = tail;
    return 0;
    
}

int getString(QueueBuffer qB, char * dst){

    if(!hasNext(qB)) return NULL;
    
    int size = qB->size, head = qB->head, i = 0;
    char * buff = qB->buff;

    while(size > 0){
        dst[i] = buff[head];
        head = (head + 1) % MAX_BUFFER_SIZE;
        i++;size--;
    }

    qB->head = head;
    qB->size = size;

    return 0;
}

int hasNext(QueueBuffer qB){
    return qB->size > 0;
}