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
    initializeMemManagerList(memoryStartAddress, 80*1048576); //80MB de memoria dinamica
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

void testFunction1(){
    int i = 0;
    while((i++)<10) {
        print("Hello World!\n");
    }
}

void testFunction2(){
    int i = 0;
    while((i++)<10) {
        print("Trello world!\n");
    }
    sleep(2000);
}

void testFunction3(){
    int i = 0;
    while((i++)<10) {
        print("Return world!\n");
    }
    sleep(2000);
}

int main()
{

    while(1){

    }
    print("Starting kernel main\n");
    sleep(2);
    Process p1 = newProcess("function1", (uint64_t) &testFunction1, 0);
    Process p2 = newProcess("function2", (uint64_t) &testFunction2, 0);
    Process p3 = newProcess("function3", (uint64_t) &testFunction3, 0);
    Process p4 = newProcess("shell", (uint64_t) sampleCodeModuleAddress, 0);
    newPCB(p1);
    newPCB(p2);
    newPCB(p3);
    newPCB(p4);
    //mFree(array);
    //goToUserland();
    for(int i = 0; i<10; i++)
        print("Hey I'm done here\n");
	print("kernel stop\n");
    return 0;

}
