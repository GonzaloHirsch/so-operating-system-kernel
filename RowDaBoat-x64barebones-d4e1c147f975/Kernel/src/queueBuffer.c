
#include <queueBuffer.h>
#include <stddef.h>
#include <strings.h>
#include <memManager.h>
#include "../include/queueBuffer.h"
#include "../include/memManager.h"


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


int putString(QueueBuffer qB, char * string, int count){
    int i = 0;

    while(qB->size < MAX_BUFFER_SIZE-1 && i < count && string[i] != 0){
        qB->buff[qB->tail%MAX_BUFFER_SIZE] = string[i++];
        qB->tail = (qB->tail + 1) % MAX_BUFFER_SIZE;
        qB->size++;
    }

    qB->buff[qB->tail] = 0;
    qB->tail = (qB->tail+1)%MAX_BUFFER_SIZE;

    return 0;
}

int getString(QueueBuffer qB, char * dst){

    int i = 0;

    //Iteraremos hasta encontrar el 0 o llegar a la cantidad de count
    while((qB->buff[qB->head] != -1 || i==0)){

        //Si no hay nada para leer, esperaremos.
        if(qB->size > 0 && qB->buff[qB->head] != -1){
            dst[i] = qB->buff[qB->head];
            qB->head = (qB->head + 1) % MAX_BUFFER_SIZE;
            i++;qB->size--;
        }
        //Esto es para abarcar el caso en que manda un 0 solamente.
        else if(qB->size > 0 && qB->buff[qB->head] == -1){
            qB->size--;
            break;
        }
    }

    qB->head = (qB->head + 1)%MAX_BUFFER_SIZE;
    dst[i] = 0;

    return 0;
}


int isQueueBufferEmpty(QueueBuffer qB){
   return qB->size==0;
}
