#include "../include/apps.h"

const char * commandsInfoSh[] = {
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
  "loop- \n"
};

const int cCount = 13;

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

  char line1[] = "The authors of this OS(Arquitectura de Computadoras Version) are:\n";
  char line2[] = "Ignacio Ribas - Gonzalo Hirsch - Ignacio Villanueva\n";
	char line3[] = "The authors of this OS(Sistemas Operativos Version) are:\n";
  char line4[] = "Ignacio Ribas - Gonzalo Hirsch - Augusto Henestrosa\n";

  sys_write(1, line1, strlen(line1));
  sys_write(1, line2, strlen(line2));
  sys_write(1, line3, strlen(line3));
  sys_write(1, line4, strlen(line4));
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
  int index = 0;
    int indexOut = 0;
  while(sys_read(0, buffIn, 1024)!=-1) {

      index = 0;
      indexOut = 0;

      while (index < 1024) {
          if (!(buffIn[index] == 'a' || buffIn[index] == 'A' || buffIn[index] == 'e' || buffIn[index] == 'E' ||
                buffIn[index] == 'i' || buffIn[index] == 'I' || buffIn[index] == 'o' || buffIn[index] == 'O' ||
                buffIn[index] == 'u' || buffIn[index] == 'U')) {
              buffOut[indexOut++] = buffIn[index];
          }
          index++;
      }

      sys_write(1, buffOut, strlen(buffOut));
  }
  sys_close_fd(1);
  unblockOnExit();
}

void sem_command(void){
  blockOnEntry();
  sys_print_sem_info();
  unblockOnExit();
}

void pipe_command(void){
  blockOnEntry();
  sys_print_pipe_info();
  unblockOnExit();
}

void ps_command(void){
  blockOnEntry();
  sys_list_processes();
  unblockOnExit();
}

void mem_command(void){
  blockOnEntry();
  sys_print_mem_state();
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
	for (int i = 0; i < cCount; i++){
    sys_write(1, commandsInfoSh[i], strlen(commandsInfoSh[i]));
		//print(commandsInfo[i]);
	}
  sys_close_fd(1);
  unblockOnExit();
}

int getCount(char * src, char ch){
    int c=0;
    while(*src){
        if(*src==ch) c++;
        src++;
    }
    return c;
}

int wc_command(void){
  blockOnEntry();
  int count = 0;
  char buff[1024] = {0};
  sys_read(0, buff, 10);
  while(sys_read(0, buff, 100)!=-1){
    count+=getCount(buff, '\n');
  }
	char num[3] = {0};
  itoa(count, num, 10);
  sys_write(1, num, strlen(num));
  unblockOnExit();
}

void quit_command(void){
  int pid = sys_get_pid();
  int ppid = sys_get_p_pid(pid);
  char * goodbyeMess = "Goodbye!\n";
  sleep(3000);
  sys_write(1,goodbyeMess,strlen(goodbyeMess));
  sys_unblock(ppid);
  sys_kill(pid);
}
