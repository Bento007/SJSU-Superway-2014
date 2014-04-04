/*
 * SNAP.cpp
 *
 *  Created on: Mar 28, 2014
 *      Author: Bento007
 */

#include <SNAP.h>
#include <stdint.h>
#include <time.h>
#include "rtc.h"
#include "uart3.hpp"

/* To communicate with wireless network!
 *
 * #include "uart3.hpp"
 *
 * grab an instance of uart 3. //TODO: create a semifore to control UART3 between tasks
 *  Uart3& snap = Uart3::getInstance();
 *
 * READ:
 *  snap.gets(char* ,int NumberToRead ,int timeToWait);
 * Example:
 *  char str[16];   //where the incoming str is read into.
 *  snap.gets(str ,16 ,100); //read 16 characters from SNAP
 *
 * WRITE:
 *  snap.putChar(char, int timeToWait); //TODO: make compatible with char*
 * NOTE! Only one character can be put into the queue per putChar()
 * NOTE! SNAP will not receive the string unless you end the message with '\n'
 * Example:
 *  snap.putChar('H', 100);
 *  snap.putChar('E', 100);
 *  snap.putChar('L', 100);
 *  snap.putChar('L', 100);
 *  snap.putChar('O', 100);
 *  snap.putChar('\n', 100);// terminating character
 *
 */

/*command
 *     update = 'U',
 *     setETM = 'T',
 *     merge  = 'M',
 *     help   = 'E',
 *     getDest= 'D'
 */

//SNAP::SNAP(uint8_t priority):scheduler_task("wireless", 512, priority)//for scheduler
SNAP::SNAP()
{
   //add semaphore
   Uart3& wireless = Uart3::getInstance();
   wireless.init(19200,32,32);
   rtc_init (); //initialize RTC
   time = rtc_gettime ();
}

bool SNAP::send_Update(int location,int status,int speed)
{
    //add semaphore
    Uart3& wireless = Uart3::getInstance();
    if(wireless.printf("U%i:%i:%i,%i,%i,%i\n",time.hour, time.min, time.sec, location, status,speed))
        return true;
    else
        return false;
}

bool SNAP::send_Estimated_Time_to_Merge(int speed, int ticks)
{
    Uart3& wireless = Uart3::getInstance();
    int ETM = ticks/speed;
    if(wireless.printf("T%i",ETM))
        return true;
    else
        return false;
}
bool SNAP::send_Merge(int speed, int ticks)
{
    Uart3& wireless = Uart3::getInstance();
    int ETM = ticks/speed;
    if(wireless.printf("M%i",ETM))
        return true;
    else
        return false;
}
bool SNAP::send_Help(int status)
{
    Uart3& wireless = Uart3::getInstance();
    if(wireless.printf("E%i",status))
        return true;
    else
        return false;;
}
char SNAP::get_CMD()
{
    //add semaphore
    char cmd;
    Uart3& wireless = Uart3::getInstance();
    wireless.getChar(&cmd,100);
    return cmd;
}

bool SNAP::send_Test()
{
    //add semaphore
    Uart3& wireless = Uart3::getInstance();
    char test[] = {"Test"};
    wireless.putline(test,200);
    if(wireless.gets(test,6,200))
        return true;
    else
        return false;
}
int  SNAP::get_Dest()
{
    Uart3& wireless = Uart3::getInstance();
    char test[] ={"TEST"};
    wireless.putline("D",200);
    if(wireless.gets(test,6,200))
        return true;
    else
        return false;
}
void SNAP::get_Time()//TODO
{
    rtc_t update;
    Uart3& wireless = Uart3::getInstance();
    wireless.putChar('X',1000);
    wireless.scanf("%i %i %i %i %i %i %i %i", update.sec, update.min, update.hour,
            update.dow, update.day, update.month, update.year, update.doy);
    rtc_settime (&update);
}
