#include <syscalls.h>
#include "../include/syscalls.h"

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

int sys_read(int fd, char *buf, int count){
	return _int80((uint64_t)READ, (uint64_t)fd, (uint64_t)buf, (uint64_t)count, 0, 0);
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

//Processes - 7 calls

int sys_new_process(char * name, uint64_t functionAddress, int priority, enum Visibility isForeground){
    return _int80(NEW_PROCESS, (uint64_t) name, functionAddress, priority, isForeground, 0);
};

int sys_get_pid(){
    return _int80(GET_PID, 0,0,0,0,0);
}

void sys_list_processes(){
    _int80(LIST_PROCESSES, 0,0,0,0,0);
}

sem * sys_create_sem(const char * name){
		return (sem *)_int80(CREATE_SEM, (uint64_t) name,0,0,0,0);
}

void sys_post_sem(const sem * semaphore){
		_int80(POST_SEM, (uint64_t) semaphore,0,0,0,0);
}

void sys_wait_sem(const sem * semaphore){
		_int80(WAIT_SEM, (uint64_t) semaphore,0,0,0,0);
}

void sys_kill(int pid){
    _int80(KILL_PROCESS, pid, 0, 0, 0, 0);
}

void sys_change_priority(int pid, int priority){
    _int80(CHANGE_PRIORITY, pid, priority, 0, 0, 0);
}

void sys_block(int pid) {
    _int80(BLOCK_PROCESS, pid, 0, 0, 0, 0);
}

void sys_unblock(int pid) {
    _int80(UNBLOCK_PROCESS, pid, 0, 0, 0, 0);
}

int sys_create_pipe(char *name) {
    return _int80(CREATE_PIPE, (uint64_t)name, 0, 0, 0, 0);
}

int sys_set_process_fd(int pid, int fdPosition, int fd) {
    return _int80(SET_PROCESS_FD, pid, fdPosition, fd, 0, 0);
}

void sys_print_pipe_info() {
    _int80(PRINT_PIPE_INFO, 0, 0, 0, 0, 0);
}

void sys_print_sem_info() {
    _int80(PRINT_SEM_INFO, 0, 0, 0, 0, 0);
}

void sys_close_sem(const sem *semaphore) {
    _int80(CLOSE_SEM, (uint64_t)semaphore, 0, 0, 0, 0);
}

void sys_set_sem_value(sem *semaphore, int newVal) {
    _int80(SET_SEM_VALUE, (uint64_t)semaphore, newVal, 0, 0, 0);
}

void sys_print_mem_state(){
    _int80(PRINT_MEM_STATE, 0, 0, 0, 0, 0);
}

void * sys_malloc(size_t size){
    return (void *)_int80(MALLOC, size, 0, 0, 0, 0);
}

void sys_mfree(void *address) {
    _int80(MFREE, (uint64_t)address, 0, 0, 0, 0);
}

int sys_get_p_pid(int pid){
		return _int80(GET_P_PID, pid, 0, 0, 0, 0);
}

int sys_create_process(char * name, uint64_t functionAddress, int priority, enum Visibility isForeground){
	return _int80(CREATE_PROCESS, (uint64_t) name, functionAddress, priority, isForeground, 0);
}

void sys_start_process(int pid){
	_int80(START_PROCESS, pid, 0, 0, 0, 0);
}

void sys_close_fd(int fd) {
    _int80(CLOSE_FD, fd, 0, 0, 0, 0);
}
