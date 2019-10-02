//
// Created by nacho on 12/06/19.
//

#include <strings.h>

int strlen(char * str){
    int ret = 0;
    while(*(str+ret) != 0) ret++;
    return ret;
}

uint32_t int_to_string(uint64_t value, char * buffer, uint32_t base)
{
    char *p = buffer;
    char *p1, *p2;
    uint32_t digits = 0;

    //Se calcula el digito correspondiente para cada caracter
    do
    {
        uint32_t remainder = value % base;
        *p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
        digits++;
    }
    while (value /= base);

    // Se termina el string
    *p = 0;

    // Se revierte el string
    p1 = buffer;
    p2 = p - 1;
    while (p1 < p2)
    {
        char tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;
    }

    return digits;
}