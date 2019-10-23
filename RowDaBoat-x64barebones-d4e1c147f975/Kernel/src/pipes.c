#include <pipes.h>
#include <memManager.h>

typedef struct pipeType{

    char name[100];
    int fdRead;
    int fdWrite;

}pipeCDT;

static Pipe pipeList[MAX_PIPES];










