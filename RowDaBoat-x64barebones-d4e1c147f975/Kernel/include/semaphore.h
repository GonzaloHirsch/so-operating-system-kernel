//
// Created by click on 19/10/19.
//

#ifndef SO_TP2_SEMAPHORE_H
#define SO_TP2_SEMAPHORE_H

#define MAX_SEMAPHORE_NAME 64
#define MAX_WAITING_PROCESSES 128
#define MAX_SEMAPHORE_COUNT 256

typedef int sem;    //como dice Tanenbaum, semaphores are a special kind of int
typedef struct SemaphoreCDT * Semaphore;

void initializeSemaphores();

const sem * openSemaphore(char * name);
void closeSemaphore(const sem *id);
void closeSemaphoreById(int semId);
void semUnlink(char * name);
void semWait(const sem * semaphore);
void semPost(const sem * semaphore);
int semGetValue(const sem * semaphore);
void printAllSemaphores();
void semPostById(int semId);
void semWaitById(int semId);
sem * getSemaphoreById(int id);
void setSemValue(const sem * semaphore, int newVal);

#endif //SO_TP2_SEMAPHORE_H
