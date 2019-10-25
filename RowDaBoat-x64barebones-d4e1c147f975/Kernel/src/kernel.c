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
#include <buddyManager.h>
#include <processes.h>
#include <scheduler.h>
#include "../include/processes.h"
#include "../include/scheduler.h"
#include "../include/console.h"
#include "../include/time.h"
#include "../include/semaphore.h"
#include "../include/intQueue.h"
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
    print("Initializing Memmanager\n");
    //todo nuestro memmanager
    initializeMemManager(memoryStartAddress, 80*1048576); //80MB de memoria dinamica
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

void pipeTest1(){
    char * name = "pipe1";
    char * ejemplo = "aaaaabbbbbbbbccccccccccccddddddddeeeeee";
    print("Started writing\n");
    int fd = pipeFifo(name);
    write(fd, ejemplo, strlen(ejemplo));
    print("Finished writing\n");

    char * ejemplo2 = "prueba 2";
    print("Started writing 2\n");
    write(fd, ejemplo2, strlen(ejemplo2));
    print("Finished writing 2\n");

}

void pipeTest2(){
    char * name = "pipe1";
    char resultado[100];
    print("Started reading\n");
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

    Process p1 = newProcess("pipeTest1", (uint64_t) pipeTest1, 2, FOREGROUND);
    Process p2 = newProcess("pipeTest2", (uint64_t) pipeTest2, 2, FOREGROUND);
    

    newPCB(p2);
    newPCB(p1);
   
    while(1){}
}


int main()
{

    print("Starting kernel main\n");
    sleep(2);

    Process mainProcess = newProcess("mainProcess", (uint64_t) mainFunction, 5, FOREGROUND);
    newPCB(mainProcess);



	print("kernel stop\n");
    return 0;
}