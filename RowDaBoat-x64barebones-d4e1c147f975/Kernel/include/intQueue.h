//
// Created by click on 19/10/19.
//

#ifndef SO_TP2_INTQUEUE_H
#define SO_TP2_INTQUEUE_H

#include <stdint.h>

typedef struct IntQueueCDT * IntQueue;

IntQueue newQueue(int size);
void enqueue(IntQueue q, int value);
int dequeue(IntQueue q);
int isEmpty(IntQueue q);
void freeQueue(IntQueue q);

#endif //SO_TP2_INTQUEUE_H

