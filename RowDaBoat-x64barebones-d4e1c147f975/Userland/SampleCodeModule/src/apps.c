#include "../include/apps.h"

char * commandsData[] = {
  "help- \n",
  "quit- \n",
  "time- \n",
  "date- \n",
  "credits- \n",
  "getpid- \n",
  "mem- \n",
  "ps- \n",
  "pipe- \n",
  "sem- \n",
  "cat- \n",
  "wc- \n",
  "filter- \n",
  "loop- \n",
};

char * creditsInfo[] = {
  "The authors of this OS(Arquitectura de Computadoras Version) are:\n",
  "Ignacio Ribas - Gonzalo Hirsch - Ignacio Villanueva\n",
	"The authors of this OS(Sistemas Operativos Version) are:\n",
  "Ignacio Ribas - Gonzalo Hirsch - Augusto Henestrosa\n"
};

#define CCOUNT 14
int creditInfoCount = 4;

void blockOnEntry();
void unblockOnExit();

// Funciones para bloquear al padre y para desbloquearlo y matarse a uno mismo (proceso)

void blockOnEntry(){
  int pid = sys_get_pid();
  int ppid = sys_get_p_pid(pid);
  sys_block(ppid);
}

void unblockOnExit(){
  int pid = sys_get_pid();
  int ppid = sys_get_p_pid(pid);
  sys_unblock(ppid);
  sys_kill(pid);
}

//------------------------------------------------------------------------------

// Comandos disponibles para hacer desde el sh

void get_pid_command(void){
  int pid = sys_get_pid();
  char num[4] = {0};
  itoa(pid, num, 10);
  sys_write(1, num, strlen(num));
  sys_close_fd(1);
}

void credits_command(void){
  for (int i = 0; i < creditInfoCount; i++){
    sys_write(1, creditsInfo[i], strlen(creditsInfo[i]));
  }

  sys_close_fd(1);
}

void date_command(void){
  char msg[] = "The date is ";
	char date[20] = {0};
	getDate(date);
  concat(msg + strlen(msg), date);
  sys_write(1, msg, strlen(msg));
  sys_close_fd(1);
}

void time_command(void){
  char msg[] = "The time is ";
	char time[20] = {0};
	getTime(time);
  concat(msg + strlen(msg), time);
  sys_write(1, msg, strlen(msg));
  sys_close_fd(1);
}

void loop_command(void){
  char msg[] = "Hi! My PID is ";
  print(msg);

  int pid = sys_get_pid();
  char num[4] = {0};
  itoa(pid, num, 10);
  concat(msg + strlen(msg), num);

  while(1){
    sys_write(1, msg, strlen(msg));
    goToSleep(75);
  }
  sys_close_fd(1);
}

void filter_command(void){
  char buffOut[1024] = {0};
  char buffIn[1024] = {0};

  int index;
  int indexOut = 0;
  while(sys_read(0, buffIn, 1024) != -1 && indexOut < 1024){
    index = 0;
    while(buffIn[index] != 0){
      if (!(buffIn[index] == 'a' || buffIn[index] == 'A' || buffIn[index] == 'e' || buffIn[index] == 'E' || buffIn[index] == 'i' || buffIn[index] == 'I' || buffIn[index] == 'o' || buffIn[index] == 'O' || buffIn[index] == 'u' || buffIn[index] == 'U')){
        buffOut[indexOut++] = buffIn[index];
      }
      index++;
    }
  }

  sys_write(1, buffOut, strlen(buffOut));
  sys_close_fd(1);
}

void cat_command(void){
  char buff[1024] = {0};
  while(sys_read(0, buff, 1024) != -1){
    sys_write(1, buff, strlen(buff));
  }
  sys_close_fd(1);
}

void help_command(void){
	for (int i = 0; i < CCOUNT; i++){
    sys_write(1, commandsData[i], strlen(commandsData[i]));
	}
  sys_close_fd(1);
}

void wc_command(void){
  int count = 0;
  char buff[1024] = {0};

  int index;
  while(sys_read(0, buff, 1024) != -1){
    index = 0;
    while(buff[index] != 0){
      if (buff[index++] == '\n'){
        count++;
      }
    }
  }

	char num[4] = {0};
  itoa(count, num, 10);
  int i = 0;
  while(i < 4 && num[i] != 0){
    i++;
  }
  if (i < 3){
    num[i] = '\n';
  }

  sys_write(1, num, strlen(num));
}

void quit_command(){
  unblockOnExit();
}

//------------------------------------------------------------------------------


// Funciones built in, imprime el kernel y no sobre un FD

void sem_command(void){
  sys_print_sem_info();
}

void pipe_command(void){
  sys_print_pipe_info();
}

void ps_command(void){
  sys_list_processes();
}

void mem_command(void){
  sys_print_mem_state();
}

//------------------------------------------------------------------------------
