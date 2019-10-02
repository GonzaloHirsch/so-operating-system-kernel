#ifndef SOUND_H
#define SOUND_H

#include <stdint.h>
#include <io_read.h>
#include <time.h>

void play_sound(uint32_t nFrequence);
void nosound();
 void beep(int freq, int time);

#endif
