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
// se supone que close cierra el semaforo para un proceso,
// y semUnlink lo "desvincula" completamente del nombre: cualquier
// proceso que use el semaphore pasaria a tener el sem* igual a NULL.
// lo dejo como un todo.
void closeSemaphore(const sem *id);
void semUnlink(char * name);
void semWait(const sem * semaphore);
void semPost(const sem * semaphore);
int semGetValue(const sem * semaphore);
void printAllSemaphores();
void semPostById(int semId);
sem * getSemaphoreById(int id);

#endif //SO_TP2_SEMAPHORE_H
