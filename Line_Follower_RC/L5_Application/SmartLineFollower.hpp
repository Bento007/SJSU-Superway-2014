/*
 * SmartLineFollower.hpp//
 *
 *  Created on: Apr 14, 2014
 *      Author: Elizabeth
 */

#ifndef SMARTLINEFOLLOWER_HPP_
#define SMARTLINEFOLLOWER_HPP_

#include "tasks.hpp"
#include "examples/examples.hpp"
#include "utilities.h"
#include <stdint.h>
#include <stdio.h>
#include <sys_config.h>
#include "lpc_sys.h"
#include "io.hpp"
#include "queue.h"

// PIN MAP //
#define leftmotor 6			// PORT 2
#define rightmotor 7
#define aMUX 29				// PORT 0
#define bMUX 30
#define lleftsensor 29      // PORT 1
#define leftsensor 28
#define middlesensor 23
#define rightsensor 22
#define rrightsensor 20

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

void turnRight();
void loop();
void station();
void straight();
void RCmode();

xQueueHandle instructions;

void setup() {

	LPC_SC->PCLKSEL0 &= ~(3<<12);
	LPC_SC->PCLKSEL0 |= (1<<13);					// set CLK/1

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

void loop() {    // all sensors are active low
  // LOW = white    HIGH = black
  int command=1;
  int pop;
  while(1){
	  xQueueSend(instructions, &command, 500);
	  RCmode();
	  xQueueReceive(instructions, &pop, 500);

	  if(pop==2){
		turnRight();
	  }
	  if(pop==1){
		straight();
	  }
	  if(pop==0){
		station();
	  }
  }
}

void turnRight(){
	bool exit=true;
	LD.setNumber(2);
	while(exit){
		LPC_GPIO0->FIOSET = (1 << aMUX);	// 0b11 = go straight
		LPC_GPIO0->FIOSET = (1 << bMUX);
		if(!getLeft()){
			exit=false;
		}
	}
}

void straight(){                // not 100% sure this is correct
	LD.setNumber(1);
	LPC_GPIO0->FIOCLR = (1 << aMUX);		// 0b10 = turn right
	LPC_GPIO0->FIOSET = (1 << bMUX);
	bool exit=true;
	while(exit)
	{
		if((!getLeft()&&!getRight())||(!getLLeft()&&!getLeft()&&!getRight())||(!getLLeft()&&!getRight()&&!getRRight()))
		{
			exit=false;
		}
	}
}

void station(){
	LD.setNumber(0);
	LPC_GPIO0->FIOCLR = (1 << aMUX);		// 0b00 = RC mode
	LPC_GPIO0->FIOCLR = (1 << bMUX);
	delay_ms(5000);
	straight();
}

void RCmode(){
	LD.setNumber(3);
	LPC_GPIO0->FIOCLR = (1 << aMUX);			// 0b00 = RC mode
	LPC_GPIO0->FIOCLR = (1 << bMUX);
	bool exit=true;
	while(exit){
		printf("lleft=%i  left=%i  middle=%i  right=%i  rright=%i\n",getLLeft(),getLeft(),getMiddle(),getRight(),getRRight());
		if(!getLLeft()&&!getRRight()){
			exit=false;
		}
		delay_ms(100);
	}
}


#endif /* SMARTLINEFOLLOWER_HPP_ */
