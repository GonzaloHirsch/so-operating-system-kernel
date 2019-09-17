//
// Created by nacho on 12/06/19.
//

#ifndef ARQUI_TPE_STRINGS_H
#define ARQUI_TPE_STRINGS_H

#include <stdint.h>

int strlen(char * str);

uint32_t int_to_string(uint64_t value, char * buffer, uint32_t base);

#endif //ARQUI_TPE_STRINGS_H
