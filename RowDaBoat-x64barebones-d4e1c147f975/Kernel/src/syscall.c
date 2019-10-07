#include <syscall.h>
#include <naiveConsole.h>
#include <stdint.h>
#include <console.h>

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
	}
	return 0;
}

//Handler para la system write
void handle_sys_write(int fd, const char * buf, int length){
    print_N(buf, length);
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
	for (int i = 0; i < length; i++){
		*(buf + i) = getChar();
	}
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
