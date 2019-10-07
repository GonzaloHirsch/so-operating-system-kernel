//
// Created by nacho on 12/06/19.
//

#ifndef ARQUI_TPE_CONSOLE_H
#define ARQUI_TPE_CONSOLE_H

#include <types.h>
#include <pixelMap.h>
#include <strings.h>
#include <stdarg.h>
#include <videoDriver.h>

void init_console();

void backspace();

void printWithColors(Color chosenForeground, Color chosenBackground, char * str, va_list list);

void print(char * str, ...);

void print_N(const char * src, int length);

void print_char(char c);

void printInteger(uint64_t dec);

void printError(char * str, ...);

void move_line_up(unsigned int line);

void clear_line(unsigned int line);

void move_all_up();

void clear_console();


#endif //ARQUI_TPE_CONSOLE_H
