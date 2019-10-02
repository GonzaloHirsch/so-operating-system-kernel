#include <gameGrid.h>

// ----------------------------------------------------------------------------------
// Este modulo se ocupa de dibujar todo lo relacionado al tablero
// Como los bordes, el puntaje y las instrucciones.
// ----------------------------------------------------------------------------------


#define BORDER_SIZE 5
#define CELL_SIZE 10
#define CELL_PADDING 1

static Vector2 GRID_SIZE = {95,65};
static Vector2 START_POS = {30,40};

static Color BORDER_COLOR = {126, 56, 120};

void drawBorder(){
  Vector2 hor = {CELL_SIZE * GRID_SIZE.x + 2, BORDER_SIZE};
  Vector2 ver = {BORDER_SIZE , CELL_SIZE * GRID_SIZE.y + BORDER_SIZE*2 + 2};

  // Vertical lines
  //Left
  Vector2 auxStart = {START_POS.x - BORDER_SIZE - 2, START_POS.y - BORDER_SIZE - 2};
  drawRect(auxStart, ver, BORDER_COLOR);

  //Right
  auxStart.x = START_POS.x + GRID_SIZE.x * CELL_SIZE;
  auxStart.y = START_POS.y - BORDER_SIZE - 2;
  drawRect(auxStart, ver, BORDER_COLOR);

  // Horizontal lines
  //Top
  auxStart.x = START_POS.x - 2;
  auxStart.y = START_POS.y - BORDER_SIZE - 2;
  drawRect(auxStart, hor, BORDER_COLOR);

  //Bottom
  auxStart.y = START_POS.y + GRID_SIZE.y * CELL_SIZE;
  drawRect(auxStart, hor, BORDER_COLOR);
}

static Color header = {43,87,151};

void drawHeader(){
  Vector2 size = {1080, 16};
  drawRect(ZeroVector, size, header);
  drawString(ZeroVector, "Esc --> EXIT  WASD --> MOVE" , COLOR_WHITE, header);
}

void drawPoints(int p){
  Vector2 pos = {50, 0};
  drawString(pos, "Time: " , COLOR_WHITE, header);
  pos.x += 8;
  char buf[10];
  itoa(p, buf, 10);
  drawString(pos,buf , COLOR_WHITE, header);
}

void drawCell(Vector2 cell, Color color) {
  Vector2 cellStart = {START_POS.x + cell.x * CELL_SIZE - CELL_PADDING, START_POS.y + cell.y * CELL_SIZE - CELL_PADDING};
  Vector2 cellSize = {CELL_SIZE - CELL_PADDING, CELL_SIZE - CELL_PADDING};
  drawRect(cellStart, cellSize, color);
}

void initGrid() {

  drawBorder();
  drawHeader();

}
