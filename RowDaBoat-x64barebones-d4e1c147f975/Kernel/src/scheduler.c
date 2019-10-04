//
// Created by IR on 2/10/2019.
//



typedef struct ProcessControlBlockCDT{

    Process process;

    struct ProcessControlBlockCDT * prev;
    struct ProcessControlBlockCDT * next;

} PCBCDT;

