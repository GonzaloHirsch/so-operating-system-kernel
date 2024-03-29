#ifndef SYSCALL_H
#define SYSCALL_H

#include <stdint.h>
#include "keyboard.h"
#include "time.h"
#include "io_read.h"
#include "sound.h"
#include "console.h"
#include "videoDriver.h"
#include "types.h"
#include "interrupts.h"
#include "semaphore.h"


#define READ 1
#define WRITE 2
#define BEEP 3
#define TICKS 4
#define TIME 5
#define SLEEP 6
#define DATE 7
#define CLEAR 8
#define DRAW_PIXEL 9
#define OVER_CLOCK 10
#define SHUTDOWN 11
#define NEW_PROCESS 12
#define GET_PID 13
#define LIST_PROCESSES 14
#define KILL_PROCESS 15
#define CHANGE_PRIORITY 16
#define BLOCK_PROCESS 17
#define UNBLOCK_PROCESS 18
#define CREATE_SEM 19
#define POST_SEM 20
#define WAIT_SEM 21
#define CREATE_PIPE 22
#define SET_PROCESS_FD 23
#define PRINT_PIPE_INFO 24
#define PRINT_SEM_INFO 25
#define CLOSE_SEM 26
#define SET_SEM_VALUE 27
#define PRINT_MEM_STATE 28
#define MALLOC 29
#define MFREE 30
#define GET_P_PID 31
#define CREATE_PROCESS 32
#define START_PROCESS 33
#define CLOSE_FD 34

uint64_t handleSyscall(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);

#endif
