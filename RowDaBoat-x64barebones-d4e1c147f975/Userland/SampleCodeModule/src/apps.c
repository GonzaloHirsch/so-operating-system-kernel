#include "../include/apps.h"

char * commandsData[] = {
  "help - Displays available commands and their usage\n",
  "quit- Returns to shell\n",
  "time- Displays current system time\n",
  "date- Displays current system date\n",
  "credits- Displays info about the group\n",
  "getpid- Gets current process pid\n",
  "mem- Prints memory status \n",
  "ps- Prints all active process information\n",
  "pipe- Prints all active pipes information\n",
  "sem- Prints all active semaphores information\n",
  "cat- Prints to screen\n",
  "wc- Counts the lines\n",
  "filter- Filters\n",
  "loop- Loops every certains seconds and prints a salute and the process id\n",
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

void get_pid_command(void){
  blockOnEntry();

  int pid = sys_get_pid();
  char num[4] = {0};
  itoa(pid, num, 10);
  sys_write(1, num, strlen(num));

  unblockOnExit();
}

void credits_command(void){
  blockOnEntry();

  for (int i = 0; i < creditInfoCount; i++){
    sys_write(1, creditsInfo[i], strlen(creditsInfo[i]));
  }

  sys_close_fd(1);

  unblockOnExit();
}


void date_command(void){
  blockOnEntry();

  char msg[] = "The date is ";
	char date[20] = {0};
	getDate(date);
  concat(msg + strlen(msg), date);
  sys_write(1, msg, strlen(msg));
  sys_close_fd(1);

  unblockOnExit();
}

void time_command(void){
  blockOnEntry();

  char msg[] = "The time is ";
	char time[20] = {0};
	getTime(time);
  concat(msg + strlen(msg), time);
  sys_write(1, msg, strlen(msg));
  sys_close_fd(1);

  unblockOnExit();
}

void loop_command(void){
  blockOnEntry();

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

  unblockOnExit();
}

void filter_command(void){
  blockOnEntry();

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
  unblockOnExit();
}

void sem_command(void){
  blockOnEntry();
  sys_print_sem_info();
    sys_close_fd(1);
    unblockOnExit();
}

void pipe_command(void){
  blockOnEntry();
  sys_print_pipe_info();
    sys_close_fd(1);

    unblockOnExit();
}

void ps_command(void){
  blockOnEntry();
  sys_list_processes();
    sys_close_fd(1);

    unblockOnExit();
}

void mem_command(void){
  blockOnEntry();
  sys_print_mem_state();
    sys_close_fd(1);
    unblockOnExit();
}

void cat_command(void){
  blockOnEntry();
  char buff[1024] = {0};
  while(sys_read(0, buff, 1024) != -1){
    sys_write(1, buff, strlen(buff));
  }
  sys_close_fd(1);
  unblockOnExit();
}

void help_command(void){
  blockOnEntry();
	for (int i = 0; i < CCOUNT; i++){
    sys_write(1, commandsData[i], strlen(commandsData[i]));
	}
  sys_close_fd(1);
  unblockOnExit();
}

void wc_command(void){
  blockOnEntry();

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

	char num[3] = {0};
  itoa(count, num, 10);
  sys_write(1, num, strlen(num));
  unblockOnExit();
}

void quit_command(){
  unblockOnExit();
}
