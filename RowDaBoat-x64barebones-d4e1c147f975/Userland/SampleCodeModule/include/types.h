//
// Created by nacho on 12/06/19.
//

#ifndef ARQUI_TPE_TYPES_H
#define ARQUI_TPE_TYPES_H

typedef struct Color{
    unsigned char r;
    unsigned char g;
    unsigned char b;
}Color;

typedef struct Vector2{
    int x;
    int y;
}Vector2;

static const Vector2 ZeroVector = {0,0};

static const Color COLOR_WHITE = {255, 255, 255};
static const Color COLOR_BLACKED = {29, 29, 29};

#endif //ARQUI_TPE_TYPES_H
