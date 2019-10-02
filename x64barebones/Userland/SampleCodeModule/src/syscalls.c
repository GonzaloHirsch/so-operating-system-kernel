#include <syscalls.h>

// ----------------------------------------------------------------------------------
// Este modulo es el modulo de Syscalls
// Llamadas a la int 80
// Aca se agregan todas las llamadas que va a usar Userland,
// para que el usuario no tenga acceso directo
// desde el shell a estas llamadas
// ----------------------------------------------------------------------------------

void sys_write(int fd, const char *buf, int count){
	_int80((uint64_t)WRITE, (uint64_t)fd, (uint64_t)buf, (uint64_t)count, 0, 0);
}

void sys_read(int fd, char *buf, int count){
	_int80((uint64_t)READ, (uint64_t)fd, (uint64_t)buf, (uint64_t)count, 0, 0);
}

void sys_get_key(int fd, char * buf){
	_int80((uint64_t)READ, (uint64_t)fd, (uint64_t)buf, (uint64_t)1, 0, 0);
}

void sys_write_key(int fd, const char * buf){
	_int80((uint64_t)WRITE, (uint64_t)fd, (uint64_t)buf, (uint64_t)1, 0, 0);
}

void sys_beep(int freq, int time){
	_int80((uint64_t)BEEP, (uint64_t)freq, (uint64_t)time, 0, 0, 0);
}

int sys_time(int selector){
	uint64_t time = _int80((uint64_t)TIME, (uint64_t)selector, 0, 0, 0, 0);
	return time;
}

void sys_over_clock(int rate){
	_int80((uint64_t)OVER_CLOCK, (uint64_t)rate, 0, 0, 0, 0);
}

void sys_sleep(int ticks){
	_int80((uint64_t)SLEEP, 0, 0, (uint64_t)ticks, 0, 0);
}

void sys_draw_pixel(int x, int y, int r, int g, int b){
	_int80((uint64_t)DRAW_PIXEL, (uint64_t)x, (uint64_t)y, (uint64_t)r, (uint64_t)g, (uint64_t)b);
}

void sys_clear_console(void){
	_int80((uint64_t)CLEAR, 0, 0, 0, 0, 0);
}

uint64_t sys_get_ticks(void){
	int ticks;
	_int80((uint64_t)TICKS, 0, (uint64_t)&ticks, 0, 0, 0);
	return ticks;
}

void sys_shutdown(){
    _int80(SHUTDOWN, 0, 0, 0, 0, 0);
}
