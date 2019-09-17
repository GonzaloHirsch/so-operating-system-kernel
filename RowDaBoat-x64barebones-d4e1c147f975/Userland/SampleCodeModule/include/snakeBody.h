#ifndef SNAKE_BODY_H
#define SNAKE_BODY_H

#include <types.h>
#include <gameGrid.h>
#include <graphicsLibrary.h>
#include <utils.h>

void initSnakeBody(Vector2 startPos);

void updateSnake(void);

void updateDirection(char key);

int isSnakeColliding(void);

void resetSnakeBody(void);

void growSnake(void);

#endif
