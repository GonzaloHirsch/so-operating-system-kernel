#ifndef TIME_H
#define TIME_H

#define REGULAR_TICK_TIME 55

//#include <lib.h>
#include <io_read.h>
#include <interrupts.h>
#include <lib.h>

//source
//https://wiki.osdev.org/CMOS#Getting_Current_Date_and_Time_from_RTC
#define SECONDS_REGISTER 0x00
#define MINUTES_REGISTER 0x02
#define HOURS_REGISTER 0x04
#define WEEKDAY_REGISTER 0x06
#define DAY_OF_MONTH_REGISTER 0x07
#define MONTH_REGISTER 0x08
#define YEAR_REGISTER 0x09
#define CENTURY_REGISTER 0x32

#define SECONDS 0
#define MINUTES 1
#define HOURS 2
#define WEEKDAY 3
#define DAY_OF_MONTH 4
#define MONTH 5
#define YEAR 6
#define CENTURY 7

void timer_handler();
int ticks_elapsed();
void sleep(uint64_t millis);
void timer_wait(int ticks);

/*
void ncPrint(const char * string);
void ncPrintChar(char character);
void ncNewline();
void ncPrintDec(uint64_t value);
void ncPrintHex(uint64_t value);
void ncPrintBin(uint64_t value);
void ncPrintBase(uint64_t value, uint32_t base);
void ncClear();
*/
int get_time(int selector);

#endif
