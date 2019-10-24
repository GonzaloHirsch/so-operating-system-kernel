#include <philosophers.h>
#include "../include/utils.h"

// Implementacion adapatada a partir de la de Tanenbaum

#define LEFT(i,c) ((i - 1)%c)
#define RIGHT(i,c) ((i + 1)%c)

static int state[MAX_PHILOSOPHER_COUNT];
static int philosophers[MAX_PHILOSOPHER_COUNT];
static int actualPhilosopherCount;
static sem * sems[MAX_PHILOSOPHER_COUNT];
static sem * mutex;
static int problemRunning;
static int eatingTimes[MAX_PHILOSOPHER_COUNT];
static int thinkingTimes[MAX_PHILOSOPHER_COUNT];

void philosopher();
void takeForks(int i);
void placeForks(int i);
void think(int i);
void eat(int i);
void check(int i);
int addPhilosopher();
int removePhilosopher();
void printTable();

void philosopher(){
  sys_wait_sem(mutex);
  int i = actualPhilosopherCount - 1;
  //printf("CREATE %d", i);
  sys_post_sem(mutex);
  //print("Post mutex\n");
  while (problemRunning){
    think(i);
    //print("Thinking\n");
    takeForks(i);
    eat(i);
    //print("Eating\n");
    placeForks(i);
  }
}

void takeForks(int i){
  sys_wait_sem(mutex);
  state[i] = HUNGRY;
  //printf("TAKE %d", i);
  //print("TAKE\n");
  printTable();
  //print("CHECK SELF\n");
  check(i);
  sys_post_sem(mutex);
  sys_wait_sem(sems[i]);
}

void placeForks(int i){
  sys_wait_sem(mutex);
  state[i] = THINKING;
  //print("PLACE\n");
  printTable();
  //print("CHECK LEFT\n");
  check(LEFT(i, actualPhilosopherCount));
  //print("CHECK RIGHT\n");
  check(RIGHT(i, actualPhilosopherCount));
  sys_post_sem(mutex);
}

void think(int i){
  goToSleep(thinkingTimes[i]);
}

void eat(int i){
  goToSleep(eatingTimes[i]);
}

void check(int i){
  //print("CHECK\n");
  printf("%d %d", i, actualPhilosopherCount);
  if (state[i] == HUNGRY && state[LEFT(i, actualPhilosopherCount)] != EATING && state[RIGHT(i, actualPhilosopherCount)] != EATING){
    //print("CHECK OK\n");
    state[i] = EATING;
    printTable();
    sys_post_sem(sems[i]);
  }
}

void philosopherProblem(){
  problemRunning = 1;
  createThinkingTimes();
  createEatingTimes();
  mutex = sys_create_sem("philo_mutex");

  for (int i = 0; i < BASE_PHILOSOPHER_COUNT; i++){
    goToSleep(30);
    printf("About to create %d", i);
    addPhilosopher();
  }

  print("CREATED");
  int res;

  while(problemRunning){
    char key = getKeyNonBlocking();
    switch (key) {
      case 'a':
        res = addPhilosopher();
        if (res == -1){
          print("Can\'t add another philosopher. Maximum 10 philosophers.\n");
        } else {
          print("A new philosopher joined!\n");
        }
      break;
      case 'd':
        res = removePhilosopher();
        if (res == -1){
          print("Can\'t remove another philosopher. Minimum 5 philosophers.\n");
        } else {
          print("One philosopher has left!\n");
        }
      break;
      case 'q':
        problemRunning = 0;
      break;
    }
  }
}

int addPhilosopher(){
  if (actualPhilosopherCount + 1 > MAX_PHILOSOPHER_COUNT){
    return -1;
  }
  sys_wait_sem(mutex);
  printf("creating %d", actualPhilosopherCount);
  char num[3];
  itoa(actualPhilosopherCount, num, 10);
  char name[6 + 3 + 1] = {0};
  concat(name, "philo_");
  concat(name + 6, num);
  actualPhilosopherCount++;
  state[actualPhilosopherCount - 1] = THINKING;
  sems[actualPhilosopherCount - 1] = sys_create_sem(name);
  int pid = sys_new_process(name, (uint64_t) philosopher, 2, FOREGROUND);
  philosophers[actualPhilosopherCount - 1] = pid;
  sys_post_sem(mutex);
  return 0;
}

int removePhilosopher(){
  if (actualPhilosopherCount - 1 < BASE_PHILOSOPHER_COUNT){
    return -1;
  }
  sys_wait_sem(mutex);
  actualPhilosopherCount--;
  sys_kill(philosophers[actualPhilosopherCount]);
  philosophers[actualPhilosopherCount] = 0;
  sys_post_sem(mutex);
  return 0;
}

void createEatingTimes(){
  for (int i = 0; i < MAX_PHILOSOPHER_COUNT; i++){
    eatingTimes[i] = 40 + 2 * i;
  }
}

void createThinkingTimes(){
  for (int i = 0; i < MAX_PHILOSOPHER_COUNT; i++){
    thinkingTimes[i] = 30 + 3 * i;
  }
}

void printTable(){
  int totalCharCount = (actualPhilosopherCount * 3) + 2;
  char table[totalCharCount];
  for (int i = 0; i < actualPhilosopherCount; i++){
    switch(state[i]){
      case HUNGRY:
        table[(3 * i)] = '(';
        table[(3 * i) + 1] = 'H';
        table[(3 * i) + 2] = ')';
      break;
      case THINKING:
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
  table[totalCharCount - 2] = '\n';
  table[totalCharCount - 1] = 0;
  print(table);
}
