#ifndef SYSCALLS_H
#define SYSCALLS_H

//Incluye a interrupts para poder hacer la llamada a la int 80
#include <interrupts.h>
#include <stdint.h>
#include <utils.h>

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

void sys_write(int fd, const char *buf, int count);

void sys_write_key(int fd, const char * buf);

void sys_get_key(int fd, char *buf);

void sys_over_clock(int rate);

void sys_read(int fd, char *buf, int count);

void sys_sleep(int ticks);

void sys_beep(int freq, int time);

uint64_t sys_get_ticks(void);

void sys_draw_pixel(int x, int y, int r, int g, int b);

void sys_clear_console(void);

int sys_time(int selector);

void sys_shutdown();

#endif
