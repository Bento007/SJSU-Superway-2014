/*
 * SNAP.cpp
 *
 *  Created on: Mar 28, 2014
 *      Author: Bento007
 *      This contains the functions definition for the SNAP.hpp library.
 */

#include "SNAP.h"
#include <stdint.h>
#include <time.h>

//#include "FreeRTOS.h"
//#include "queue.h"
//#include "semphr.h"
//#include "task.h"

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

//SNAP::SNAP(uint8_t priority):scheduler_task("wireless", 512, priority, NULL)//for scheduler
SNAP::SNAP(): status(0),speed(0),ticks(0),location(0), destination(0),mLastActivityTime(0)
{
   //add semaphore
//   Uart3& wireless = Uart3::getInstance();
//   wireless.init(19200,32,32);
//   rtc_init (); //initialize RTC
//   time = rtc_gettime ();
//   status = 0;
//   location = 0;
//   speed = 0;
//   ticks = 0;
//   mLastActivityTime = 0;
}

bool SNAP::init()
{
    Uart3& wireless = Uart3::getInstance();
    wireless.init(19200,32,32);
    return true;
}
bool SNAP::send_Update()
{
    //add semaphore
    Uart3& wireless = Uart3::getInstance();
//    if(wireless.printf("U%i:%i:%i,%i,%i,%i\n",time.hour, time.min, time.sec, location, status,speed))
    if(wireless.printf("U%i,%i,%i,%i\n",mLastActivityTime, location, status,speed,ticks))
    {
        resetActivity();
        return true;
    }
    else
        return false;
}

bool SNAP::send_Estimated_Time_to_Merge(uint32_t speed, int ticks)
{
    Uart3& wireless = Uart3::getInstance();
    int ETM = ticks/speed;
    if(wireless.printf("M%i",ETM))
        return true;
    else
        return false;
}
bool SNAP::send_Merge(uint32_t location,uint8_t status,uint32_t speed, int ticks)
{
    if(send_Update() &&
            send_Estimated_Time_to_Merge(speed,ticks))
        return true;
    else
        return false;
}
bool SNAP::send_Help(uint8_t status, uint32_t location )  //sends help to SNAP
{
    Uart3& wireless = Uart3::getInstance();
    if(wireless.printf("E%i,%i",status,location))
        return true;
    else
        return false;;
}
char SNAP::get_nextCMD()
{
    //add semaphore
    char cmd;
    Uart3& wireless = Uart3::getInstance();
    wireless.scanf("%c",&cmd);
    return cmd;
}

bool SNAP::send_Test()
{
    //add semaphore
    Uart3& wireless = Uart3::getInstance();
    char test[] = {"T     "};
    wireless.putline(test,200);
    if(wireless.gets(test,6,200))
        return true;
    else
        return false;
}
bool  SNAP::get_newDest(uint32_t* dest)
{
    Uart3& wireless = Uart3::getInstance();
//    uint8_t dest;
//    wireless.putline("D",200);
    if(wireless.scanf("%i",dest))
    {
        return true;
        destination = *dest;
    }

    else
        return false;
}
int SNAP::get_Help()
{
    int dest;
    Uart3& wireless = Uart3::getInstance();
    wireless.scanf("%i",&dest);
    return dest;
}
bool SNAP::get_TrackUpdate(uint32_t* dest,uint32_t* weight)
{
    Uart3& wireless = Uart3::getInstance();
    if(wireless.scanf("%i %i",*dest,*weight))
        return true;
    else
        return false;
}
int SNAP::get_Merge()    //get the new time to merge
{
    int etm;
    Uart3& wireless = Uart3::getInstance();
    wireless.scanf("%i",&etm);
    return etm;
}
/* This updates the time from the server
 * and sets the local RTC.
 * */
void SNAP::setup_Time()//TODO
{
    rtc_t update;
    Uart3& wireless = Uart3::getInstance();
    wireless.putChar('X',1000);
    wireless.scanf("%i %i %i %i %i %i %i %i", update.sec, update.min, update.hour,
            update.dow, update.day, update.month, update.year, update.doy);
    rtc_settime (&update);
}
void SNAP::update_SNAP(uint32_t loc,uint8_t sta,uint32_t spe, int tic)
{   status = sta;  speed = spe;
    ticks = tic;    location = loc;
    resetActivity();
}
