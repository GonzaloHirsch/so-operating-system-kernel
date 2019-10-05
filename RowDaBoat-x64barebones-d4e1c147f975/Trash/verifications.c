//
// Created by root on 6/14/19.
//

#include <exceptionDispatcher.h>

void verifyDivZero();
void verifyInvOpcode();
static void tBI(); //(to Be Implemented

static void (*verifFuncs[])() = {verifyDivZero, tBI, tBI, tBI, tBI, tBI, verifyInvOpcode, tBI, tBI, tBI, tBI, tBI, tBI};


void tBI(){
    return;
}

void verifyDivZero(){

    setVerifying();
    setExceptionAssert(0);
    int i;
    i/=0;
    print("Sali");
   // __asm__("mov $1, %rdi;"
  //          "call printInteger;");


    sleep(1000);
    clearVerifying();
    return;
}

void verifyInvOpcode(){
    setVerifying();
    setExceptionAssert(6);

    uint64_t invalid= 0xFFFFFFFFFFFF;
    uint64_t * ptr = &invalid;
    ((void(*)())ptr)();

    print("Sali");
    sleep(1000);
    clearVerifying();
    return;
}
