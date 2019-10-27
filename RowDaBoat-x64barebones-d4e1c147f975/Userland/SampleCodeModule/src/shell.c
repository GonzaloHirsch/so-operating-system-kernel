#include <shell.h>
#include "../include/syscallTests.h"
#include "../include/syscalls.h"
#include "../include/shell.h"

// ----------------------------------------------------------------------------------
// Este modulo es el modulo principal de Userland
// A partir de este modulo se pueden seleccionar otros modulos a traves de la terminal
// Se hace todo el manejo de los comandos
// ----------------------------------------------------------------------------------

//Todos los comandos disponibles
const char * commands[] = {
  "help",
  "snake",
  "shutdown",
  "invalid",
  "time",
  "beep",
  "sleep",
  "date",
  "clear",
  "div",
  "credits",
  "starwars",
  "mario",
  "tp",
  "lp",
  "getpid",
  "kill",
  "block",
  "unblock",
  "mem",
  "ps",
  "pipe",
  "sem",
  "phylo",
  "nice",
  "cat",
  "wc",
  "filter",
  "loop",
  "sh"
};

/*
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
*/

const int commandCount = 30;

int getCommand(char * cmd, int * index);
void generate_invalid_opc(void);
int generate_zero_division(void);
void display_credits(void);
void make_starwars(void);
void make_mario(void);

uint64_t * init_shell(void){
	display_welcome_message();

	print("arquiOS@ITBA: ");
	//Comando elegido
	int command = INVALID_COMMAND;
	//Buffer para el comando que se quiere escribir
	char commandBuff[MAX_BUFF_SIZE] = {0};
	//Posicion en el buffer de comando
	int commandBuffPos = 0;
	//Tecla que se toca
	char key;

	int index;

	//while para la shell y su funcionamiento
	while(command != SHUTDOWN_COMMAND){
		key = getKey();

		//CASO EN QUE SE PASA DE LA CANTIDAD MAXIMA DE CARACTERES
		if (commandBuffPos == MAX_BUFF_SIZE){
			//Define el comando como invalido
			command = INVALID_COMMAND;
			//Imprime una linea nueva para que se vea bien
			print("\n");
			//Llama a la funcion para manejar el comando
			handle_command(command, NULL);
			//Hace un reset del buffer de comando volviendo a la posicion 0
			commandBuffPos = 0;
			//Imprime el usuario de nuevo
			print("\narquiOS@ITBA: ");
		}

		//CASO ENTER
		if (key == '\n'){
			//Manda el enter a la pantalla para que baje la linea y se vea bien
			writeKey(&key);
			//Agrega terminacion en 0 al buffer de comando
			commandBuff[commandBuffPos] = 0;
			//Recupera el comando que fue elegido
			command = getCommand(commandBuff, &index);
			//Llama a la funcion que decide como actuar en frente del comando
			handle_command(command, commandBuff + index);
			//Hace un reset del buffer de comando volviendo a la posicion 0
			commandBuffPos = 0;
			//Vuelve a imprimir el usuario para que se vea bien
			if(command!=SHUTDOWN_COMMAND) print("arquiOS@ITBA: ");
		}
		//CASO BACKSPACE - DELETE
		else if (key == '\b'){
			//En el caso de que se borro todo, para que no borre de mas
			if (commandBuffPos != 0){
				//Manda el backspace para que borre fisicamente a la tecla
				writeKey(&key);
				//Borra la letra del buffer de comando
				commandBuffPos--;
			}
		}
		//CASO CUALQUIER OTRA TECLA
		else if (key != 0){
			//Manda la letra a pantalla
			writeKey(&key);
			//Manda la letra al buffer de comando
			commandBuff[commandBuffPos] = key;
			//Incrementa la longitud de la palabra
			commandBuffPos++;
		}
	}

	display_goodbye_message();

	return (uint64_t *) RETURN_ADRESS;
}

int strtokcpy(char * dst, const char * src, char delim){

    int i;
    for(i = 0; src[i] && src[i] != '\n' && src[i] != delim; i++){
        dst[i] = src[i];
    }
    dst[i] = 0;
    return i+1;
}

int getCommand(char * cmd, int * index){
	//Itero el array de comandos para ver cual es el que se elige
	int result = INVALID_COMMAND;
	char nameBuff[MAX_NAME_SIZE];
	*index = strtokcpy(nameBuff, cmd, ' ');
	for (int i = 0; i < commandCount && result == INVALID_COMMAND; i++){
		//En el caso de que sean iguales
		if (!strcmp(nameBuff, commands[i])){
			result = i;
		}
	}
	return result;
}

//Switch para el comando elegido
//Recibe el comando como un parametro
void handle_command(int cmd, char * params){
	int w, x, shellPID;
  char buff[256] = {0};
	switch(cmd){
		case HELP_COMMAND:
			help_command();
		break;
		case SNAKE_COMMAND:
		w = initSnakeGame();
		printf("You survived %d seconds\n", w);
		break;
		//Retorna y sale del while, y no se puede hacer nada mas
		case SHUTDOWN_COMMAND:
	    clearScreen();
	    display_goodbye_message();
	    sys_shutdown();
		break;
		case INVALID_OC_COMMAND:
			generate_invalid_opc();
		break;
		case DIV_COMMAND:
			generate_zero_division();
		break;
		case DATE_COMMAND:
			display_date();
		break;
		case TIME_COMMAND:
			display_time();
		break;
		case BEEP_COMMAND:
			make_sound();
		break;
		case SLEEP_COMMAND:
			sleep();
		break;
		case CLEAR_COMMAND:
			clearScreen();
		break;
		case INVALID_COMMAND:
			display_invalid_command();
		break;
		case CREDITS_COMMAND:
			display_credits();
		break;
		case STARWARS_COMMAND:
			make_starwars();
		break;
		case MARIO_COMMAND:
			make_mario();
		break;
    case TEST_PROCESSES_COMMAND:
      processCreationTest();
    break;
    case LIST_ALL_PROCESSES_COMMAND:
      sys_list_processes();
    break;
    case GET_PID_COMMAND:
      printf("%d\n", sys_get_pid());
    break;
    case KILL_COMMAND:
      //sscanf(params, "%d\n", &w);
      scanf("%d\n", &w);
      sys_kill(w);
      printf("Killed Process %d\n", w);
    break;
    case BLOCK_COMMAND:
      //sscanf(params, "%d\n", &w);
      scanf("%d\n", &w);
      sys_block(w);
      printf("Blocked Process %d\n", w);
		break;
    case UNBLOCK_COMMAND:
      //sscanf(params,"%d\n", &w);
      scanf("%d\n", &w);
      sys_unblock(w);
      printf("Unblocked Process %d\n", w);
		break;
    case PHYLO_COMMAND:
      shellPID = sys_get_pid();
      sys_new_process("philosophers_problem", (uint64_t) philosopherProblem, 1, FOREGROUND);
      sys_block(shellPID);
    break;
    case NICE_COMMAND:
      //sscanf(params, "%d %d\n", &w, &x);
      scanf("%d %d\n", &w, &x);
      sys_change_priority(w, x);
    break;
    case LOOP_COMMAND:
      //TODO: VER COMO SE HACE ESTO, SI ES CON OTRO PROCESO O LOOPEAMOS LA SHELL
      //loop_process();
      //sscanf(params, "%d %d\n", &w, &x);
      //sys_change_priority(w, x);
    break;
    case PIPE_INFO_COMMAND:
        sys_print_pipe_info();
        break;
    case SEM_INFO_COMMAND:
        sys_print_sem_info();
        break;
    case SH_COMMAND:
      shellPID = sys_get_pid();
      sys_new_process("sh_process", (uint64_t) shellMain, 1, FOREGROUND);
      sys_block(shellPID);
    break;
	}
	print("\n");
}

/*
	Imprime el mensaje de bienvenida
	Se generaron con una pagina web
*/
void display_welcome_message(void){
	clearScreen();
	print("						                                               /$$  /$$$$$$   /$$$$$$\n");
	print("						                                              |__/ /$$__  $$ /$$__  $$\n");
	print("						        /$$$$$$   /$$$$$$   /$$$$$$  /$$   /$$ /$$| $$  \\ $$| $$  \\__/\n");
	print("						       |____  $$ /$$__  $$ /$$__  $$| $$  | $$| $$| $$  | $$|  $$$$$$ \n");
	print("						        /$$$$$$$| $$  \\__/| $$  \\ $$| $$  | $$| $$| $$  | $$ \\____  $$\n");
	print("						       /$$__  $$| $$      | $$  | $$| $$  | $$| $$| $$  | $$ /$$  \\ $$\n");
	print("						      |  $$$$$$$| $$      |  $$$$$$$|  $$$$$$/| $$|  $$$$$$/|  $$$$$$/\n");
	print("						       \\_______/|__/       \\____  $$ \\______/ |__/ \\______/  \\______/ \n");
	print("						                                | $$                                  \n");
	print("						                                | $$    \n");
	print("						                                |__/   \n\n");
	print("													Trabajo Practico Especial\n");
	print("												  Arquitectura de Computadoras\n");
	print("											 		1er Cuatrimestre - 2019\n\n");
  print("											 		   Trabajo Practico\n");
  print("											 		  Sistemas Operativos\n");
  print("											 		2do Cuatrimestre - 2019\n\n");
	print("													  Welcome to arquiOS\n");
	print("										Type \"help\" to discover all available commands\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}

/*
void display_help(void){
  print("fun fun fun");
	for (int i = 0; i < commandCount; i++){
		print(commandsInfo[i]);
	}
}
*/

void display_time(void){
	print("The time is ");
	char time[20];
	getTime(time);
	print(time);
}

void loop_process(){

}

void display_date(void){
	print("The date is ");
	char date[20];
	getDate(date);
	print(date);
}

void display_credits(void){
	print("The authors of this OS(Arquitectura de Computadoras Version) are:\n");
  print("Ignacio Ribas - Gonzalo Hirsch - Ignacio Villanueva\n");
	print("The authors of this OS(Sistemas Operativos Version) are:\n");
  print("Ignacio Ribas - Gonzalo Hirsch - Augusto Henestrosa\n");
}

void generate_invalid_opc(){
	uint64_t invalid = 0xFFFFFFFFFFFF;
	uint64_t * ptr = &invalid;
	((void(*)())ptr)();
}

int generate_zero_division(){
	int a = 10;
	int b = 0;
	return a/b;
}

void make_sound(void){
	makeSound(800, 5);
}

void make_starwars(void){
		makeSound(440, 500/3); makeSound(440, 1500/3);
    makeSound(440, 1500/3);
    makeSound(349, 1050/3); makeSound(523, 450/3);
    makeSound(440, 1500/3); makeSound(349, 1050/3);
    makeSound(523, 450/3); makeSound(440, 1000/3);
    goToSleep(10); // Delay 250 milliseconds !!!!
    makeSound(659, 1500/3); makeSound(659, 1500/3);
    makeSound(659, 1500/3); makeSound(698, 1050/3);
    makeSound(523, 450/3);
    makeSound(415, 1500/3); makeSound(349, 1050/3);
    makeSound(523, 450/3); makeSound(440, 1000/3);
    goToSleep(10); // Delay 250 milliseconds !!!!
    makeSound(880, 1500/3); makeSound(440, 1050/3);
    makeSound(440, 450/3); makeSound(880, 1500/3);
    makeSound(830, 750/3);
    makeSound(784, 750/3);
    makeSound(740, 375/3); makeSound(698, 375/3);
    makeSound(740, 750/3);
    goToSleep(7); // Delay 250 milliseconds !!!!
    makeSound(455, 750/3); makeSound(622, 1500/3);
    makeSound(587, 750/3); makeSound(554, 750/3);
    makeSound(523, 375/3); makeSound(466, 375/3);
    makeSound(523, 750/3);
    goToSleep(7); // Delay 250 milliseconds !!!!
    makeSound(349, 375/3); makeSound(415, 1500/3);
    makeSound(349, 375/3); makeSound(440, 375/3);
    makeSound(523, 1500/3); makeSound(440, 375/3);
    makeSound(523, 375/3); makeSound(659, 1000/3);
    makeSound(880, 1500/3); makeSound(440, 1050/3);
    makeSound(440, 450/3); makeSound(880, 1500/3);
    makeSound(830, 750/3); makeSound(784, 750/3);
    makeSound(740, 375/3); makeSound(698, 375/3);
    makeSound(740, 750/3);
    goToSleep(7);
    makeSound(455, 750/3); makeSound(622, 1500/3);
    makeSound(587, 750/3); makeSound(554, 750/3);
    makeSound(523, 375/3); makeSound(466, 375/3);
    makeSound(523, 750/3);
    goToSleep(7);
    makeSound(349, 750/3); makeSound(415, 1500/3);
    makeSound(349, 375/3); makeSound(523, 375/3);
    makeSound(440, 1500/3); makeSound(349, 375/3);
    makeSound(261, 375/3); makeSound(440, 1000/3);
    goToSleep(7);
}

void make_mario(void){
		makeSound(659, 125/35); makeSound(659, 125/35); goToSleep(125/35); makeSound(659, 125/35);
		goToSleep(167/35); makeSound(523, 125/35); makeSound(659, 125/35); goToSleep(125/35);
		makeSound(784, 125/35); goToSleep(375/35); makeSound(392, 125/35); goToSleep(375/35);
		makeSound(523, 125/35); goToSleep(250/35); makeSound(392, 125/35); goToSleep(250/35);
		makeSound(330, 125/35); goToSleep(250/35); makeSound(440, 125/35); goToSleep(125/35);
		makeSound(494, 125/35); goToSleep(125/35); makeSound(466, 125/35); goToSleep(42/35);
		makeSound(440, 125/35); goToSleep(125/35); makeSound(392, 125/35); goToSleep(125/35);
		makeSound(659, 125/35); goToSleep(125/35); makeSound(784, 125/35); goToSleep(125/35);
	 makeSound(880, 125/35); goToSleep(125/35); makeSound(698, 125/35); makeSound(784, 125/35);
	 goToSleep(125/35); makeSound(659, 125/35); goToSleep(125/35); makeSound(523, 125/35);
	 goToSleep(125/35); makeSound(587, 125/35); makeSound(494, 125/35); goToSleep(125/35);
	  makeSound(523, 125/35); goToSleep(250/35); makeSound(392, 125/35); goToSleep(250/35);
		makeSound(330, 125/35); goToSleep(250/35); makeSound(440, 125/35); goToSleep(125/35);
		makeSound(494, 125/35); goToSleep(125/35); makeSound(466, 125/35); goToSleep(42/35);
		makeSound(440, 125/35); goToSleep(125/35); makeSound(392, 125/35); goToSleep(125/35);
		makeSound(659, 125/35); goToSleep(125/35); makeSound(784, 125/35); goToSleep(125/35);
		 makeSound(880, 125/35); goToSleep(125/35); makeSound(698, 125/35); makeSound(784, 125/35);
		 goToSleep(125/35); makeSound(659, 125/35); goToSleep(125/35); makeSound(523, 125/35);
		goToSleep(125/35); makeSound(587, 125/35); makeSound(494, 125/35); goToSleep(375/35);
		makeSound(784, 125/35); makeSound(740, 125/35); makeSound(698, 125/35); goToSleep(42/35);
		 makeSound(622, 125/35); goToSleep(125/35); makeSound(659, 125/35); goToSleep(167/35);
		 makeSound(415, 125/35); makeSound(440, 125/35); makeSound(523, 125/35); goToSleep(125/35);
		  makeSound(440, 125/35); makeSound(523, 125/35); makeSound(587, 125/35); goToSleep(250/35);
			makeSound(784, 125/35); makeSound(740, 125/35); makeSound(698, 125/35); goToSleep(42/35);
			makeSound(622, 125/35); goToSleep(125/35); makeSound(659, 125/35);
		goToSleep(167/35); makeSound(698, 125/35); goToSleep(125/35); makeSound(698, 125/35);
		makeSound(698, 125/35); goToSleep(625/35); makeSound(784, 125/35); makeSound(740, 125/35);
	 makeSound(698, 125/35); goToSleep(42/35); makeSound(622, 125/35); goToSleep(125/35);
	 makeSound(659, 125/35); goToSleep(167/35); makeSound(415, 125/35); makeSound(440, 125/35);
	 makeSound(523, 125/35); goToSleep(125/35); makeSound(440, 125/35); makeSound(523, 125/35);
	  makeSound(587, 125/35); goToSleep(250/35); makeSound(622, 125/35); goToSleep(250/35);
		makeSound(587, 125/35); goToSleep(250/35); makeSound(523, 125/35); goToSleep(1125/35);
		makeSound(784, 125/35); makeSound(740, 125/35); makeSound(698, 125/35); goToSleep(42/35);
		makeSound(622, 125/35); goToSleep(125/35); makeSound(659, 125/35);
	 goToSleep(167/35); makeSound(415, 125/35); makeSound(440, 125/35); makeSound(523, 125/35);
	  goToSleep(125/35); makeSound(440, 125/35); makeSound(523, 125/35); makeSound(587, 125/35);
		goToSleep(250/35); makeSound(784, 125/35); makeSound(740, 125/35); makeSound(698, 125/35);
		goToSleep(42/35); makeSound(622, 125/35); goToSleep(125/35); makeSound(659, 125/35);
		goToSleep(167/35); makeSound(698, 125/35); goToSleep(125/35); makeSound(698, 125/35);
		makeSound(698, 125/35); goToSleep(625/35); makeSound(784, 125/35);
	 makeSound(740, 125/35); makeSound(698, 125/35); goToSleep(42/35); makeSound(622, 125/35);
	 goToSleep(125/35); makeSound(659, 125/35); goToSleep(167/35); makeSound(415, 125/35);
	 makeSound(440, 125/35); makeSound(523, 125/35); goToSleep(125/35); makeSound(440, 125/35);
	 makeSound(523, 125/35); makeSound(587, 125/35); goToSleep(250/35); makeSound(622, 125/35);
	 goToSleep(250/35); makeSound(587, 125/35); goToSleep(250/35); makeSound(523, 125/35);
}

void sleep(void){
	goToSleep(50);
}

void display_invalid_command(void){
	print("Invalid command, type \'help\' to view system commands\n");
}

void display_goodbye_message(void){
	clearScreen();
	print("							  /$$$$$$                            /$$ /$$                          \n");
	print("							 /$$__  $$                          | $$| $$                          \n");
	print("							| $$  \\__/  /$$$$$$   /$$$$$$   /$$$$$$$| $$$$$$$  /$$   /$$  /$$$$$$ \n");
	print("							| $$ /$$$$ /$$__  $$ /$$__  $$ /$$__  $$| $$__  $$| $$  | $$ /$$__  $$\n");
	print("							| $$|_  $$| $$  \\ $$| $$  \\ $$| $$  | $$| $$  \\ $$| $$  | $$| $$$$$$$$\n");
	print("							| $$  \\ $$| $$  | $$| $$  | $$| $$  | $$| $$  | $$| $$  | $$| $$_____/\n");
	print("							|  $$$$$$/|  $$$$$$/|  $$$$$$/|  $$$$$$$| $$$$$$$/|  $$$$$$$|  $$$$$$$\n");
	print("							 \\______/  \\______/  \\______/  \\_______/|_______/  \\____  $$ \\_______/\n");
	print("							                                                   /$$  | $$          \n");
	print("							                                                  |  $$$$$$/          \n");
	print("					    		                                               \\______/           \n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}
