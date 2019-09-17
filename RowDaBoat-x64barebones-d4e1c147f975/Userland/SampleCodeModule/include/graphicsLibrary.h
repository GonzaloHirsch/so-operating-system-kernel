#ifndef GRAPHICS_LIBRARY_H
#define GRAPHICS_LIBRARY_H

#include <syscalls.h>
#include <types.h>
#include <pixelMap.h>

void drawRect(Vector2 start, Vector2 size, Color color);

void drawPixel(Vector2 pos, Color color);

void drawString(Vector2 pos, char * str, Color foreground, Color background);

void clearScreen(void);

#endif
