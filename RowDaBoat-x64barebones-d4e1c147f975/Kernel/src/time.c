#include <stdint.h>
#include <time.h>
#include <naiveConsole.h>
#include <interrupts.h>

static unsigned long ticks = 0;

static int selector_to_register[] = {SECONDS_REGISTER, MINUTES_REGISTER, HOURS_REGISTER, WEEKDAY_REGISTER,
  DAY_OF_MONTH_REGISTER, MONTH_REGISTER, YEAR_REGISTER, CENTURY_REGISTER};

void timer_handler();
int ticks_elapsed();

void timer_handler() {
  ticks++;
}

int ticks_elapsed(){
	return ticks;
}

/*
max = 1193182 / 1 = 1193182 hz
min = 1193182 / 65536 ≈ 18.2065 hz
freq = 1193182 / div
*/
void over_clock(int div){
  //Verifica que no se este pasando
  if (1 <= div && div <= 65356){
    _over_clock(div);
  }
}

void set_time(){
  write_port(0x70, 0x0B);
  uint8_t aux = read_port(0x71);
  aux = aux | 0x04;
  write_port(0x71, aux);
}

int get_time(int selector){
  _cli();
  set_time();
  write_port(0x70, selector_to_register[selector]);
  int aux = read_port(0x71);
  _sti();
  return aux;
}

void sleep(uint64_t millis){
    _sti();
    int x = ticks_elapsed();
    while((ticks_elapsed()-x)*REGULAR_TICK_TIME < millis){
       halt();
    }
}

void timer_wait(int expectedTicks){
  //Prepara para que pueda recibir iterrupciones
  _sti();
  expectedTicks += ticks;
  while (ticks < expectedTicks){
    //Le dice que puede ser interrumpido
    halt();
  }
}
