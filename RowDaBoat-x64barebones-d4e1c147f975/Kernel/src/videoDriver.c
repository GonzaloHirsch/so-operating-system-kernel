//
// Created by nacho on 10/06/19.
//

#include <videoDriver.h>

// Struct taken from https://wiki.osdev.org/Getting_VBE_Mode_Info
typedef struct __attribute__((packed)) {
    uint16_t attributes;
    uint8_t win_A,win_B;
    uint16_t granularity;
    uint16_t winsize;
    uint16_t segment_A, segment_B;
    uint32_t real_fct_ptr;
    uint16_t pitch;

    uint16_t x_res, y_res; // Screen X and Y resolution
    uint8_t w_char, y_char, planes, bpp, banks; // BPP - Bits Per Pixel
    uint8_t memory_model, bank_size, image_pages;
    uint8_t reserved0;

    uint8_t red_mask, red_position;
    uint8_t green_mask, green_position;
    uint8_t blue_mask, blue_position;
    uint8_t rsv_mask, rsv_position;
    uint8_t directcolor_attributes;

    uint32_t physbase;  // your LFB (Linear Framebuffer) address ;)
    uint32_t reserved1;
    uint16_t reserved2;
} ModeInfoBlock;

static ModeInfoBlock* infoBlock;

// Relevo de informacion de la pantalla
void initVideoDriver() {
    infoBlock = (ModeInfoBlock*)0x0000000000005C00;
}

// Checkear si la posicion esta dentro de la pantalla
int out_of_range_pixel(Vector2 pos) {
    return !((pos.x >= 0) && (pos.x <= infoBlock->x_res) && (pos.y >= 0) && (pos.y <= infoBlock->y_res));
}

// Dibujar un pixel a pantalla
void draw_pixel(Vector2 pos, Color color){
    if (out_of_range_pixel(pos))
        return;
    
    unsigned char * pixel_address;
    int bpp = infoBlock->bpp / 8;
    pixel_address = (unsigned char *) ((uint64_t)(infoBlock->physbase + pos.x * bpp  + pos.y * (infoBlock->x_res) * bpp));
    pixel_address[0] = color.b;
    pixel_address[1] = color.g;
    pixel_address[2] = color.r;
}


// Mover todas las lineas una para arriba
void move_all_lines_up(){
  void * source = (void *)((uint64_t)(infoBlock->physbase + (infoBlock->bpp/8) * infoBlock->x_res * CHAR_HEIGHT));
  void * dest = (void *)((uint64_t)infoBlock->physbase);
  int size = (((infoBlock->bpp/8) * (infoBlock->x_res-1) *  infoBlock->y_res)-(infoBlock->bpp/8) * infoBlock->x_res * CHAR_HEIGHT);
  infoBlock->physbase = ((uint64_t)(memcpy(dest, source, size)));
}


// Relevar color de pixel en cierta posicion
void get_pixel(Vector2 pos, Color* out){
    if (out_of_range_pixel(pos))
        return;

    unsigned char * pixel_address;
    int bpp = infoBlock->bpp / 8;
    pixel_address = (unsigned char *) ((uint64_t)(infoBlock->physbase + pos.x * bpp + pos.y * (infoBlock->x_res) * bpp));
    out->b = pixel_address[0];
    out->g = pixel_address[1];
    out->r = pixel_address[2];
}

// Obsoleto: Dibujar rectangulo
void draw_rect(Vector2 pos, Vector2 size, Color color){
    Vector2 auxPos = {0,0};
    for (auxPos.y = pos.y; auxPos.y < pos.y + size.x; auxPos.y++) {
        for (auxPos.x = pos.x; auxPos.x < pos.x + size.x; auxPos.x++) {
            draw_pixel(auxPos, color);
        }
    }
}

// Dibujar caracter en pantalla
// Se utiliza pixel_map, el cual contiene cada letra, pixel por pixel
void draw_char_with_background(Vector2 pos, char c, Color foreground, Color background){
    unsigned char * cMap = pixel_map(c);
    for (int j = 0; j < CHAR_HEIGHT; ++j) {
        for (int i = 0; i < CHAR_WIDTH; ++i) {
            Vector2 aux = {(pos.x*(CHAR_WIDTH))+i, (pos.y*CHAR_HEIGHT)+j};
            if((cMap[j] >> (CHAR_WIDTH - i - 1)) & 1) // NO se porque el -1, pero funciona...
                draw_pixel(aux,foreground);
            else
                draw_pixel(aux, background);
        }

    }

}

//Obsoleto: Dibujar string a pantalla
void draw_string_with_background(Vector2 pos, char * str, Color foreground, Color background){
    Vector2 auxPos = {pos.x,pos.y};

    while((*str) != 0) {
        draw_char_with_background(auxPos, (*str), foreground, background);
        auxPos.x++;
        str++;
    }
}

int getResX(){
    return infoBlock->x_res;
}
int getResY(){
    return infoBlock->y_res;
}
