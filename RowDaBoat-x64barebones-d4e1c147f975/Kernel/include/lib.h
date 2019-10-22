#ifndef LIB_H
#define LIB_H

#include <stdint.h>


void  strcpy(char * dst, char * src);
int strcmp( char * s1,  char * s2);
void *memset(void * destination, int32_t character, uint64_t length);
void *memcpy(void * destination, const void * source, uint64_t length);
char *itoa(int value, char* buffer, int base);


char *cpuVendor(char *result);
int getSeconds();
int getKeyboard();



#endif