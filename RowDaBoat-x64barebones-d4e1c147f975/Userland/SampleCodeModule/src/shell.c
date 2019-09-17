#include <shell.h>

// ----------------------------------------------------------------------------------
// Este modulo es el modulo principal de Userland
// A partir de este modulo se pueden seleccionar otros modulos a traves de la terminal
// Se hace todo el manejo de los comandos
// ----------------------------------------------------------------------------------

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

//Todos los comandos disponibles
const char * commands[] = {"help", "snake", "shutdown", "invalid", "time", "beep", "sleep", "date", "clear", "div", "credits", "starwars", "mario"};
const int commandCount = 13;

int getCommand(char * cmd);
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
			handle_command(command);
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
			command = getCommand(commandBuff);
			//Llama a la funcion que decide como actuar en frente del comando
			handle_command(command);
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

int getCommand(char * cmd){
	//Itero el array de comandos para ver cual es el que se elige
	int result = INVALID_COMMAND;
	for (int i = 0; i < commandCount && result == INVALID_COMMAND; i++){
		//En el caso de que sean iguales
		if (!strcmp(cmd, commands[i])){
			result = i;
		}
	}
	return result;
}

//Switch para el comando elegido
//Recibe el comando como un parametro
void handle_command(int cmd){
	int w;
	switch(cmd){
		case HELP_COMMAND:
			display_help();
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
	print("													  Welcome to arquiOS\n");
	print("										Type \"help\" to discover all available commands\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}

void display_help(void){
	print("help - Displays available commands and their usage\n");
	print("snake - Initiates the snake game\n");
	print("shutdown - Shuts down the system\n");
	print("time - Displays current system time\n");
	print("date - Displays current system date\n");
	print("beep - Makes the system go Beep!\n");
	print("sleep - Makes the system sleep for 5 seconds\n");
  print("div - Performs a division by zero\n");
  print("invalid - Executes an invalid operation\n");
  print("clear - Clears the screen\n");
	print("credits - Displays info about the group\n");
	print("starwars - Makes a cool Star Wars sound!\n");
	print("mario - Makes a cool Mario sound!\n");
}

void display_time(void){
	print("The time is ");
	char time[20];
	getTime(time);
	print(time);
}

void display_date(void){
	print("The date is ");
	char date[20];
	getDate(date);
	print(date);
}

void display_credits(void){
	print("The authors of this OS are:\n");
  print("Ignacio Ribas - Gonzalo Hirsch - Ignacio Villanueva\n");
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
