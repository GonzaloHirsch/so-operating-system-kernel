#include "../include/apps.h"

const char * commandsInfo[] = {
  "help - Displays available commands and their usage\n",
  "snake - Initiates the snake game\n",
  "shutdown - Shuts down the system\n",
  "time - Displays current system time\n",
  "date - Displays current system date\n",
  "beep - Makes the system go Beep!\n",
  "sleep - Makes the system sleep for 5 seconds\n",
  "div - Performs a division by zero\n",
  "invalid - Executes an invalid operation\n",
  "clear - Clears the screen\n",
  "credits - Displays info about the group\n",
  "starwars - Makes a cool Star Wars sound!\n",
  "mario - Makes a cool Mario sound!\n",
  "tp - \n",
  "lp - \n",
  "getpid - \n",
  "kill - \n",
  "block - \n",
  "unblock - \n",
  "mem - Prints memory status\n",
  "ps - Prints all active process information\n",
  "pipe - Prints all active pipes information\n",
  "sem - Prints all active semaphores information\n",
  "phylo - Starts the phylosophers problem, exit the problem with \'q\'\n",
  "nice - Changes the priority of a process\n",
  "cat - Prints to stdin as it receives data\n",
  "wc - Counts amount of lines in input\n",
  "filter - Filters vowels from input\n",
  "loop - Prints PID with a message every 5 seconds\n",
  "sh - Inits a secondary shell\n",
};

const int cCount = 30;

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

void help_command(void){
  blockOnEntry();
	for (int i = 0; i < cCount; i++){
    sys_write(1, commandsInfo[i], strlen(commandsInfo[i]));
		//print(commandsInfo[i]);
	}
  sys_mark_pipe_end(1);
  unblockOnExit();
}

int wc_command(void){
  blockOnEntry();

  int count = 0;
  char buff[1024] = {0};
  sys_read(0, buff, 1024);

  //print("loop");
  int index = 0;
  while(index < 1024){
    if (buff[index] == '\n'){
      count++;
    }
    index++;
  }

/*
  while(strlen(buff) > 0){
    //print("loop");
    int index = 0;
    while(index < 1024){
      if (buff[index] == '\n'){
        count++;
      }
      index++;
    }
    sys_read(0, buff, 1024);
  }
*/
	char num[5] = {0};
  itoa(count, num, 10);
  sys_write(1, num, strlen(num));
  print(num);
  unblockOnExit();
}
