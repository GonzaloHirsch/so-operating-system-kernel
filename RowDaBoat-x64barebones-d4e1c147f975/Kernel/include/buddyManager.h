#ifndef BUDDY_MANAGER_H
#define BUDDY_MANAGER_H

#define SMALLEST_SIZE 1024 //1mb
enum BUDDY_STATE {B_FREE = 1, B_NOT_FREE = 0, B_PARENT = 2};

void * buddyMalloc(size_t newSize);
void initializeBuddyMemory(void * startDir, size_t totalSize);

size_t powerTo(size_t value, size_t pow);
size_t logBase2Ceil(size_t value);



#endif

