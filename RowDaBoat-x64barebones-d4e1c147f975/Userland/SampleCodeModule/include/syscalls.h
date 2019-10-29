#ifndef SYSCALLS_H
#define SYSCALLS_H

//Incluye a interrupts para poder hacer la llamada a la int 80
#include "interrupts.h"
#include <stdint.h>
#include <stddef.h>
#include "utils.h"
#include "../include/processes.h"
#include "../include/semaphores.h"

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

void sys_write(int fd, const char *buf, int count);

sem * sys_create_sem(const char * name);

void sys_post_sem(const sem * semaphore);

void sys_wait_sem(const sem * semaphore);

void sys_print_sem_info();

void sys_close_sem(const sem* semaphore);

void sys_write_key(int fd, const char * buf);

void sys_get_key(int fd, char *buf);

void sys_over_clock(int rate);

int sys_read(int fd, char *buf, int count);

void sys_sleep(int ticks);

void sys_beep(int freq, int time);

uint64_t sys_get_ticks(void);

void sys_draw_pixel(int x, int y, int r, int g, int b);

void sys_clear_console(void);

int sys_time(int selector);

void sys_shutdown();

int sys_new_process(char * name, uint64_t functionAddress, int priority, enum Visibility isForeground);

int sys_get_pid();

void sys_list_processes();

void sys_kill(int pid);

void sys_change_priority(int pid, int priority);

void sys_block(int pid);

void sys_unblock(int pid);

int sys_create_pipe(char * name);

int sys_set_process_fd(int pid, int fdPosition, int fd);

void sys_print_pipe_info();

void sys_set_sem_value(sem * semaphore, int newVal);

void sys_print_mem_state();

void * sys_malloc(size_t size);

void sys_mfree(void * address);

int sys_get_p_pid(int pid);

int sys_create_process(char * name, uint64_t functionAddress, int priority, enum Visibility isForeground);

void sys_start_process(int pid);

void sys_close_fd(int fd);

#endif
