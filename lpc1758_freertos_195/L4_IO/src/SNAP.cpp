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
SNAP::SNAP()
{
   //add semaphore
   Uart3& wireless = Uart3::getInstance();
   wireless.init(19200,32,32);
   rtc_init (); //initialize RTC
   time = rtc_gettime ();
}
//int SNAP::newDest()
//{
//    return 0;
//}
bool SNAP::sendUpdate(int location,int status,int speed)
{
    //add semaphore
    Uart3& wireless = Uart3::getInstance();
    if(wireless.printf("U%d:%d:%d,%d,%d,%d\n",time.hour, time.min, time.sec, location, status,speed))
        return true;
    else
        return false;
}

bool SNAP::sendETM(int speed, int ticks)
{
    Uart3& wireless = Uart3::getInstance();
    int ETM = ticks/speed;
    if(wireless.printf("T%d",ETM))
        return true;
    else
        return false;
}
bool SNAP::sendMerge(int speed, int ticks)
{
    return sendETM(speed, ticks);
}
bool SNAP::sendHelp(int status)
{
    Uart3& wireless = Uart3::getInstance();
    if(wireless.printf("E%d",status))
        return true;
    else
        return false;;
}
char SNAP::getCMD(char*)
{
    //add semaphore
    char cmd;
    Uart3& wireless = Uart3::getInstance();
    wireless.getChar(&cmd,100);
    return cmd;
}

bool SNAP::test()
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
//void SNAP::setTime()//TODO
//{
//    rtc_t update;
//    sendCMD("X");
//
//    rtc_settime (&update);
//}
