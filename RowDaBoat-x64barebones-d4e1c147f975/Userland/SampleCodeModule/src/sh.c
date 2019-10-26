//
// Created by click on 26/10/19.
//

#include "../include/sh.h"
#include "../include/shell.h"
#include "../../../Kernel/include/processes.h"


static void runCommand(char * buffer);

void shellMain(){


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
            print("Invalid Command\n");
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
            runCommand(commandBuff);
            //Llama a la funcion que decide como actuar en frente del comando

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
}

static void runCommand(char * buffer){

    int i = 0, j = 0, k = 0, p=0;
    //todo 10 es el max number of programs
    // 256 es el max nombre de programs.
    // por ahora es para evitar errores.
    char programName[256];
    void * processFunctions[10];
    char pendingPipes[9];

    while(buffer[i]){

        //si es espacio
        if(buffer[i]==' '){
            programName[j]=0;
            if((processFunctions[k++] = getProgramFunctionFromName(programName))==NULL){
                print("Invalid Command\n");
                return;
            }

            j=0;
            i++;
        }
        else if(buffer[i]=='|'){
            //indica que se pipea el proceso de antes del | con el de despues
            pendingPipes[k-1]=1;
            //leo hasta el proximo caracter no space

        }
        else{
            programName[j++] = buffer[i++];
        }
    }
}