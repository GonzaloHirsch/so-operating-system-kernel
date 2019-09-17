#ifndef IRQ_DISPATCHER_H
#define IRQ_DISPATCHER_H

#include <stdint.h>

void irqDispatcher(uint64_t irq);

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