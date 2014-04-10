/*
 * SNAP.cpp
 *
 *  Created on: Mar 28, 2014
 *      Author: Bento007
 *      This contains the functions definition for the SNAP.hpp library.
 */

#include <SNAP.h>
#include <stdint.h>
#include "rtc.h"
#include "uart2.hpp"
#include "utilities.h"
#include <stdio.h>  //DBG
//#include <time.h>


//SNAP::SNAP(uint8_t priority):scheduler_task("wireless", 512, priority, NULL)//for scheduler
SNAP::SNAP()
{
   //add semaphore
   Uart2& wireless = Uart2::getInstance();
   wireless.init(19200,32,32);
   rtc_init (); //initialize RTC
//   setup_Time();
//   rtc_settime(get_MasterTime());   //sets the time from master time
//   time = rtc_gettime ();
//   status = 0;
//   speed = 0;
//   ticks = 0;
}

bool SNAP::send_Update(uint32_t location,uint8_t status,uint32_t speed)
{
    //add semaphore
    Uart2& wireless = Uart2::getInstance();
    rtc_t time = rtc_gettime();
    if(wireless.printf("U%i:%i:%i:%i,%i,%i,%i\n",time.dow,time.hour, time.min, time.sec, location, status,speed))
        return true;
    else
        return false;
}

bool SNAP::send_Estimated_Time_to_Merge(uint32_t speed, int ticks)
{
    Uart2& wireless = Uart2::getInstance();
    int ETM = ticks/speed;
//    rtc_t time = rtc_gettime();
    if(wireless.printf("M%i",ETM))
        return true;
    else
        return false;
}
bool SNAP::send_Merge(uint32_t location,uint8_t status,uint32_t speed, int ticks)
{
//    int ETM = ticks/speed;
//    rtc_t time = rtc_gettime();
    if(send_Update(location,status,speed) &&
            send_Estimated_Time_to_Merge(speed,ticks))
        return true;
    else
        return false;
}
bool SNAP::send_Help(uint8_t status, uint32_t location )  //sends help to SNAP
{
    Uart2& wireless = Uart2::getInstance();
    if(wireless.printf("E%i,%i",status,location))
        return true;
    else
        return false;;
}
char SNAP::get_CMD()
{
    //add semaphore
    char cmd;
    Uart2& wireless = Uart2::getInstance();
    wireless.scanf("%c",&cmd);
    return cmd;
}

bool SNAP::send_Test()
{
    //add semaphore
    Uart2& wireless = Uart2::getInstance();
    char data[8], a;
    int x,y;
    wireless.printf("T\n");
    if(wireless.scanf("%s %c %i %i",data,&a,&x,&y))
    {
//        printf("Test: %i %i %c %s\n",x,y,a,data);
        return true;
    }
    else
        return false;
}
int  SNAP::get_Dest()
{
    Uart2& wireless = Uart2::getInstance();
    uint8_t dest;
    wireless.putline("D",200);
    delay_ms(10);//TODO: change to vtask delay or find better solution
    if(wireless.scanf("%i",&dest))
        return dest;
    else
        return false;
}
int SNAP::get_Help()
{
    int dest;
    Uart2& wireless = Uart2::getInstance();
    delay_ms(10);//TODO: change to vtask delay or find better solution
    wireless.scanf("%i",&dest);
    return dest;
}
bool SNAP::get_Update(uint32_t* dest,uint32_t* weight)
{
    Uart2& wireless = Uart2::getInstance();
    delay_ms(10);//TODO: change to vtask delay or find better solution
    if(wireless.scanf("%i %i",*dest,*weight))
        return true;
    else
        return false;
}
int SNAP::get_Merge()    //get the new time to merge
{
    int etm;
    Uart2& wireless = Uart2::getInstance();
    delay_ms(10);//TODO: change to vtask delay or find better solution
    wireless.scanf("%i",&etm);
    return etm;
}
void SNAP::setup_Time()//TODO
{
    rtc_t update;
    int ty =0,tm=0,td=0,th=0,tmi=0,ts=0,tdw=0,tdy=0;
    char flag;
    Uart2& wireless = Uart2::getInstance();
    wireless.flush();
    wireless.printf("X\n",500);
    wireless.getChar(&flag,100);
    while(flag != 'X')
    {
        wireless.getChar(&flag,100);
    }
    delay_ms(10);//TODO: change to vtask delay or find better solution

    wireless.scanf("%i %i %i %i %i %i %i %i",&ty,&tm,&td,&th,
            &tmi,&ts,&tdw,&tdy);
    printf("%i %i %i %i %i %i %i %i\n",ty,tm,td,th,tmi,ts,tdw,tdy);
    update.year = ty;   update.month = tm;
    update.day = td;    update.hour = th;
    update.min = tmi;   update.sec = ts;
    update.dow =tdw;    update.doy = tdy;
    rtc_settime (&update);
}

void SNAP::send_Time()  //sends current RTC
{
    rtc_t update = rtc_gettime();
    Uart2& wireless = Uart2::getInstance();
    wireless.printf("R %i %i %i %i %i %i %i %i\n",update.year,update.month,update.day,
            update.hour, update.min,update.sec,update.dow,update.doy);
}
