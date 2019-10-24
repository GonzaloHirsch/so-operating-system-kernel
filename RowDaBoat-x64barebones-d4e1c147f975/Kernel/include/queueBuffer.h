#ifndef QUEUE_QUEUEBUFFER_H
#define QUEUE_QUEUEBUFFER_H

#define MAX_BUFFER_SIZE 500

typedef struct QueueBufferCDT * QueueBuffer;

QueueBuffer createQueueBuffer();

int putString(QueueBuffer qB, char * string);

int getString(QueueBuffer qB, char * dst);

int hasNext(QueueBuffer qB);

#endif //QUEUE_QUEUEBUFFER_H
