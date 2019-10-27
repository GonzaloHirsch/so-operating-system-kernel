#ifndef APPS_H
#define APPS_H

#include "utils.h"

//Constantes para los comandos
#define INVALID_COMMAND -1
#define HELP_COMMAND 0
#define SNAKE_COMMAND 1
#define SHUTDOWN_COMMAND 2
#define INVALID_OC_COMMAND 3
#define TIME_COMMAND 4
#define BEEP_COMMAND 5
#define SLEEP_COMMAND 6
#define DATE_COMMAND 7
#define CLEAR_COMMAND 8
#define DIV_COMMAND 9
#define CREDITS_COMMAND 10
#define STARWARS_COMMAND 11
#define MARIO_COMMAND 12
#define TEST_PROCESSES_COMMAND 13
#define LIST_ALL_PROCESSES_COMMAND 14
#define GET_PID_COMMAND 15
#define KILL_COMMAND 16
#define BLOCK_COMMAND 17
#define UNBLOCK_COMMAND 18
#define MEMORY_COMMAND 19
#define PROCESS_INFO_COMMAND 20
#define PIPE_INFO_COMMAND 21
#define SEM_INFO_COMMAND 22
#define PHYLO_COMMAND 23
#define NICE_COMMAND 24
#define CAT_COMMAND 25
#define WC_COMMAND 26
#define FILTER_COMMAND 27
#define LOOP_COMMAND 28
#define SH_COMMAND 29

void help_command(void);

int wc_command(void);

#endif
