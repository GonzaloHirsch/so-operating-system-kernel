#ifndef INTERRUPTS_H
#define INTERRUPTS_H

void _irq00Handler();
void _irq01Handler();
void _irq80Handler();

void _cli();
void _sti();
void halt();

void pic_master_mask(uint8_t mask);

void pic_slave_mask(uint8_t mask);

/*
void ncPrint(const char * string);
void ncPrintChar(char character);
void ncNewline();
void ncPrintDec(uint64_t value);
void ncPrintHex(uint64_t value);
void ncPrintBin(uint64_t value);
void ncPrintBase(uint64_t value, uint32_t base);
void ncClear();
*/

#endif