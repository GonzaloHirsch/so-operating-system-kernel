#ifndef PIPES_H
#define PIPES_H

#include <stdint.h>
#include <stddef.h>

typedef struct pipeCDT * Pipe;

int pipeFifo(char * name);
int writePipe(int pipeNumber, char * src, int length);
int readPipe(int pipeNumber, char * dest, int length);

#define MAX_PIPES 100
#define MAX_NAME_PIPE 100

#endif
