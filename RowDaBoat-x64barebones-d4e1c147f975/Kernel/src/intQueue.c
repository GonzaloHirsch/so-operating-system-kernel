//
// Created by click on 19/10/19.
//

#include "../include/intQueue.h"
#include "../include/memManager.h"

typedef struct IntQueueCDT{

    int head, tail;
    size_t size;
    int queue[];

}IntQueueCDT;

IntQueue newQueue(int size) {
    //Reservo para el tamanio exacto de memoria
    IntQueue aux = mAlloc(sizeof(int)*2 + sizeof(size_t) + (size+1)*sizeof(int));
    aux->size = size +1;
    aux->head = aux->tail = 0;
    return aux;
}

void enqueue(IntQueue q, int value) {
    if(q->head == q->tail+1){
        return;
    }
    else {
        q->queue[q->tail]=value;
        q->tail = (q->tail+1)%(q->size);
    }
}

int dequeue(IntQueue q) {
    if(isEmpty(q)) return 0;
    int aux = q->queue[q->head];
    q->head = (q->head + 1)%(q->size);
    return aux;
}

int isEmpty(IntQueue q) {
    return q->head == q->tail;
}

void freeQueue(IntQueue q){
    mFree((void*) q );
}