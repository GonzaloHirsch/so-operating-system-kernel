#include "../include/readerDaemon.h"
#include "../include/processes.h"
#include "../include/keyboard.h"
#include "../include/scheduler.h"

extern int forceChangeProcess();


static int currentLength;
static char * currentBuffer;
static Process daemon;
//static int ch = 0;

static void daemonMain();


void initializeReaderDaemon(){

    daemon = newProcess("readerDaemon", (uint64_t) daemonMain, 2, BACKGROUND);
    newPCB(daemon);
}

void wakeUpDaemon(){
    setProcessState(daemon, STATE_READY);
}

void setCurrentLength(int length){
    currentLength = length;
}

void setCurrentBuffer(char * buffer){
    currentBuffer = buffer;
}

static void daemonMain(){
    /*
    ESTO ES CODIGO DEPRECADO, LO COMENTAMOS PARA EVITAR EL WARNING QUE TIRABA
    LO DEJAMOS PORQUE SE MENCIONA EN EL INFORME

    const sem * sem0 = openSemaphore("sysReadDataMutex");
    const sem * sem1 = openSemaphore("sysReadMutex");

    //setteo el valor inicial de sysReadMutex a 0
    semWait(sem1);

    while(1){

        setProcessStateByPid(getPid(), STATE_BLOCKED);
        forceChangeProcess();

        for (int i = 0; i < currentLength;){
            ch = getChar();
            if(ch!=NULL && ch!=EOF) {
                *(currentBuffer + i) = ch;
                i++;
            }
            ch = 0;
        }

        semPost(sem1);
        semPost(sem0);

    }
*/
}
