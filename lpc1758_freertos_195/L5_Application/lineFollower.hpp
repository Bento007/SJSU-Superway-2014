/*
 * lineFollower.hpp
 *
 *  Created on: Mar 21, 2014
 *      Author: Eriberto
 *
 *      EDIT: Mar 25, 2014
 *      Author:Trent S.
 *      -#defined constants
 *      -Removed globals
 *      -Fix leftMotor & rightMotor in constructor
 *      -added private members int leftSpeed and rightSpeed
 *      -Replaced  delay_ms(1000) with  vTaskDelay(1000)
 *      -Changed bool run(void*p) return value to true
 *      -Added timers
 */


#ifndef LINEFOLLOWER_HPP_
#define LINEFOLLOWER_HPP_

//#include "tasks.hpp"
//#include "examples/examples.hpp"
#include "utilities.h"
#include <stdint.h>
#include <stdio.h>
#include <sys_config.h>
#include "lpc_sys.h"
#include "io.hpp"
#include "queue.h"

// PIN MAP //
#define leftmotor 0         // PORT 2
#define rightmotor 1
#define aMUX 29             // PORT 0
#define bMUX 30
#define lleftsensor 29      // PORT 1
#define leftsensor 28
#define middlesensor 23
#define rightsensor 22
#define rrightsensor 20

void turnRight();
void loop();
void station();
void straight();
void RCmode();
void SWmode();
void setLeftMotor(bool set);
void setRightMotor(bool set);

bool getRRight(void)
{
    return !!(LPC_GPIO1->FIOPIN & (1 << rrightsensor));
}
bool getRight(void)
{
    return !!(LPC_GPIO1->FIOPIN & (1 << rightsensor));
}
bool getMiddle(void)
{
    return !!(LPC_GPIO1->FIOPIN & (1 << middlesensor));
}
bool getLeft(void)
{
    return !!(LPC_GPIO1->FIOPIN & (1 << leftsensor));
}
bool getLLeft(void)
{
    return !!(LPC_GPIO1->FIOPIN & (1 << lleftsensor));
}

QueueHandle_t instructions;

void setup() {

    LPC_SC->PCLKSEL0 &= ~(3<<12);
    LPC_SC->PCLKSEL0 |= (1<<13);                    // set CLK/1

    // INPUTS //
    LPC_GPIO1->FIODIR &= ~(1 << lleftsensor);
    LPC_GPIO1->FIODIR &= ~(1 << leftsensor);
    LPC_GPIO1->FIODIR &= ~(1 << middlesensor);
    LPC_GPIO1->FIODIR &= ~(1 << rightsensor);
    LPC_GPIO1->FIODIR &= ~(1 << rrightsensor);

    // OUTPUTS //
    LPC_GPIO2->FIODIR |= (1 << leftmotor);
    LPC_GPIO2->FIODIR |= (1 << rightmotor);
    LPC_GPIO0->FIODIR |= (1 << aMUX);
    LPC_GPIO0->FIODIR |= (1 << bMUX);

  // 2 = turn right
  // 1 = straight
  // 0 = station
}

void debuggerFunc(void)
{
    delay_ms(500);
}

void loop() {    // all sensors are active low
  // LOW = white    HIGH = black
  int goright=2;
  int gostraight=1;
  int gostation=0;
  int pop;

  while(1){

//      RCmode(); //stays here until need to change.

      if(xQueueReceive(directionQ, &pop, 500)){
      printf("LF %i", pop);
      if(pop == 0){
          puts("\n");
          xQueueSend(lineFollowertoSM, &pop, 10);
      }
      debuggerFunc();
      }
//      if(pop==2){
//        turnRight();
//      }
//      if(pop==1){
//        straight();
//      }
//      if(pop==0){
//        station();
////        int i =9;
////        xQueueSend(lineFollowertoSM, &i, 10);
//      }
  }
}

void turnRight(){
    bool exit=true;
    LD.setNumber(2);
    while(exit){
        LPC_GPIO0->FIOSET = (1 << aMUX);    // 0b11 = go straight
        LPC_GPIO0->FIOSET = (1 << bMUX);
        if((!getLLeft()&&!getLeft())||(!getLLeft()&&!getRight())){
            exit=false;
        }
    }
}

void straight(){                // not 100% sure this is correct
    LD.setNumber(1);
    LPC_GPIO0->FIOCLR = (1 << aMUX);        // 0b10 = turn right
    LPC_GPIO0->FIOSET = (1 << bMUX);
    bool exit=true;
    while(exit)
    {
        printf("lleft=%i  left=%i  middle=%i  right=%i  rright=%i\n",!getLLeft(),!getLeft(),!getMiddle(),!getRight(),!getRRight());
        if((!getLeft()&&!getRight())||(!getLLeft()&&!getLeft()&&!getRRight())||(!getLLeft()&&!getRight()&&!getRRight()))
        {
            exit=false;
        }
    }
}

void station(){
    LD.setNumber(0);
    LPC_GPIO0->FIOCLR = (1 << aMUX);        // 0b00 = RC mode
    LPC_GPIO0->FIOCLR = (1 << bMUX);
    delay_ms(5000);
    straight();
}

void RCmode(){
    bool exit=true;
    while(exit){
        LD.setNumber(3);
        LPC_GPIO0->FIOCLR = (1 << aMUX);            // 0b00 = RC mode
        LPC_GPIO0->FIOCLR = (1 << bMUX);
        if(getLLeft()&&getRRight()){
            exit=false;
        }
        if(!getLLeft()&&getLeft()&&!getMiddle()&&getRight()&&!getRRight()){
            SWmode();
        }
    }
}

void SWmode(){
//  setLeftMotor(false);
//  setRightMotor(true);
    LD.setNumber(4);
    LPC_GPIO2->FIOCLR = (1 << leftmotor);
    LPC_GPIO2->FIOSET = (1 << rightmotor);

    // choose 0b01 = software mode
    LPC_GPIO0->FIOSET = (1 << aMUX);
    LPC_GPIO0->FIOCLR = (1 << bMUX);
    delay_ms(100);

}


#endif /* LINEFOLLOWER_HPP_ */
