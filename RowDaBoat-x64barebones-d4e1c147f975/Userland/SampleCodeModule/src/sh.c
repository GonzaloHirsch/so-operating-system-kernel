//
// Created by click on 26/10/19.
//

#include "../include/sh.h"
#include "../include/shell.h"
#include "../include/apps.h"
//#include "../../../Kernel/include/processes.h"
#include "../include/processes.h"
#include "../include/utils.h"

const char * allCommands[] = {
  "help",
  "quit",
  "time",
  "date",
  "credits",
  "getpid",
  "mem",
  "ps",
  "pipe",
  "cat",
  "wc",
  "filter",
  "loop"
};

const void * funs[] = {
  (void *)help_command,
  "quit",
  (void *)time_command,
  (void *)date_command,
  (void *)credits_command,
  (void *)get_pid_command,
  (void *)mem_command,
  (void *)ps_command,
  (void *)pipe_command,
  (void *)cat_command,
  (void *)wc_command,
  (void *)filter_command,
  (void *)loop_command
};

const int comCount = 12;

static void runCommand(char * buffer);
static void * getProgramFunctionFromName(char * command);

void shellMain(){
    print("arquiOS#ITBA: ");
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
            print("Invalid Command\n");
            //Hace un reset del buffer de comando volviendo a la posicion 0
            commandBuffPos = 0;
            //Imprime el usuario de nuevo
            print("\narquiOS#ITBA: ");
        }

        //CASO ENTER
        if (key == '\n'){
            //Manda el enter a la pantalla para que baje la linea y se vea bien
            writeKey(&key);
            //Agrega terminacion en 0 al buffer de comando
            commandBuff[commandBuffPos] = 0;
            //Recupera el comando que fue elegido
            runCommand(commandBuff);
            //Llama a la funcion que decide como actuar en frente del comando

            //Hace un reset del buffer de comando volviendo a la posicion 0
            commandBuffPos = 0;
            //Vuelve a imprimir el usuario para que se vea bien
            print("\narquiOS#ITBA: ");
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

    int pid = sys_get_pid();
    int ppid = sys_get_p_pid(pid);
    sys_unblock(ppid);
    sys_kill(pid);
}

static void runCommand(char * buffer){

    int i = 0, j = 0, k = 0, p=0;
    int isBG = 0;
    //todo 10 es el max number of programs
    // 256 es el max nombre de programs.
    // por ahora es para evitar errores.
    char programName[256];
    void * processFunctions[MAX_NUMBER_OF_PROGRAMS];
    int pids[MAX_NUMBER_OF_PROGRAMS];
    char pendingPipes[MAX_NUMBER_OF_PROGRAMS - 1] = {0};

    // Parseando los comandos
    while(buffer[i]){
        //si es espacio
        if(buffer[i]==' '){
            programName[j]=0;
            if (strlen(programName) != 0){
              // Verificacion para ver que no nos pasemos de la cantidad maxima de comandos
              if (k < MAX_NUMBER_OF_PROGRAMS){
                if((processFunctions[k++] = getProgramFunctionFromName(programName))==NULL){
                    print("Invalid Command\n");
                    return;
                }
              } else {
                print("Max number of commands reached\n");
                return;
              }
            }

            j=0;
            i++;
        }
        else if(buffer[i]=='|'){
            //indica que se pipea el proceso de antes del | con el de despues
            pendingPipes[k-1]=1;
            i++;
            //leo hasta el proximo caracter no space
        }
        else{
            programName[j++] = buffer[i++];
        }
    }

    // Capturamos el ultimo comando, que no se capturaria por no terminar en un espacio
    programName[j]=0;

    // Flaggea la ejecucion en background
    if (programName[j - 1] == '&'){
      isBG = 1;
    } else {
      // Verificacion para ver que no nos pasemos de la cantidad maxima de comandos
      if (k < MAX_NUMBER_OF_PROGRAMS){
        if((processFunctions[k++] = getProgramFunctionFromName(programName))==NULL){
            print("Invalid Command\n");
            return;
        }
      } else {
        print("Max number of commands reached\n");
        return;
      }
    }

    int pipefd = 0;
    for (int l = 0; l < k; l++){
      // Genera el nombre del proceso/pipe
      char num[2];
      itoa(l, num, 10);
      char name[3 + 2 + 1] = {0};
      concat(name, "sh_");
      concat(name + 3, num);

      if (isBG){
        // Crea el proceso pero no lo ejecuta, para poder settear los fds
        pids[l] = sys_create_process(name, (uint64_t) processFunctions[l], 1, BACKGROUND);
      } else {
        // Crea el proceso pero no lo ejecuta, para poder settear los fds
        pids[l] = sys_create_process(name, (uint64_t) processFunctions[l], 1, FOREGROUND);
      }

      // En el caso de que el comando anterior queria pipearse con este
      if (l - 1 >= 0 && pendingPipes[l - 1] == 1){
        sys_set_process_fd(pids[l], 0, pipefd);
      }

      // En el caso de que tenga un pipe con el proximo comando
      if (l < k - 1 && pendingPipes[l] == 1){
        pipefd = sys_create_pipe(name);
        sys_set_process_fd(pids[l], 1, pipefd);
      }

      // Inicia el proceso con los fds ya setteados
      sys_start_process(pids[l]);
    }
}

void * getProgramFunctionFromName(char * command){
  void * res = NULL;
  for (int i = 0; i < comCount; i++){
		//En el caso de que sean iguales
		if (!strcmp(command, allCommands[i])){
			res = funs[i];
		}
	}
  return res;
}
