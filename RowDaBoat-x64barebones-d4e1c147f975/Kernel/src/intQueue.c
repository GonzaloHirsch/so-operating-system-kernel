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
    if(q->head == (q->tail+1)%q->size){
        return;
    }
    else {
        q->queue[q->tail]=value;
        q->tail = (q->tail+1)%(q->size);
    }
}

int dequeue(IntQueue q) {
    if(isEmpty(q)) return -1;
    int aux = q->queue[q->head];
    q->head = (q->head + 1)%(q->size);
    return aux;
}

int peep(IntQueue q){
    if(!isEmpty(q)) return q->queue[q->head];
    return -1;
}

int isEmpty(IntQueue q) {
    return q->head == q->tail;
}


int findAndDequeue(IntQueue q, int value) {
    for (int i = q->head; i != q->tail; i = (i + 1) % q->size) {
        if (q->queue[i] == value) {
            int tmp;
            tmp = q->queue[q->head];
            q->queue[q->head] = value;
            q->queue[i] = tmp;
            return dequeue(q);
        }
    }
    return -1;
}

void freeQueue(IntQueue q){
    mFree((void*) q );
}

int size(IntQueue q) {
    if(q->head <= q->tail){
        return q->tail-q->head;
    }
    else return q->size-q->head + q->tail;
}
