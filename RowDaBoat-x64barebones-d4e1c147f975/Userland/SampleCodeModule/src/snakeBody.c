#include <snakeBody.h>

// ----------------------------------------------------------------------------------
// Este modulo de se ocupa de actualizar y manejar la viborita
// ----------------------------------------------------------------------------------

#define MAX_BODY_SIZE 400

static Vector2 snake[MAX_BODY_SIZE] = {{0,0}};
static Vector2 GRID_SIZE = {95,65};

static int snakeSize;

static int head;
static int tail;

static Vector2 direction;

void resetSnakeBody(){
  direction.x = 1;
  direction.y = 0;

  snakeSize = 25;
  head = 0;
  tail = snakeSize - 1;

}

void drawSnake(){
  for (int i = 0; i < snakeSize; i++) {
    drawCell(snake[i], COLOR_WHITE);
  }
}

void setSnakeCell(int index, Vector2 position){
  snake[index].x = position.x;
  snake[index].y = position.y;
}

void growSnake(){
  for (int i = snakeSize -1; i >= tail; i--)
  {
    snake[i+1].x = snake[i].x;
    snake[i+1].y = snake[i].y;
  }
  snakeSize++;
  head++;
  tail++;
}

void initSnakeBody(Vector2 startPos){
  Vector2 auxPos = {startPos.x, startPos.y};
  snakeSize = 25;
  for (int i = 0; i < snakeSize; i++){
    setSnakeCell(i, auxPos);
    auxPos.x--;
  }

  drawSnake();

}

void updateSnake(){
  drawCell(snake[tail], COLOR_BLACKED);
  snake[tail].x = snake[head].x + direction.x;
  snake[tail].y = snake[head].y + direction.y;
  drawCell(snake[tail], COLOR_WHITE);

  head = tail;
  tail = (tail - 1) >= 0 ? tail - 1 : snakeSize - 1;
}

void updateDirection(char key) {
  switch(key){
    case 'W':
    case 'w':
      if(direction.y != 1){
        direction.y = -1;
        direction.x = 0;
      }
      break;
    case 'S':
    case 's':
      if(direction.y != -1){
        direction.y = 1;
        direction.x = 0;
      }
      break;
    case 'A':
    case 'a':
      if(direction.x != 1){
        direction.x = -1;
        direction.y = 0;
      }
        break;
    case 'D':
    case 'd':
      if(direction.x != -1){
        direction.x = 1;
        direction.y = 0;
      }
      break;
    case 'l':
      growSnake();
      break;
    }
}

int isSnakeColliding(){
    if(snake[head].x + direction.x < 0 || snake[head].y + direction.y < 0 || snake[head].x + direction.x >= GRID_SIZE.x || snake[head].y + direction.y >= GRID_SIZE.y)
        return 1;
    else {
        for (int i = 0; i < snakeSize; ++i)
        {
            if(i != head && snake[i].x == snake[head].x && snake[i].y == snake[head].y){
                if (head != snakeSize - 1 && i != 0)
                  return 1;
            }
        }
    }

    return 0;
}
