//
// Created by IR on 2/10/2019.
//

#include "../include/processes.h"
#include <stdint.h>

struct ProcessADT{

    int pid;
    int ppid;
    State state;
    uint64_t basePointer;
    uint64_t stackPointer;
    char name[MAX_NAME_LENGTH];
};

typedef struct ProcessStack{

    uint64_t rip;
    uint64_t cs;
    uint64_t eflags;
    uint64_t rsp;
    uint64_t ss;
    uint64_t base;

    uint64_t gs;
    uint64_t fs;
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t rbp;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;

}ProcessStack;


Process newProcess(char * process_name, ){

    Process aux = (Process)
}