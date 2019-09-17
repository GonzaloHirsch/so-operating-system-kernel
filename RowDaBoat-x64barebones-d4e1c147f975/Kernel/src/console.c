//
// Created by nacho on 12/06/19.
//

#include <console.h>

#define MAX_BUFFER 100


Color background = {29,29,29};

Color foreground = {255,255,255};

Color errorForeground = {255,0,0};

Color okForeground = {0, 255, 0};

static Vector2 ZeroVector = {0,0};
static Vector2 cursor = {0,0};

int get_max_line();

int get_max_cursorY(){
    return getResY() / CHAR_HEIGHT;
}

int get_max_cursorX(){
    return getResX() / CHAR_WIDTH;
}

void init_console(){
    clear_console();
    cursor.y = get_max_line()-1;
}

// Borrado de un caracter
void backspace(){
  if(cursor.y == 0 && cursor.x == 0){
    return;
  } else if(cursor.x == 0){
    cursor.x = get_max_cursorX()-1;
    cursor.y--;
  } else {
    cursor.x--;
  }

  draw_char_with_background(cursor, ' ', background, background);
}

void new_line(){
    cursor.y++;
    cursor.x=0;
}

// Imprime un string finalizado con 0 con Color especifico.
void printWithColors(Color chosenForeground, Color chosenBackground, char * str, va_list list){

    int i = 0;
    while(str[i] != 0){
        if(str[i] == '%' && str[i-1] != '\\'){
            char buffer[MAX_BUFFER] = {0};
            switch (str[i+1]) {
                case 'd':
                    int_to_string((int) va_arg(list,int), buffer, 10);
                    print(buffer);
                    i++;
                    break;
                case 's':
                    print(va_arg(list,char*));
                    i++;
                    break;
            }
        }
        else {
            switch(str[i]) {
                case '\n':
                    new_line();
                    break;
                case '\t':
                    cursor.x+=4;
                    break;
                default:
                    draw_char_with_background(cursor, str[i], chosenForeground, chosenBackground);
                    cursor.x++;
                    break;
            }
        }


        if(cursor.x >= getResX()/CHAR_WIDTH){
            cursor.x = 0;
            cursor.y++;
        }
        if(cursor.y >= getResY()/CHAR_HEIGHT){
            move_all_up();
            cursor.y--;
        }
        i++;
    }
}

// Imprimir un solo caracter
void print_char(char c){
    switch(c) {
        case '\n':
            new_line();
            break;
        case '\t':
            cursor.x+=4;
            break;
        case '\b':
            backspace();
            break;
        default:
            draw_char_with_background(cursor, c, foreground, background);
            cursor.x++;
            break;
    }

    //Empezar una nueva linea si el cursor se pasa del
    //maximo x posible
    if(cursor.x >= getResX()/CHAR_WIDTH){
        cursor.x = 0;
        cursor.y++;
    }

    //Mover todo el contenido una linea para arriba
    //si se necesita una nueva linea y el cursor esta
    //en la posicion mas baja de la pantalla
    if(cursor.y >= getResY()/CHAR_HEIGHT){
        move_all_up();
        cursor.y--;
    }
}

// Imprimir un string de N caracteres
void print_N(const char * str, int length){
    for(int i = 0; i < length; i++){
        print_char(str[i]);
    }
}

void print(char * str, ...){

    va_list list;
    va_start(list, str);
    printWithColors(foreground, background, str, list);
    va_end(list);
}

void printInteger(uint64_t dec){
    print("%d", dec);
}

void printError(char * str, ...){

    va_list list;
    va_start(list, str);
    printWithColors(errorForeground, background, str, list);
    va_end(list);
}

//Obsolete: Mover una linea para arriba
void move_line_up(unsigned int line){
    Color c = {0,0,0};
    Vector2 posGet = {0,line*CHAR_HEIGHT};
    Vector2 posDraw = {0,(line-1)*CHAR_HEIGHT};
    for(int j = 0; j < CHAR_HEIGHT; j++){
        for (int i = 0; i < getResX(); ++i)
        {
            get_pixel(posGet, &c);
            draw_pixel(posDraw, c);
            posGet.x++;
            posDraw.x++;
        }
        posGet.x=0;
        posGet.y++;
        posDraw.x=0;
        posDraw.y++;
    }
}

void move_all_up(){
  move_all_lines_up();
    clear_line(get_max_line()-1);
}

// Borrar linea
void clear_line(unsigned int line){
    Vector2 posDraw = {0,line*CHAR_HEIGHT};
    for(int j = 0; j < CHAR_HEIGHT; j++){
        for (int i = 0; i < getResX(); ++i)
        {
            draw_pixel(posDraw, background);
            posDraw.x++;
        }
        posDraw.x=0;
        posDraw.y++;
    }
}

// Limpiar pantalla
void clear_console(){
    Vector2 size = {getResX(), getResY()};
    draw_rect(ZeroVector, size, background);
    cursor.x = 0;
    cursor.y = get_max_line()-1;
}

int get_max_line(){
    return getResY() / CHAR_HEIGHT;
}
