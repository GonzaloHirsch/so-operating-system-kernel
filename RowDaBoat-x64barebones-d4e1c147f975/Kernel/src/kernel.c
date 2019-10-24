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
#include "../include/readerDaemon.h"
#include "../include/intPairQueue.h"

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;
extern void forceChangeProcess();

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
    setProcessState(getCurrentProcess(), STATE_TERMINATED);
    forceChangeProcess();
    closeSemaphore(testSem);
}

void mainFunction(){

    Process p1 = newProcess("semTest1", (uint64_t) semTest1, 2, FOREGROUND);
    Process p2 = newProcess("semTest2", (uint64_t) semTest2, 2, FOREGROUND);
    Process p3 = newProcess("semTest3", (uint64_t) semTest3, 2, FOREGROUND);
    Process p4 = newProcess("semTest4", (uint64_t) semTest4, 2, FOREGROUND);
    Process p5 = newProcess("semTest1", (uint64_t) semTest1, 2, FOREGROUND);
    Process p6 = newProcess("semTest2", (uint64_t) semTest2, 2, FOREGROUND);
    Process p7 = newProcess("semTest3", (uint64_t) semTest4, 2, FOREGROUND);
    /*
    Process p8 = newProcess("semTest4", (uint64_t) semTest3, 2, BACKGROUND);
    Process p9 = newProcess("semTest4", (uint64_t) semTest4, 2, BACKGROUND);
    Process p10 = newProcess("semTest1", (uint64_t) semTest1, 2, BACKGROUND);
    Process p11 = newProcess("semTest2", (uint64_t) semTest2, 2, BACKGROUND);
    Process p12 = newProcess("semTest3", (uint64_t) semTest3, 2, BACKGROUND);
    Process p13 = newProcess("semTest4", (uint64_t) semTest4, 2, BACKGROUND);
    */

    newPCB(p1);
    newPCB(p2);
    newPCB(p3);
    newPCB(p4);
    newPCB(p5);
    newPCB(p6);
    newPCB(p7);
    /*
    newPCB(p8);
    newPCB(p9);
    newPCB(p10);
    newPCB(p11);
    newPCB(p12);
    newPCB(p13);
     */
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

void testIntPairQueue(){

    IntPairQueue q = newIntPairQueue(35);
    for(int i = 0; i<37; i++){
        enqueuePair(q, i, i*5);
    }
    struct intPair it;
    for(int i = 0; i<37; i++){
        it = dequeuePair(q);
        print("Pair: (%d, %d)\n",it.first, it.second);
    }
    for(int i = 0; i<37; i++){
        enqueuePair(q, i, i*5);
    }
    for(int i = 0; i<37; i++){
        setValue(q, i, getValue(q, i)/5*4);
    }
    for(int i = 0; i<37; i++){
        it = dequeuePair(q);
        print("Pair: (%d, %d)\n",it.first, it.second);
    }
    print("\n");
}

void init(){
    //testIntQueue();

    print("Starting kernel main\n");
    sleep(2);

    initializeReaderDaemon();

    Process shellProcess = newProcess("shell", (uint64_t) sampleCodeModuleAddress, 10, FOREGROUND);
    newPCB(shellProcess);

    //Process mainProcess = newProcess("mainProcess", (uint64_t) mainFunction, 5, FOREGROUND);
    //newPCB(mainProcess);
}

int main()
{

    Process initProcess = newProcess("init", init, 1, BACKGROUND);
    newPCB(initProcess);



    //mFree(array);
    //goToUserland();
    for(int i = 0; i<10; i++)
        print("Hey I'm done here\n");
	print("kernel stop\n");
    return 0;

}
