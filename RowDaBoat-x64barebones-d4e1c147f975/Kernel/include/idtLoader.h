#ifndef IDT_LOADER_H
#define IDT_LOADER_H

#include <stdint.h>

typedef struct {
  uint16_t offset_l;
  uint16_t selector;
  uint8_t  cero;
  uint8_t  access;
  uint16_t offset_m;
  uint32_t offset_h;
  uint32_t other_cero;
} DESCR_INT;

#define ACS_PRESENT     0x80            /* segmento presente en memoria */
#define ACS_CSEG        0x18            /* segmento de codigo */
#define ACS_DSEG        0x10            /* segmento de datos */
#define ACS_READ        0x02            /* segmento de lectura */
#define ACS_WRITE       0x02            /* segmento de escritura */
#define ACS_IDT         ACS_DSEG
#define ACS_INT_386 	0x0E		/* Interrupt GATE 32 bits */
#define ACS_INT         ( ACS_PRESENT | ACS_INT_386 )

void setup_IDT_entry (int index, uint64_t offset);
void load_idt();

#endif