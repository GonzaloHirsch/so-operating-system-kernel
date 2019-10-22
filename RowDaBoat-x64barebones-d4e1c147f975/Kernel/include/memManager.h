#ifndef MEM_MANAGER_H
#define MEM_MANAGER_H

#include <stdint.h>
#include <stddef.h>

enum STATE {FREE = 1, NOT_FREE = 0};

typedef struct t_node * Node;

void * mAlloc(size_t size);
int mFree(void * ptr);
void initializeMemManagerList(void * startDir, size_t totalSize);


#endif
