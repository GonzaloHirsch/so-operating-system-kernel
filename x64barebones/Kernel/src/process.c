//
// Created by IR on 2/10/2019.
//

#include "../include/process.h"
#include <stdint.h>

struct ProcessADT{

    int pid;
    int ppid;
    State state;
    uint64_t basePointer;
    uint64_t stackPointer;
    char name[MAX_NAME_LENGTH];
};

