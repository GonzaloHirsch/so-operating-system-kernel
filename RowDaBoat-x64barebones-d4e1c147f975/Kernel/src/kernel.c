#include <stdint.h>
//#include <string.h>
#include <strings.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <irqDispatcher.h>
#include <time.h>
#include <keyboard.h>
#include <idtLoader.h>
#include <interrupts.h>
#include <syscall.h>
#include <videoDriver.h>
#include <console.h>
#include <pixelMap.h>
#include <exceptions.h>
#include <memManager.h>
#include <processes.h>
#include <scheduler.h>
#include "../include/processes.h"
#include "../include/scheduler.h"
#include "../include/console.h"
#include "../include/time.h"
#include "../include/semaphore.h"
#include "../include/intQueue.h"
#include "../include/pipes.h"
#include "../include/fileDescriptor.h"
#include "../include/readerDaemon.h"
#include <pipes.h>
#include <fileDescriptor.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

//Addresses a donde copia los modulos
static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;
static void * const memoryStartAddress = (void*)0x700000;

typedef int (*EntryPoint)();


void clearBSS(void * bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void goToUserland(){
	((EntryPoint)sampleCodeModuleAddress)();
}

void * initializeKernelBinary()
{
	char buffer[10];

	cpuVendor(buffer);

	//Llena con los addresses a donde copia los modulos
	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);

	clearBSS(&bss, &endOfKernel - &bss);

	print("Initializing Video Driver\n");
    initVideoDriver();
    print("Initializing Console\n");
    init_console();
    print("Initializing Memmanager list\n");
    //todo nuestro memmanager
    initializeMemManagerList(memoryStartAddress, 80*1048576); //80MB de memoria dinamica
    //initialize_list(memoryStartAddress, 80*1048576);
    print("Initializing Processes\n");
    initProcesses();
    print("Creating default fds\n"); //sacar despues
    createDefaultFds();
    print("Loading idt\n");
 	load_idt();
 	print("Loading exceptions\n");
	loadExceptions();
	print("Turning on rtc\n");
	turn_on_rtc();


	return getStackBase();
}

void semTest1(){

    const sem * testSem = openSemaphore("test");
    do{


        semWait(testSem);
        for(int i = 0; i<20; i++){
            print("Test1: %d\n", i);
            if(i==15)sleep(2000);
        }
        print("\n");
        semPost(testSem);
    }while(0);
    closeSemaphore(testSem);
}

void semTest2(){

    const sem * testSem = openSemaphore("test");
    do{


        semWait(testSem);
        for(int i = 0; i<10; i++){
            print("Test2: %d\n", i);
            if(i==9)sleep(1000);
        }
        print("\n");
        semPost(testSem);
    }while(0);
    closeSemaphore(testSem);
}

void semTest3() {

    const sem *testSem = openSemaphore("test");
    do{

        semWait(testSem);
        for (int i = 0; i < 5; i++) {
            print("Test3: %d\n", i);
            if(i==2)sleep(1000);
        }
        print("\n");
        semPost(testSem);
    }while(0);
    closeSemaphore(testSem);
}

void semTest4() {

    const sem *testSem = openSemaphore("test");
    do{

        semWait(testSem);
        for (int i = 0; i < 5; i++) {
            print("Test4: %d\n", i);
            if(i==2)sleep(1000);
        }
        print("\n");
        semPost(testSem);
    }while(0);
    closeSemaphore(testSem);
}

void pipeTest2(){
    char * name = "pipe1";
    char * ejemplo = "aaaaabbbbbbbbccccccccccccddddddddeeeeee";
    sleep(3000);
    print("Started writing\n");
    int fd = pipeFifo(name);
    write(fd, ejemplo, strlen(ejemplo));
    print("Finished writing\n");

    char * ejemplo2 = "pruebaasdfds         ssssssssss 2";
    print("Started writing 2\n");
    write(fd, ejemplo2, strlen(ejemplo2));
    print("Finished writing 2\n");

}

void pipeTest1(){
    char * name = "pipe1";
    char resultado[100];
    char * welcomeMessage = "Started reading\n";
    write(1,welcomeMessage,strlen(welcomeMessage));
    int fd = pipeFifo(name);
    read(fd, resultado,100);
    print(resultado);print("\n");
    print("Finished reading\n");

    char resultado2[100];
    print("Started reading 2\n");
    read(fd, resultado2,100);
    print(resultado2);print("\n");
    print("Finished reading 2\n");


}




void mainFunction(){
    /*
    Process p1 = newProcess("semTest1", (uint64_t) semTest1, 2, FOREGROUND);
    Process p2 = newProcess("semTest2", (uint64_t) semTest2, 2, FOREGROUND);
    Process p3 = newProcess("semTest3", (uint64_t) semTest3, 2, FOREGROUND);
    Process p4 = newProcess("semTest4", (uint64_t) semTest4, 2, FOREGROUND);
    Process p5 = newProcess("semTest1", (uint64_t) semTest1, 2, FOREGROUND);
    Process p6 = newProcess("semTest2", (uint64_t) semTest2, 2, FOREGROUND);
    Process p7 = newProcess("semTest3", (uint64_t) semTest4, 2, FOREGROUND);
       newPCB(p1);
       newPCB(p2);
       newPCB(p3);
       newPCB(p4);
       newPCB(p5);
       newPCB(p6);
    newPCB(p7);
    Process p14 = newProcess("pipeTest1", (uint64_t) pipeTest1, 2, FOREGROUND);
    Process p15 = newProcess("pipeTest2", (uint64_t) pipeTest2, 2, FOREGROUND);
    newPCB(p14);
    newPCB(p15);
    */

    Process p1 = newProcess("pipeTest1", (uint64_t) pipeTest1, 2, FOREGROUND);
    Process p2 = newProcess("pipeTest2", (uint64_t) pipeTest2, 2, FOREGROUND);


    



    while(1){}
}

void testIntQueue(){
    IntQueue q = newQueue(35);
    for(int i = 0; i<37; i++){
        enqueue(q, i);
    }
    for(int i = 0; i<37; i++){
        print("%d - ", dequeue(q));
    }
    print("\n");

}

void dup2Test1(){

    int tunnel = pipeFifo("dup2TestPipe");
    //lo que sigue es dup2
    setProcessFd(getPid(), 1, tunnel);

    char * message = "The other process is gonna read this";
    while(1){
        write(STDOUT_FD, message, strlen(message));
        sleep(2000);
    }

}

void dup2Test2(){

    int tunnel = pipeFifo("dup2TestPipe");
    //lo que sigue es dup2
    setProcessFd(getPid(), 0, tunnel);

    char result[100];

    while(1) {
        read(STDIN_FD, result, 100);
        if(strlen(result)>0) write(STDOUT_FD, result, 100);
    }
}

void init(){
    //testIntQueue();

    print("Starting kernel main\n");
    sleep(2);

    //initializeReaderDaemon();

    //Process dup21 = newProcess("dup21", dup2Test1, 2, BACKGROUND);
    //Process dup22 = newProcess("dup22", dup2Test2, 2, BACKGROUND);

    //newPCB(dup22);
    //newPCB(dup21);

    Process shellProcess = newProcess("shell", (uint64_t) sampleCodeModuleAddress, 10, FOREGROUND);
    newPCB(shellProcess);

    //Process mainProcess = newProcess("mainProcess", (uint64_t) mainFunction, 5, FOREGROUND);
    //newPCB(mainProcess);
}

int main()
{


    testIntQueue();

    print("Starting kernel main\n");
    sleep(2);

    Process initProcess = newProcess("init", init, 1, BACKGROUND);
    newPCB(initProcess);

	print("kernel stop\n");
    return 0;

}
