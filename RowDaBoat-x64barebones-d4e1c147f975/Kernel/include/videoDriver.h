//
// Created by nacho on 10/06/19.
//

#ifndef ARQUI_TPE_VIDEODRIVER_H
#define ARQUI_TPE_VIDEODRIVER_H

#include <stdint.h>
#include <lib.h>
#include <pixelMap.h>
#include <types.h>

void initVideoDriver(void);

void draw_pixel(Vector2 pos, Color color);
void get_pixel(Vector2 pos, Color* out);

void draw_rect(Vector2 pos, Vector2 size, Color color);
void draw_char_with_background(Vector2 pos, char c, Color foreground, Color background);
void draw_string_with_background(Vector2 pos, char * str, Color foreground, Color background);

int getResX(void);
int getResY(void);
void move_all_lines_up(void);


#endif //ARQUI_TPE_VIDEODRIVER_H
