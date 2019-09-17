#include <sound.h>
#include <stdint.h>
#include <naiveConsole.h>
#include <time.h>

/*
  Este codigo sale de OSDEV
  Link: https://wiki.osdev.org/PC_Speaker
*/

void play_sound(uint32_t nFrequence) {
	uint32_t Div;
	uint8_t tmp;

//Se setea el PIT  a la frecuencia deseada
	Div = 1193180 / nFrequence;
	write_port(0x43, 0xb6);
	write_port(0x42, (uint8_t) (Div) );
	write_port(0x42, (uint8_t) (Div >> 8));

//Se reproduce el sonido usando el parlante de la PC
	tmp = read_port(0x61);
	if (tmp != (tmp | 3)) {
    write_port(0x61, tmp | 3);
	}
 }

 //Se hace terminar el ruido
void nosound() {
 	uint8_t tmp = read_port(0x61) & 0xFC;
 	write_port(0x61, tmp);
 }

 //Se emite un beep
 void beep(int freq, int time) {
 	 play_sound(freq);	//800 esta buena
 	 sleep(time);		//4 ticks, son como 220ms
 	 nosound();
 }
