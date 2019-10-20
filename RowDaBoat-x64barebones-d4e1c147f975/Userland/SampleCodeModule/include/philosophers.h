#ifndef PHILOSOPHERS_H
#define PHILOSOPHERS_H

#include <utils.h>
#include <syscalls.h>

#define BASE_PHILOSOPHER_COUNT = 5
#define MAX_PHILOSOPHER_COUNT = 10
#define HUNGRY = 0
#define EATING = 1
#define THINKING = 2

enum Visibility{FOREGROUND, BACKGROUND};

#endif //SO_TP2_PROCESSES_H
