/*
 * uart3_test.h
 *
 *  Created on: Apr 3, 2014
 *      Author: Bento007
 */

#ifndef UART3_TEST_H_
#define UART3_TEST_H_

#include <stdio.h>      // printf() / scanf()
#include "utilities.h"  // delay_ms()
#include "io.hpp"       // switches and LEDS
#include "uart3.hpp"

int uart3_print_test(void)
{
    Uart3& SNAP = Uart3::getInstance();
    SNAP.init(19200,32,32);
    int a=1;
    char x='A';

    LE.setAll(0xFF);
    delay_ms(2000);

    // Repeat the code inside while(1) forever
    while(1)
    {
        if(SW.getSwitch(1))
        {
            SNAP.put("Test Put\n",1000);
            printf("test put");
            LE.on(1);
        }
        else if(SW.getSwitch(2))
        {
            SNAP.putline("Test putline",1000);
            printf("Test putline\n");
            LE.on(2);
        }
        else if(SW.getSwitch(3))
        {
            SNAP.printf("Test printf: %i %c", a,x);
            printf("Test printf\n");
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

int uart3_scan_test(void)
{
    Uart3& SNAP = Uart3::getInstance();
    SNAP.init(19200,32,32);
    int x;
    char a;
    char data[32];

    LE.setAll(0xFF);
    delay_ms(2000);

    // Repeat the code inside while(1) forever
    while(1)
    {
        if(SW.getSwitch(1))
        {
            SNAP.getChar(&a,1000);
            printf("test getChar: %c\n", a);
            LE.on(1);
        }
        else if(SW.getSwitch(2))
        {
            SNAP.gets(data,32,1000);
            printf("Test gets:%s\n",data);
            LE.on(2);
        }
        else if(SW.getSwitch(3))
        {
            SNAP.scanf("%c %i",&a,&x);
//            SNAP.scanf("%i",&x);
            printf("Test scanf: %c %i\n",a,x);
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

#endif /* UART3_TEST_H_ */
