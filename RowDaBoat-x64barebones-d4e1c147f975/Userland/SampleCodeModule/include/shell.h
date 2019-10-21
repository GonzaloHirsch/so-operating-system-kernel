#ifndef SHELL_H
#define SHELL_H

#include <stdint.h>
#include "naiveConsole.h"
#include "utils.h"
#include "graphicsLibrary.h"
#include "types.h"
#include "snakeGame.h"
#include "philosophers.h"

#define MAX_BUFF_SIZE 750
#define MAX_NAME_SIZE 30

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

//Address original que devolvia el samplecoremodule
#define RETURN_ADRESS 0xDEADC0DE

uint64_t * init_shell(void);

void handle_command(int cmd, char * params);

void display_welcome_message(void);

void display_help(void);

void display_goodbye_message(void);

void clear_buffer(char * buff);

void display_time(void);

void display_date(void);

void make_sound(void);

void sleep(void);

void display_invalid_command(void);

#endif
