#ifndef KEYBOARD_H
#define KEYBOARD_H

#define BREAK_CODE_DIF          128

//En mi compu es 170 el left_shift
#define LEFT_SHIFT              130
//En mi compu es 182 el right_shift
#define RIGHT_SHIFT             131
#define CAPS_LOCK               132
#define UP_ARROW                133
#define DOWN_ARROW              134
#define LEFT_ARROW              135
#define RIGHT_ARROW             136

#define EOF 0
#define MAX_BUFFER_SIZE 750

void printKey(void);
int addChar(char c);
int getChar(void);
void keyboard_handler(void);

int isAlpha(char c);
int isNumeric(char c);
int isArrow(char c);

#endif
