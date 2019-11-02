#ifndef INTERRUPTS_H
#define INTERRUPTS_H

void _irq00Handler();
void _irq01Handler();
void _irq70Handler();
void _irq80Handler();

void _cli();
void _sti();
void halt();

void pic_master_mask(uint8_t mask);

void pic_slave_mask(uint8_t mask);

#endif