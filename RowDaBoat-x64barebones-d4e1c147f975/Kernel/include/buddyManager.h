#ifndef BUDDY_MANAGER_H
#define BUDDY_MANAGER_H

#define SMALLEST_SIZE 1024 //1mb

#include <stddef.h>
#include <stdint.h>
#include "../include/lib.h"

enum BUDDY_STATE {B_FREE = 1, B_NOT_FREE = 0, B_PARENT = 2};

void * buddyMalloc(size_t newSize);
void initializeBuddyMemory(void * startDir, size_t totalSize);
int buddyFree(void * ptr);




#endif

