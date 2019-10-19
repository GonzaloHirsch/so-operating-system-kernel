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
    print("Loading idt\n");
 	load_idt();
 	print("Loading exceptions\n");
	loadExceptions();
	print("Turning on rtc\n");
	turn_on_rtc();


	return getStackBase();
}


int main()
{


    print("Starting kernel main\n");
    sleep(2);

    Process shellProcess = newProcess("shell", (uint64_t) sampleCodeModuleAddress, 10, FOREGROUND);
    newPCB(shellProcess);

    //mFree(array);
    //goToUserland();
    for(int i = 0; i<10; i++)
        print("Hey I'm done here\n");
	print("kernel stop\n");
    return 0;

}
