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
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"
#include "rtc.h"
#include "uart3.hpp"
//#include "utilities.h"
//#include <stdio.h>

//TODO if status says pod is free send it a new destination base on it's current location

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
//bool SNAP::recentlyActive( uint32_t ms)
//{
//    Uart3& wireless = Uart3::getInstance();
//    return wireless.recentlyActive(ms);
//}

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
    if(wireless.printf("U%i,%i,%i,%i,%i,%i\n",mLastActivityTime, location, status,speed,ticks,ticks/speed))
    {
        resetUpdateTime();
        return true;
    }
    else
        return false;
}

//bool SNAP::send_Estimated_Time_to_Merge(uint32_t speed, int ticks)
//{
//    Uart3& wireless = Uart3::getInstance();
//    int ETM = ticks/speed;
////    rtc_t time = rtc_gettime();
//    if(wireless.printf("M%i",ETM))
//    {
////        wireless.resetActivity();
//        return true;
//    }
//    else
//        return false;
//}
bool SNAP::send_Merge()
{
    send_Update();
    Uart3& wireless = Uart3::getInstance();
    wireless.printf("M\n");
    return true;
//    if(send_Update() && send_Estimated_Time_to_Merge(speed,ticks))
//        return true;
//    else
//        return false;
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
    char test[8], a;
    test[0]='T';
    int x,y;
    wireless.putline(test,200);
    vTaskDelay(10);//TODO: change to vtask delay or find better solution
    if(wireless.scanf("%s %c %i %i",test,&a,&x,&y))
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
void SNAP::setup_Time()
{
    rtc_t update;
    int ty =0,tm=0,td=0,th=0,tmi=0,ts=0,tdw=0,tdy=0;
    char flag;
    int counter= 0;
    Uart3& wireless = Uart3::getInstance();
    wireless.flush();
    wireless.printf("X\n",500);
    wireless.getChar(&flag,100);
    while(flag != 'X')
    {
        wireless.getChar(&flag,100);
        counter++;
        if(counter > 128)//retry
        {
            counter = 0;
            wireless.printf("X\n",500);
            wireless.getChar(&flag,100);
        }
    }
    vTaskDelay(10);//TODO: change to vtask delay or find better solution

    wireless.scanf("%i %i %i %i %i %i %i %i",&ty,&tm,&td,&th,
            &tmi,&ts,&tdw,&tdy);
//    printf("%i %i %i %i %i %i %i %i\n",ty,tm,td,th,tmi,ts,tdw,tdy);
    update.year = ty;   update.month = tm;
    update.day = td;    update.hour = th;
    update.min = tmi;   update.sec = ts;
    update.dow =tdw;    update.doy = tdy;
    rtc_settime (&update);
}

void SNAP::update_SNAP(uint32_t loc,uint8_t sta,uint32_t spe, int tic)
{   status = sta;  speed = spe;
    ticks = tic;    location = loc;
}

void SNAP::send_Time()  //sends current RTC
{
    rtc_t update = rtc_gettime();
    Uart3& wireless = Uart3::getInstance();
    wireless.printf("R %i %i %i %i %i %i %i %i\n",update.year,update.month,update.day,
            update.hour, update.min,update.sec,update.dow,update.doy);
}

bool SNAP::RXempty()
{
    Uart3& wireless = Uart3::getInstance();
    if(wireless.getRxQueueSize()>0)
        return true;
    else
        return false;
}
