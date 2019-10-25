#ifndef MEM_MANAGER_H
#define MEM_MANAGER_H

#include <stdint.h>
#include <stddef.h>

//Metodos genericos
void * mAlloc(size_t size);
int mFree(void * ptr);
void initializeMemManager(void * startDir, size_t totalSize);

#define FIRST_FIT_MANAGER 0
#define BUDDY_MANAGER 1

//Todo de first fit

enum STATE {FREE = 1, NOT_FREE = 0};

typedef struct t_node * Node;

//Metodos de first fit.
void * memMalloc(size_t size);
int memFree(void * ptr);
void initializeMemManagerList(void * startDir, size_t totalSize);


#endif
