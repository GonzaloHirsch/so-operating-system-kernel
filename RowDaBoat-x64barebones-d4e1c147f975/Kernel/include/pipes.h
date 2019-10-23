#ifndef PIPES_H
#define PIPES_H

#include <stdint.h>
#include <stddef.h>

typedef pipeCDT * Pipe;

int pipe(char* name, int* fds);

#define MAX_PIPES 100

#endif
