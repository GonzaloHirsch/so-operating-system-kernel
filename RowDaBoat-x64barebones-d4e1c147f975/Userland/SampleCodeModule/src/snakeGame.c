#include <snakeGame.h>

// ----------------------------------------------------------------------------------
// Este modulo es el modulo principal del juego.
// Desde aca se inicializa el mismo, se maneja el presionado de teclas
// y se maneja el framerate.
// ----------------------------------------------------------------------------------

static uint64_t frame = 0;
static uint64_t lastFrame = 0;

static int isRunning;
static int winner;

//Tiempo de vida
static int totalTimeAlive = 0;
//Tiempo entre incrementos de velocidad y tamaño
static int partialTime = 0;
//Tiempo relativo, porque recuperamos los segundos del sistema, que no empieza en 0
static int relativeTime = 0;

//Cantidad de ticks que se restan a los esperados para el incremento
static int velocityIncrements = 2;
//Ticks esperados para hacer un incremento
static int expectedFrame = 18;


void onExit();
void nextFrame();
int abosulteTimeDifference(int before, int after);

void resetVariables(){
  winner = 1;

  totalTimeAlive = 0;
  partialTime = 0;
  relativeTime = sys_time(SECONDS);
  expectedFrame = 18;

  resetSnakeBody();
}

int initSnakeGame(){
  isRunning = 1;
  clearScreen();
  initGrid();

  resetVariables();

  Vector2 startPos = {30, 10};

  initSnakeBody(startPos);

  nextFrame();

  onExit();

  return totalTimeAlive;
}

void updateInput(){
  char key = getKey();
  switch (key) {
    case 96:
    case 126:
      isRunning = 0;
    break;
  }
  updateDirection(key);
}

void Update(){
  int auxTime = sys_time(SECONDS);
  int delta = abosulteTimeDifference(relativeTime, auxTime);
  relativeTime = auxTime;
  totalTimeAlive += delta;
  partialTime += delta;

  if (partialTime >= 15){
    growSnake();
    sys_beep(800, 5);
    if (expectedFrame - velocityIncrements >= 1){
      expectedFrame -= velocityIncrements;
    } else {
      //Velocidad maxima
      expectedFrame = 0;
    }
    partialTime = 0;
  }

  updateInput();
  if(isSnakeColliding()){
    isRunning = 0;
    winner = 0;
  }
  updateSnake();
  drawPoints((int)totalTimeAlive);
}

int abosulteTimeDifference(int before, int after){
  int result = 0;
  if (after < before){
    result = 60 - before + after;
  } else {
    result = after - before;
  }
  return result;
}

void nextFrame(){
  _sti();
  lastFrame = sys_get_ticks();
  while(isRunning){
    frame = sys_get_ticks();
    if (frame - lastFrame > expectedFrame){
      lastFrame = frame;
      Update();
    }
    _hlt();
  }
}

void onExit(){
  clearScreen();
}
