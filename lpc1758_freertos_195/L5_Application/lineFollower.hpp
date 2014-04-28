/*
 * SmartLineFollower.hpp//
 *
 *  Created on: Apr 14, 2014
 *      Author: Elizabeth
 */

#ifndef SMARTLINEFOLLOWER_HPP_
#define SMARTLINEFOLLOWER_HPP_

#include "utilities.h"
#include <stdint.h>
#include <stdio.h>
#include <sys_config.h>
#include "lpc_sys.h"
#include "io.hpp"
#include "queue.h"
#include <printf_lib.h>
#include <semphr.h>
#include "eint.h"

// PIN MAP //
#define LEFTMOTOR 0         // PORT 2
#define RIGHTMOTOR 1
#define AMUX 29             // PORT 0
#define BMUX 30
#define LLEFTSENSOR 3      // PORT 2
#define LEFTSENSOR 4
#define MIDDLESENSOR 5
#define RIGHTSENSOR 6
#define RRIGHTSENSOR 7
#define QDELAY 500

void RCmode();
void turnRight();
void loop();
void station();
void straight();
void SWmode();
void tickFunction();

bool getRRight(void) {
    return !!(LPC_GPIO2->FIOPIN & (1 << RRIGHTSENSOR));
}
bool getRight(void) {
    return !!(LPC_GPIO2->FIOPIN & (1 << RIGHTSENSOR));
}
bool getMiddle(void) {
    return !!(LPC_GPIO2->FIOPIN & (1 << MIDDLESENSOR));
}
bool getLeft(void) {
    return !!(LPC_GPIO2->FIOPIN & (1 << LEFTSENSOR));
}
bool getLLeft(void) {
    return !!(LPC_GPIO2->FIOPIN & (1 << LLEFTSENSOR));
}

//xQueueHandle instructions;
//xSemaphoreHandle ticks_sem;

void setup() {

    LPC_SC->PCLKSEL0 &= ~(3 << 12);
    LPC_SC->PCLKSEL0 |= (1 << 13);                  // set CLK/1

    // INPUTS //
    LPC_GPIO2->FIODIR &= ~(1 << LLEFTSENSOR);
    LPC_GPIO2->FIODIR &= ~(1 << LEFTSENSOR);
    LPC_GPIO2->FIODIR &= ~(1 << MIDDLESENSOR);
    LPC_GPIO2->FIODIR &= ~(1 << RIGHTSENSOR);
    LPC_GPIO2->FIODIR &= ~(1 << RRIGHTSENSOR);

    // OUTPUTS //
    LPC_GPIO2->FIODIR |= (1 << LEFTMOTOR);
    LPC_GPIO2->FIODIR |= (1 << RIGHTMOTOR);
    LPC_GPIO0->FIODIR |= (1 << AMUX);
    LPC_GPIO0->FIODIR |= (1 << BMUX);

    eint3_enable_port2(RRIGHTSENSOR, eint_rising_edge, tickFunction);
    // 2 = turn right
    // 1 = straight
    // 0 = station
}

void loop() {    // all sensors are active low
    // LOW = white    HIGH = black
//  int goright=2;
//  int gostraight=1;
//  int gostation=0;
//  int tickgiven = 10;
    int pop;

    int skip = 0;
    while (1) {

        RCmode();
        if (xQueueReceive(directionQ, &pop, QDELAY)) {
//        printf("nothing in queue!\n");

            if (pop == 2) {
                turnRight();
                #if DEBUG
                    puts("LF right");
                #endif
            }

            if (pop == 1) {
                if (skip == 1) {
                    turnRight();
                    #if DEBUG
                        puts("LF turn right");
                    #endif
                    skip++;
                }
                else {
                    straight();
                    skip++;
                    #if DEBUG
                        puts("LF straight");
                    #endif
                }
            }

            if (pop == 0) {
                station();
                skip = 0;
                #if DEBUG
                    puts("LF station");
                #endif
                (xQueueSend(lineFollowertoSM, &skip, QDELAY));
            }

        }  //end if xQueueReceive
    }  //end while
}  //end loop()

void turnRight() {
    bool exit = true;
    LD.setLeftDigit('R');
    while (exit) {
        LPC_GPIO0->FIOSET = (1 << AMUX);    // 0b11 = go straight
        LPC_GPIO0->FIOSET = (1 << BMUX);
        if ((!getLLeft() && !getLeft()) || (!getLLeft() && !getRight())) {
            exit = false;
        }
    }
}

void straight() {
    LD.setLeftDigit('S');
    LPC_GPIO0->FIOCLR = (1 << AMUX);        // 0b10 = turn right
    LPC_GPIO0->FIOSET = (1 << BMUX);
    bool exit=true;
    while(exit)
    {
        if( (!getLeft()&&!getRight())
                || (!getLLeft() && !getLeft() && !getRRight())
                || (!getLLeft() && !getRight() && !getRRight()) )
        {
            exit=false;
        }
    }
}

void station() {
    LD.setLeftDigit('0');
    LPC_GPIO0->FIOCLR = (1 << AMUX);        // 0b00 = RC mode
    LPC_GPIO0->FIOCLR = (1 << BMUX);
//  delay_ms(5000);// debugging
}

void RCmode() {
    bool exit = true;
    #if DEBUG
        puts("LF RC MODE");
    //                        delay_ms(100);
    #endif
    while (exit) {
        LD.setLeftDigit('G');
        LPC_GPIO0->FIOCLR = (1 << AMUX);            // 0b00 = RC mode
        LPC_GPIO0->FIOCLR = (1 << BMUX);
        if(getLLeft()&&getRRight()) {
            exit=false;
        }
        if( !getLLeft() && getLeft() && !getMiddle() && getRight() && !getRRight() ) {
            SWmode();
        }
    }
}

void SWmode() {
    LD.setLeftDigit('P');
    LPC_GPIO2->FIOCLR = (1 << LEFTMOTOR);
    LPC_GPIO2->FIOSET = (1 << RIGHTMOTOR);

    // choose 0b01 = software mode
    LPC_GPIO0->FIOSET = (1 << AMUX);
    LPC_GPIO0->FIOCLR = (1 << BMUX);
//    delay_ms(100);// giving time for pod to get out of stop error
}

void tickFunction() {
    xSemaphoreGive(ticks_sem);
//    vTaskDelay(100);
}

#endif /* SMARTLINEFOLLOWER_HPP_ */
