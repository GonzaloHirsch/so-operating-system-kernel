#ifndef SYSCALL_H
#define SYSCALL_H

#include <stdint.h>
#include <keyboard.h>
#include <time.h>
#include <io_read.h>
#include <sound.h>
#include <console.h>
#include <videoDriver.h>
#include <types.h>
#include <interrupts.h>

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

uint64_t handleSyscall(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);

#endif
