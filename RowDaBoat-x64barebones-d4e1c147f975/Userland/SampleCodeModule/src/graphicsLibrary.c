#include <graphicsLibrary.h>

// Dibujar pixel
void drawPixel(Vector2 pos, Color color){
  sys_draw_pixel(pos.x, pos.y, color.r, color.g, color.b);
}

// Dibujar rectangulo
void drawRect(Vector2 start, Vector2 size, Color color){
  for(int j = start.y; j < start.y + size.y; j++){
    for (int i = start.x; i < start.x + size.x; i++) {
      Vector2 aux = {i,j};
      drawPixel(aux, color);
    }
  }
}

// Borrar pantalla
void clearScreen(){
  sys_clear_console();
}

// Dibujar caracter en cualquier parte
void draw_char_with_background(Vector2 pos, char c, Color foreground, Color background){
    unsigned char * cMap = pixel_map(c);
    for (int j = 0; j < CHAR_HEIGHT; ++j) {
        for (int i = 0; i < CHAR_WIDTH; ++i) {
            Vector2 aux = {(pos.x*(CHAR_WIDTH))+i, (pos.y*CHAR_HEIGHT)+j};
            if((cMap[j] >> (CHAR_WIDTH - i - 1)) & 1)
                drawPixel(aux,foreground);
            else
                drawPixel(aux, background);
        }

    }

}

//Dibujar un string en cualquier parte
void drawString(Vector2 pos, char * str, Color foreground, Color background){
    Vector2 auxPos = {pos.x,pos.y};

    while((*str) != 0) {
        draw_char_with_background(auxPos, (*str), foreground, background);
        auxPos.x++;
        str++;
    }
}
