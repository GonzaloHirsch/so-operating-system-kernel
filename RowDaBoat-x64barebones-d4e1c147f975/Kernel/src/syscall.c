#include <syscall.h>
#include <naiveConsole.h>
#include <stdint.h>
#include <console.h>
#include <processes.h>
#include <scheduler.h>
#include "../include/processes.h"
#include "../include/scheduler.h"
#include "../include/fileDescriptor.h"
#include "../include/pipes.h"
#include "../include/syscall.h"
#include "../include/memManager.h"

extern void forceChangeProcess();
extern void hang();
extern void over_clock(int rate);

void handle_sys_write(int fd, const char * buf, int length);

void handle_sys_read(int fd, char * buf, int length);

void handle_sys_beep(int freq, int time);

void handle_sys_get_ticks(int * ticks);

void handle_sys_sleep(int ticks);

void handle_sys_over_clock(int rate);

int handle_sys_time(uint64_t selector);

void handle_sys_clear_console(void);

void handle_sys_draw_pixel(int x, int y, int r, int g, int b);

int handle_sys_new_process(char *name, void *functionAddress, int priority, enum Visibility isForeground);

void handle_sys_kill_process(int pid);

void handle_sys_change_priority(int pid, int priority);

void handle_sys_block_process(int pid);

void handle_sys_unblock_process(int pid);

const sem * handle_sys_create_sem(char * name);

void handle_sys_post_sem(const sem * semaphore);

void handle_sys_wait_sem(const sem * semaphore);

int handle_sys_create_pipe(char * name);

int handle_sys_set_process_fd(int pid, int fdPosition, int fd);

void handle_sys_print_sem_info();

void handle_sys_print_pipe_info();

void handle_sys_close_sem(sem * semaphore);

void handle_sys_set_sem_value(sem * semaphore, int newVal);

void handle_sys_print_mem_state();

void * handle_sys_malloc(size_t size);

void handle_sys_mfree(void * address);

//Handler de la llamada a la int 80
uint64_t handleSyscall(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9){
    switch(rdi){
		case WRITE:
			handle_sys_write(rsi, (char *)rdx, rcx);
		break;
		case READ:
			handle_sys_read(rsi, (char *)rdx, rcx);
		break;
		case TIME:
			return handle_sys_time(rsi);
		break;
		case BEEP:
			handle_sys_beep(rsi, rdx);
		break;
		case SLEEP:
			handle_sys_sleep(rcx);
		break;
		case OVER_CLOCK:
			handle_sys_over_clock(rsi);
		break;
		case CLEAR:
			handle_sys_clear_console();
		break;
		case DRAW_PIXEL:
			handle_sys_draw_pixel(rsi, rdx, rcx, r8, r9);
		break;
		case TICKS:
			handle_sys_get_ticks((int *)rdx);
		break;
	    case SHUTDOWN:
	        //Metodo facil: Halteo la PC, como seria en la version original
	        //del kernel. Se podria buscar una forma de apagar la PC
	        //realmente, es decir, apagar el hardware.
            hang();
        break;
        case NEW_PROCESS:
            return handle_sys_new_process(rsi, rdx, rcx, r8);
        case GET_PID:
            return handle_sys_get_pid();
        case LIST_PROCESSES:
            handle_sys_list_processes();
        break;
        case KILL_PROCESS:
            handle_sys_kill_process(rsi);
        break;
        case CHANGE_PRIORITY:
            handle_sys_change_priority(rsi, rdx);
        break;
        case BLOCK_PROCESS:
            handle_sys_block_process(rsi);
        break;
        case UNBLOCK_PROCESS:
            handle_sys_unblock_process(rsi);
        break;
        case CREATE_PIPE:
            return handle_sys_create_pipe((char *)rsi);
        case CREATE_SEM:
            return handle_sys_create_sem((char *)rsi);
        case WAIT_SEM:
            handle_sys_wait_sem((const sem *)rsi);
        break;
        case POST_SEM:
            handle_sys_post_sem((const sem *)rsi);
        break;
        case SET_PROCESS_FD:
            return handle_sys_set_process_fd(rsi, rdx, rcx);
        case PRINT_PIPE_INFO:
            handle_sys_print_pipe_info();
        break;
        case PRINT_SEM_INFO:
            handle_sys_print_sem_info();
        break;
        case CLOSE_SEM:
            handle_sys_close_sem(rsi);
        break;
        case SET_SEM_VALUE:
            handle_sys_set_sem_value(rsi, rdx);
        break;
        case PRINT_MEM_STATE:
            handle_sys_print_mem_state();
        break;
        case MALLOC:
            return handle_sys_malloc(rsi);
        case MFREE:
            handle_sys_mfree(rsi);
        break;
    }
	return 0;
}

//Handler para la system write
void handle_sys_write(int fd, const char * buf, int length){
    write(STDOUT_FD,buf,length);
}

//Handler para la system get ticks
void handle_sys_get_ticks(int * ticks){
	*(ticks) = ticks_elapsed();
}

//Handler para la system clear console
void handle_sys_clear_console(void){
	clear_console();
}

//Handler para la system draw pixel
void handle_sys_draw_pixel(int x, int y, int r, int g, int b){
	Vector2 auxPos = {x,y};
	Color auxColor = {r,g,b};
	draw_pixel(auxPos, auxColor);
}

//Handler para la system read
//El fd es el File Descriptor, no lo utilizamos porque no es necesario en nuestro caso
//Esta para que se pueda implementar en el futuro
void handle_sys_read(int fd, char * buf, int length){
    //int pid = getProcessPid(getCurrentProcess());
    //setProcessStateByPid(pid, STATE_BLOCKED);
    read(STDIN_FD,buf,length);
    //setProcessStateByPid(pid, STATE_READY);
}

//Handler para la system SLEEP
//Esta hecho en ticks, para tener mas control del tiempo
void handle_sys_sleep(int ticks){
	timer_wait(ticks);
}

//Handler para la system Beep
//Recibe la frecuencia y el tiempo en ticks que se hace el sonido
void handle_sys_beep(int freq, int time){
	beep(freq, time);
}

//Handler para la system overclock
//Esta deprecada, no se utiliz
//La dejamos para que se haga uso si se necesita, reprograma el PIT
void handle_sys_over_clock(int rate){
	over_clock(rate);
}

//Handler para la system time
//Recibe un selector que se utiliza para saber que registro acceder
int handle_sys_time(uint64_t selector){
	return get_time(selector);
}

int handle_sys_new_process(char *name, void *functionAddress, int priority, enum Visibility isForeground) {
    Process nP = newProcess(name, functionAddress, priority, isForeground);
    //todo mejorar esto
    newPCB(nP);
    return getProcessPid(nP);
}

int handle_sys_get_pid(){
    return getPid();
}

int handle_sys_list_processes(){
    listProcesses();
}

void handle_sys_kill_process(int pid){
    setProcessStateByPid(pid, STATE_TERMINATED);

    if(getPid()==pid){
        forceChangeProcess();
    }


}

void handle_sys_change_priority(int pid, int priority){
    setProcessPriorityByPid(pid, priority);
}

void handle_sys_block_process(int pid) {
    setProcessStateByPid(pid, STATE_BLOCKED);
}

void handle_sys_unblock_process(int pid){
    setProcessStateByPid(pid, STATE_READY);
}

int handle_sys_create_pipe(char * name){
    return pipeFifo(name);
}

const sem *handle_sys_create_sem(char *name) {
    return openSemaphore(name);
}

void handle_sys_post_sem(const sem * semaphore){
    semPost(semaphore);
}

void handle_sys_wait_sem(const sem * semaphore){
    semWait(semaphore);
}

int handle_sys_set_process_fd(int pid, int fdPosition, int fd) {
    return setProcessFd(pid, fdPosition, fd);
}

void handle_sys_print_sem_info() {
    printAllSemaphores();
}

void handle_sys_print_pipe_info() {
       printPipes();
}

void handle_sys_close_sem(sem *semaphore) {
    closeSemaphore(semaphore);
    removeSemaphoreById(getPid(), *semaphore);
}

void handle_sys_set_sem_value(sem * semaphore, int newVal){
    setSemValue(semaphore, newVal);
}

void handle_sys_print_mem_state(){
    //TODO hacer la funcion que te liste todo de la memoria
}

void * handle_sys_malloc(size_t size){
    return mAlloc(size);
}

void handle_sys_mfree(void * address){
    mFree(address);
}