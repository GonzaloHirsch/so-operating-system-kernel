//
// Created by click on 19/10/19.
//

#include "../include/syscallTests.h"
#include "../include/utils.h"
#include "../include/processes.h"


void testFunction1(){
    int i = 0;
    while((i++)<20) {
        print("AAAAAAAAAA\n");
    }
}

void testFunction2(){
    int i = 0;
    while((i++)<10) {
        print("BBBBBBBBBB\n");
    }
    sleep(2000);
}

void testFunction3(){
    int i = 0;
    while(1) {
        print("CCCCCCCCCC\n");
    }
    sleep(2000);
}

void processCreationTest(){
    print("*****Process Creation Test*****");
    int pid1 = sys_new_process("function1", (uint64_t) &testFunction1, 3, FOREGROUND);
    int pid2 = sys_new_process("function2", (uint64_t) &testFunction2, 2, FOREGROUND);
    int pid3 = sys_new_process("function3", (uint64_t) &testFunction3, 1, FOREGROUND);

}