#include <philosophers.h>

#define LEFT(i, c) (i - 1)%c
#define RIGHT(i, c) (i + 1)%c

static int state[MAX_PHILOSOPHER_COUNT];
static int philosophers[MAX_PHILOSOPHER_COUNT];
static int actualPhilosopherCount;
static int sem;
static int mutex;
static int problemRunning;

void philosopher(){
  // Reduce mutex
  int i = actualPhilosopherCount;
  // Increment mutex
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
  printTable();
  // Increment mutex
  // Decrement semaphore
}

void placeForks(int i){
  // Reduce mutex
  state[i] = THINKING;
  check(LEFT(i, actualPhilosopherCount));
  check(RIGHT(i, actualPhilosopherCount));
  printTable();
  // Increment mutex
}

void think(){
  goToSleep(30);
}

void eat(){
  goToSleep(40);
}

void check(int i){
  if (state[i] == HUNGRY && state[LEFT(i, actualPhilosopherCount)] != EATING && state[RIGHT(i, actualPhilosopherCount)] != EATING){
    state[i] = EATING;
    //Increment semaphore
  }
}

void philosopherProblem(){
  problemRunning = 0;

  for (int i = 0; i < BASE_PHILOSOPHER_COUNT; i++){
    addPhilosopher();
  }

  int auxTime = sys_time(SECONDS);
  int delta = abosulteTimeDifference(relativeTime, auxTime);

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
  if (actualPhilosopherCount + 1 > MAX_PHILOSOPHER_COUNT){
    return -1;
  }
  actualPhilosopherCount++;
  int pid = sys_new_process("", (uint64_t) philosopher, 1, FOREGROUND);
  philosophers[actualPhilosopherCount - 1] = pid;
  state[actualPhilosopherCount - 1] = THINKING;
}

int removePhilosopher(){
  if (actualPhilosopherCount - 1 < BASE_PHILOSOPHER_COUNT){
    return -1;
  }
  actualPhilosopherCount--;
  sys_kill(philosophers[actualPhilosopherCount]);
  philosophers[actualPhilosopherCount] = 0;
}

void printTable(){
  int totalCharCount = (actualPhilosopherCount * 3) + 1;
  char table[totalCharCount];
  for (int i = 0; i < actualPhilosopherCount; i++){
    switch(state[i]){
      case HUNGRY:
        table[(3 * i)] = '(';
        table[(3 * i) + 1] = 'H';
        table[(3 * i) + 2] = ')';
      break;
      case THINKINH:
        table[(3 * i)] = '(';
        table[(3 * i) + 1] = 'T';
        table[(3 * i) + 2] = ')';
      break;
      case EATING:
        table[(3 * i)] = '(';
        table[(3 * i) + 1] = 'E';
        table[(3 * i) + 2] = ')';
      break;
    }
  }
  table[totalCharCount - 1] = 0;
  print(table);
}
