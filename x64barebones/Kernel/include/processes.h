//
// Created by IR on 2/10/2019.
//

#ifndef SO_TP2_PROCESS_H
#define SO_TP2_PROCESS_H

#define MAX_NAME_LENGTH 128

typedef struct ProcessADT * Process;

enum State{STATE_READY, STATE_RUNNING, STATE_BLOCKED, STATE_TERMINATED};

#endif //SO_TP2_PROCESS_H
