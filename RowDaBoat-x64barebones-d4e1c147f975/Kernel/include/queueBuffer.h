#ifndef QUEUE_QUEUEBUFFER_H
#define QUEUE_QUEUEBUFFER_H

#define MAX_BUFFER_SIZE 1024

typedef struct QueueBufferCDT * QueueBuffer;

QueueBuffer createQueueBuffer();

int putString(QueueBuffer qB, char * string, int count);

int getString(QueueBuffer qB, char * dst);

int isQueueBufferEmpty(QueueBuffer qB);

#endif //QUEUE_QUEUEBUFFER_H
