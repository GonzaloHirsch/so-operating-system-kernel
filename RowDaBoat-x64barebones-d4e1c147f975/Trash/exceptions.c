//#include <idtLoader.h>
#include <exceptions.h>
#include <naiveConsole.h>


static void divZeroError();
static void invalidOpcodeError();
static void protectionFault();
static void pageFault();


//static void (*exceptionsArray[])() = {divZeroError, 0, 0, 0, 0, 0, invalidOpcodeError, 0, 0, 0, 0, 0, 0, 0, protectionFault, pageFault};


//void loadExceptions(){

    //setup_IDT_entry(0x00, (uint64_t) &_exception00Handler);
    //setup_IDT_entry(0x06, (uint64_t) &_exception06Handler);
    //setup_IDT_entry(0x13, (uint64_t) &_exception13Handler);
    //setup_IDT_entry(0x14, (uint64_t) &_exception14Handler);

//}

void exceptionDispatcher(uint64_t type){

  //  (*exceptionsArray[type])();
}

