#include <philosophers.h>

#define LEFT(i, c) (i - 1)%c
#define RIGHT(i, c) (i + 1)%c

int state[MAX_PHILOSOPHER_COUNT];
int philosophers[MAX_PHILOSOPHER_COUNT];
int actualPhilosopherCount;
int sem;
int mutex;
int problemRunning;

void philosopher(int i){
  while (problemRunning){
    think();
    takeForks(i);
    eat();
    placeForks(i);
  }
}

void takeForks(int i){
  // Reduce mutex
  state[i] = HUNGRY;
  check(i);
  // Increment mutex
}

void placeForks(int i){
  state[i] = THINKING;
  check(LEFT(i, actualPhilosopherCount));
  check(RIGHT(i, actualPhilosopherCount));
}

void think(){
  goToSleep(750);
}

void eat(){
  goToSleep(750);
}

void check(int i){
  if (state[i] == HUNGRY && state[LEFT(i, actualPhilosopherCount)] != EATING && state[RIGHT(i, actualPhilosopherCount)] != EATING){
    state[i] = EATING;
    //Increment semaphore
  }
}

void philosopherProblem(){
  actualPhilosopherCount = 0;
  problemRunning = 0;

  for (int i = 0; i < BASE_PHILOSOPHER_COUNT; i++){
    addPhilosopher();
  }

  while(problemRunning){
    char key = getKey();
    switch (key) {
      case 'a':
        int state = addPhilosopher();
        if (state == -1){
          print("Can\'t add another philosopher. Maximum 10 philosophers.\n");
        }
      break;
      case 'd':
        int state = removePhilosopher();
        if (state == -1){
          print("Can\'t remove another philosopher. Minimum 5 philosophers.\n");
        }
      break;
      case 'q':
        problemRunning = 1;
      break;
    }
  }
}

int addPhilosopher(){
  if (actualPhilosopherCount - 1 < BASE_PHILOSOPHER_COUNT){
    return -1;
  }
  actualPhilosopherCount++;
  //Create process
}

int removePhilosopher(){
  if (actualPhilosopherCount + 1 > MAX_PHILOSOPHER_COUNT){
    return -1;
  }
  actualPhilosopherCount--;
  //Kill process
}
