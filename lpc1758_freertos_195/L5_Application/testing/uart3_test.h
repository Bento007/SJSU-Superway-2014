/*
 * Uart3_test.h
 *
 *  Created on: Apr 3, 2014
 *      Author: Bento007
 *      This file allows you to test all of the snap
 *      fuctions and uart funcitons used in the
 *      Superway pod.
 */

#ifndef Uart3_TEST_H_
#define Uart3_TEST_H_

#include <stdio.h>      // printf() / scanf()
#include "utilities.h"  // delay_ms()
#include "io.hpp"       // switches and LEDS
#include "uart3.hpp"
#include "SNAP.h"
/* X ->time
 * X <- mastertime
 * U ->update
 * U <-loc, weight
 * D -> 'D'
 * D <- D dest
 * M -> ETM
 * M <- slow,ETM
 */

/*====================================
 * PROTOTYPES
 * ===================================
 */
int Uart3_print_test(void);
int Uart3_scan_test(void);
bool snap_test(void);
int selection();
void lights();

/*
 * Function for testing snap communication
 */
void setuptime_test();
void getDest_test();
void send_Update_Test();
void send_Estimated_Time_to_Merge_Test();
void send_Merge_Test();
void send_Help_Test();
void send_Time_Test();
void get_Update_Test();
void get_CMD_Test();
void get_Help_Test();
void get_Merge_Test();

/*==========================================
 * FUNCTION DEFINITIONS
 *==========================================
 */
int test_select()
{
    while(1)
    {
        //print menu whenever returning from tests
        printf("b1: ^ b2: V b3: select\n");
        printf("0: UART Print Tests\n");
        printf("1: UART Read Tests\n");
        printf("2: SNAP Output Tests\n");
        printf("3: SNAP Input Tests\n\n");

        switch (selection())
        {
            case 0: Uart3_print_test(); break;
            case 1: Uart3_scan_test(); break;
            case 2: snap_test(); break;
            case 3: break;
            default:break;
        }
    }
}
int Uart3_print_test(void)
{
    Uart3& SNAP = Uart3::getInstance();
    SNAP.init(19200,32,32);
    int a=1;
    char x='A';

    LE.setAll(0xFF);
    printf("Uart3 Write Test:\n");
    printf("b1: UART put Tests\n");
    printf("b2: UART putline Tests\n");
    printf("b3: UART printf Tests\n");
    printf("b4: Back\n\n");
    delay_ms(1000);

    // Repeat the code inside while(1) forever
    while(1)
    {
        if(SW.getSwitch(1))
        {
            SNAP.put("PTest Put\n",1000);
            printf("Output to SNAP: test put\n");
            LE.on(1);
        }
        else if(SW.getSwitch(2))
        {
            /*TODO FIX: in snape give error: SNAPpy Script ErrorL Invalid
             * subscript in function "stdinEvent" line 52
             * line 52: cmd = buf[0]
            */
            SNAP.putline("PTest putline",1000);
            printf("Output to SNAP: Test putline\n");
            LE.on(2);
        }
        else if(SW.getSwitch(3))
        {
            SNAP.printf("PTest printf: %i %c\n", a,x);
            printf("Output to SNAP: Test printf: %i %c\n", a,x);
            LE.on(3);
        }
        else if(SW.getSwitch(4))
        {
            LE.on(4);
            return 0;
        }
        else
        {
            LE.setAll(0);
        }
        delay_ms(100);
    }
}
int Uart3_scan_test(void)
{
    Uart3& SNAP = Uart3::getInstance();
    SNAP.init(19200,32,32);
    int x,y;
    char a;
    char data[32];

    LE.setAll(0xFF);
    printf("Uart3 Read Test:\n");
    printf("b1: UART getChar Tests\n");
    printf("b2: UART gets Tests\n");
    printf("b3: UART scanf Tests\n");
    printf("b4: Back\n");
    delay_ms(1000);

    // Repeat the code inside while(1) forever
    while(1)
    {
        SNAP.flush();
        if(SW.getSwitch(1))
        {
            LE.on(1);
            SNAP.printf("T\n");
            SNAP.getChar(&a,1000);
            printf("test getChar: %c\n", a);
        }
        else if(SW.getSwitch(2))
        {
            LE.on(2);
            SNAP.printf("T\n");
            SNAP.gets(data,32,1000);
            printf("Test gets:%s\n",data);
            SNAP.printf("T\n");
            SNAP.scanf("%s",data);
            printf("Test scanf:%s\n",data);

        }
        else if(SW.getSwitch(3))
        {
            LE.on(3);
            SNAP.printf("T\n");
            SNAP.scanf("%s %c %i %i",data,&a,&x,&y);
            printf("Test scanf: %i %i %c %s\n",x,y,a,data);

        }
        else if(SW.getSwitch(4))
        {
            LE.on(4);
            return 0;
        }
        else
        {
            LE.setAll(0);
        }
        delay_ms(100);
    }
}
bool snap_test()
{
    LE.setAll(0xFF);
    delay_ms(1000);
    SNAP::getInstance().init(); // initialize snap


    printf("SNAP time Test:\n");
    printf("b1: ^ b2: V b3: select\n");
    printf("0: back\n");
    printf("1: Set RTC from master and print time\n");
    printf("2: getDest_test();\n");
    printf("3: send_Update_Test()\n");
    printf("4: send_Estimated_Time_to_Merge_Test\n");
    printf("5: send_Merge_Test()()\n");
    printf("6: send_Help_Test()\n");
    printf("7: send_Update_Test()\n");
    printf("8: send_Time_Test()\n");
    printf("...: Back\n\n");

    while(1)
    {
        switch(selection())
        {
            case 0: return 0;          break;
            case 1: setuptime_test();  break;
            case 2: getDest_test();    break;
            case 3: send_Update_Test();break;
            case 4: send_Estimated_Time_to_Merge_Test(); break;
            case 5: send_Merge_Test(); break;
            case 6: send_Help_Test();  break;
            case 7: send_Time_Test();  break;
            default: return 0; break;
        }
    }
}
int selection()
{
    int select=0;
    bool back = 0;
    LD.setNumber(select);//set 7 seg to test
    LE.setAll(0x0);//turn off LEDS

    while(1)
    {
        if(     SW.getSwitch(1)){++select;  LE.on(1);}
        else if(SW.getSwitch(2)){--select;  LE.on(2);}
        else if(SW.getSwitch(3)){back = 1;  LE.on(3);}
        else if(SW.getSwitch(4)){LE.on(4);    }
        else                    {LE.setAll(0);}

        LD.setNumber(select);

        if(back)
        {
            lights();
            return select;
        }
        delay_ms(100);
    }
}
void setuptime_test()
{
    SNAP& com = SNAP::getInstance();
    com.setup_Time();
    printf("RTC time: %s\n", rtc_get_date_time_str());
}
void getDest_test()
{
    SNAP& com = SNAP::getInstance();
    uint32_t temp;
    com.get_newDest(&temp);
    printf("cdest: %i",temp);
}
void send_Update_Test()
{
    SNAP& com = SNAP::getInstance();
    uint32_t loc = 12,speed = 99;
    uint8_t status = 2;
    com.send_Update();
    printf("send_Update_Test\n");
    printf("loc: %i\tStat: %i\tSpeed: %i\n",loc,status,speed);
}
void send_Estimated_Time_to_Merge_Test()
{
    SNAP& com = SNAP::getInstance();
    uint32_t speed = 12,ticks = 99;
    com.send_Estimated_Time_to_Merge(speed, ticks);
    printf("send_Estimated_Time_to_Merge_Test\n");
    printf("speed: %i\tticks: %i\n",speed, ticks);
}
void send_Merge_Test()
{
    uint32_t location =32;
    uint8_t status = 2;
    uint32_t speed = 100;
    int ticks =99;
    SNAP& com = SNAP::getInstance();
    com.send_Merge(location,status,speed,ticks);
    printf("send merge test\n");
    printf("loc:%i\tstat:%i\tspeed:%i\ttick:%i\n",location,status,speed,ticks);
}
void send_Help_Test()
{
    SNAP& com = SNAP::getInstance();
    uint8_t status= 255;
    uint32_t location = 36;
    com.send_Help(status, location);
    printf("send_help test\n");
    printf("stat:%i\tloc:%i\n",status,location);
}
void send_Time_Test()
{
    SNAP& com = SNAP::getInstance();
    com.send_Time();
    printf("send_Time_Test\n");
    printf("RTC time: %s\n", rtc_get_date_time_str());
}
void lights()
{
    LE.setAll(0X0);
    delay_ms(100);
    LE.setAll(0X1);
    delay_ms(100);
    LE.setAll(0X3);
    delay_ms(100);
    LE.setAll(0X5);
    delay_ms(100);
    LE.setAll(0X7);
}
#endif /* Uart3_TEST_H_ */
