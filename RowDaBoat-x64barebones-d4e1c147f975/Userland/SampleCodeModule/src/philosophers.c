#include <philosophers.h>
#include "../include/syscalls.h"

// Implementacion adapatada a partir de la de Tanenbaum

static int state[MAX_PHILOSOPHER_COUNT];
static int philosophers[MAX_PHILOSOPHER_COUNT];
static int actualPhilosopherCount;
static sem * sems[MAX_PHILOSOPHER_COUNT];
static sem * mutex;
static int problemRunning;
static int eatingTimes[MAX_PHILOSOPHER_COUNT];
static int thinkingTimes[MAX_PHILOSOPHER_COUNT];

static int maxIters = 500;
static int actualIters = 0;

void philosopher();
void takeForks(int i);
void placeForks(int i);
void think(int i);
void eat(int i);
void check(int i);
int addPhilosopher();
int removePhilosopher();
void printTable();
int left(int i, int mod);
int right(int i, int mod);

void philosopher(){
  //sys_wait_sem(mutex);
  int i = actualPhilosopherCount - 1;
  //sys_post_sem(mutex);
  while (problemRunning){
    think(i);
    // Trata de tomar los tenedores, si no puede se bloquea por el semaforo
    takeForks(i);
    eat(i);
    // Trata de dejar los tenedores, si no puede se bloquea por el semaforo
    placeForks(i);
  }
}

void takeForks(int i){
  sys_wait_sem(mutex);
  // Lo deja en HUNGRY para el checkeo de poder tomar el tenedor
  state[i] = HUNGRY;
  // Hace el check de los vecinos para ver si puede tomar el tenedor
  check(i);
  sys_post_sem(mutex);
  sys_wait_sem(sems[i]);
}

void placeForks(int i){
  sys_wait_sem(mutex);
  // Lo devuelve al estado de THINKING
  state[i] = THINKING;
  // Verifica si el philo a la izquierda puede comer
  check(left(i, actualPhilosopherCount));
  // Verifica si el philo a la derecha puede comer
  check(right(i, actualPhilosopherCount));
  sys_post_sem(mutex);
}

void think(int i){
  goToSleep(thinkingTimes[i]);
}

void eat(int i){
  goToSleep(eatingTimes[i]);
}

void check(int i){
  //printf("CHECK %d\n", i);
  if (state[i] == HUNGRY && state[left(i, actualPhilosopherCount)] != EATING && state[right(i, actualPhilosopherCount)] != EATING){
    //printf("CHECK EAT %d\n", i);
    state[i] = EATING;
    //printTable();
    sys_post_sem(sems[i]);
  }
}

void philosopherProblem(){
  problemRunning = 1;
  createThinkingTimes();
  createEatingTimes();
  mutex = sys_create_sem("philo_mutex");

  for (int i = 0; i < BASE_PHILOSOPHER_COUNT; i++){
    addPhilosopher();
    goToSleep(30);
  }

  int res;
  actualIters = 0;

  while(problemRunning){
    char key = getKey();
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
        print("Program Finished!\n");
        problemRunning = 0;
      break;
    }
    if (actualIters >= maxIters){
      printTable();
      actualIters = 0;
    } else {
      actualIters++;
    }
  }

  int pid = sys_get_pid();
  int ppid = sys_get_p_pid(pid);
  sys_unblock(ppid);
  sys_kill(pid);
}

int addPhilosopher(){
  if (actualPhilosopherCount + 1 > MAX_PHILOSOPHER_COUNT){
    return -1;
  }
  sys_wait_sem(mutex);
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

int left(int i, int mod){
  int res = (i - 1) % mod;
  if (res < 0){
    return mod + res;
  } else {
    return res;
  }
}

int right(int i, int mod){
  int res = (i + 1) % mod;
  return res;
}
