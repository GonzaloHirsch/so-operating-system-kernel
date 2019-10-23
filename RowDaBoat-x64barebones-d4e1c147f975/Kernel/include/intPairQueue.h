//
// Created by click on 23/10/19.
//

#ifndef SO_TP2_INTPAIRQUEUE_H
#define SO_TP2_INTPAIRQUEUE_H

typedef struct IntPairQueueCDT * IntPairQueue;

struct intPair{
    int first, second;
};

IntPairQueue newIntPairQueue(int size);
void enqueuePair(IntPairQueue q, int key, int value);

struct intPair dequeuePair(IntPairQueue q);
struct intPair peepPair(IntPairQueue q);
struct intPair findAndDequeuePair(IntPairQueue q, int value);

int getValue(IntPairQueue q, int key);
void setValue(IntPairQueue q, int key, int newValue);

int isIntPairQueueEmpty(IntPairQueue q);

void freeIntPairQueue(IntPairQueue q);
int intPairQueueSize(IntPairQueue q);



#endif //SO_TP2_INTPAIRQUEUE_H
