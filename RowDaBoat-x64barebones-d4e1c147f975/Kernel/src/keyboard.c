#include <keyboard.h>
#include <naiveConsole.h>
#include <console.h>
#include <lib.h>

extern char read_key();
extern int check_key();

/*
    Buffer con los caracteres escritos
    Lo dejamos
*/
static char buffer[MAX_BUFFER_SIZE]={0};
/*
    Posicion de la escritura en el buffer
    write se usa para operaciones destructivas en el buffer
    Apunta al proximo
*/
static int write;
/*
    Posicion de la lectura en el buffer
    read se usa para operaciones no destructivas del buffer
    Apunta al proximo
*/
static int read;
/*
    Flag de shift, empieza en falso
*/
static int shift = 0;
/*
    Flag de caps lock, empieza en falso
*/
static int caps_lock = 0;
/*
    Flag de que el buffer esta lleno
*/
static int full = 0;

//Macro que shiftea al 1 index veces y le hace un and al binario para ver si esta prendido
#define CHECKBYTE(binary, index) (binary & 1<<(index))

//Key codes normales, para el teclado sin estar con el shift apretado
unsigned char keycode_map[128] = {
    27, '`', '1', '2', '3', '4', '5', '6', '7', '8', '9','0', '-', '=','\b', '\t','q', 'w', 'e', 'r',
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0 /* Control */,
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',  LEFT_SHIFT,'\\', 'z', 'x', 'c', 'v', 'b', 'n',
  'm', ',', '.', '/', RIGHT_SHIFT,'*',0/* Alt */,' ', CAPS_LOCK, 0 ,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, UP_ARROW, 0, '-', LEFT_ARROW, 0, RIGHT_ARROW,'+', 0,
    DOWN_ARROW,0/* Page Down */, 0, 0, 0, 0, 0, 0, 0, 0,
};

//Key codes con la tecla shift apretada
unsigned char alternative_keycode_map[128] = {
  0,'~','!','\"','#','$','%',94, '&', '*', '(', ')', 95,'+', '\b', '\t',
  'Q','W','E','R','T','Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 0,
  'A', 'B', 'C', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',
  '~', LEFT_SHIFT, '|', 'Z', 'X', 'C', 'V', 'B', 'N',
  'M', '<', '>', '?', RIGHT_SHIFT, '*', 0, ' ', CAPS_LOCK, 0,0,0,0,0,0,0,0,0,0, DOWN_ARROW,0,0,0,0,0,0,0,0,0,};

void keyboard_handler(void){
    if (check_key()){
      //Codigo que recibe de la interrupcion
      int code = read_key();

      //Verifica si el break code esta prendido
      //Es el bit numero 7 (el mas significativo)
      //Si esta prendido es que se solto la tecla
      if (CHECKBYTE(code, 7)){
        //Le hace un AND porque sino viene con bits prendidos extra
        code = code & 0xff;
        int aux = code - BREAK_CODE_DIF;
        //Si se solto algun shift, se apaga el flag
        if (keycode_map[aux] == RIGHT_SHIFT || keycode_map[aux] == LEFT_SHIFT){
          shift = 0;
        }
        return;
      }

      //Verifica si apretaste el caps lock, para ver si lo apagaste o prendiste
      if (keycode_map[code] == CAPS_LOCK){
        caps_lock = !(caps_lock);
        return;
      }

      //Si alguna de las teclas de shift se apreta, se prende el flag de shift
      if (keycode_map[code] == RIGHT_SHIFT || keycode_map[code] == LEFT_SHIFT){
        shift = 1;
        return;
      }

      //Recupero la tecla
      unsigned char key = keycode_map[code];
      if (isAlpha(key)){
        //Si esta prendido el caps lock o esta tocando shift, la pasa a mayuscula
        //Dos casos, para que no toque shift y mayuscula al mismo tiempo
        if (caps_lock && !shift){
          key -= 32;
        } else if (!caps_lock && shift){
          key -= 32;
        }
      } else {
        //Si tiene el shift tocado, hace los simbolos
        if (shift){
          key = alternative_keycode_map[code];
        }
      }

      addChar(key);
    }
}

/*
    Agrega un char al buffer
    Si se va a pasar de la longitud del buffer, en el caso del max - 1, lo pone al principio
*/
int addChar(char c){
    if (!full){
      buffer[write] = c;
      write = (write + 1) % MAX_BUFFER_SIZE;
      if (write == read && !full){
        full = 1;
      }
      return 1;
    }
    return 0;
}

/*
    Lee el caracter en la posicion de lectura del buffer y le resta 1 para poder leer muchos caracteres
*/
int getChar(void){
    int aux = EOF;
    if (write != read || (write == read && full)){
      aux = buffer[read];
      //Le saca el modulo para que de "vueltas" alrededor del buffer
      read = (read + 1) % MAX_BUFFER_SIZE;
      if (full){
        full = 0;
      }
    }
    return aux;
}

int isAlpha(char c){
  return (c >= 'A' && c <= 'Z')||(c >= 'a' && c <= 'z');
}

int isNumeric(char c){
  return c>= 48 || c <= 57;
}

int isArrow(char c){
  return c == RIGHT_ARROW || c == LEFT_ARROW || c == UP_ARROW || c == DOWN_ARROW;
}
