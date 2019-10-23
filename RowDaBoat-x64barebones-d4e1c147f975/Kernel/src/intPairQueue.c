//
// Created by click on 23/10/19.
//

#include "../include/intPairQueue.h"
#include "../include/memManager.h"

static int search(IntPairQueue q, int key);

typedef struct IntPairQueueCDT{

    int head, tail;
    size_t size;
    struct intPair queue[];

}IntPairQueueCDT;

IntPairQueue newIntPairQueue(int size) {
    //Reservo para el tamanio exacto de memoria
    IntPairQueue aux = mAlloc(sizeof(int)*2 + sizeof(size_t) + (size+1)*sizeof(struct intPair));
    aux->size = size +1;
    aux->head = aux->tail = 0;
    return aux;
}

void enqueuePair(IntPairQueue q, int key, int value) {
    if(q->head == (q->tail+1)%q->size){
        return;
    }
    else {
        q->queue[q->tail].first=key;
        q->queue[q->tail].second=value;
        q->tail = (q->tail+1)%(q->size);
    }
}

struct intPair dequeuePair(IntPairQueue q) {
    if(isIntPairQueueEmpty(q)) return ((struct intPair){.first=-1, .second=-1});
    q->head = (q->head + 1)%(q->size);
    return q->queue[q->head];
}

struct intPair peepPair(IntPairQueue q){
    if(!isIntPairQueueEmpty(q)) return q->queue[q->head];
    return ((struct intPair){.first=-1,.second=-1});
}

static int search(IntPairQueue q, int key){
    for (int i = q->head; i != q->tail; i = (i + 1) % q->size) {
        if (q->queue[i].first == key) {
            return i;
        }
    }
    return -1;
}

struct intPair findAndDequeuePair(IntPairQueue q, int value) {
    int i;
    if((i=search(q, value))==-1)
        return (struct intPair){-1, -1};

    struct intPair tmp;
    tmp = q->queue[q->head];
    q->queue[q->head] = q->queue[i];
    q->queue[i] = tmp;
    return dequeuePair(q);


}

int isIntPairQueueEmpty(IntPairQueue q) {
    return q->head == q->tail;
}

void freeIntPairQueue(IntPairQueue q){
    mFree((void*) q );
}

int intPairQueueSize(IntPairQueue q) {
    if(q->head <= q->tail){
        return q->tail-q->head;
    }
    else return q->size - q->head + q->tail;
}

int getValue(IntPairQueue q, int key){
    int i;
    if((i=search(q, key))!=-1){
        return q->queue[i].second;
    }
    return -1;
}

void setValue(IntPairQueue q, int key, int newValue){
    int i;
    if((i=search(q, key))!=-1){
        q->queue[i].second = newValue;
    }
}