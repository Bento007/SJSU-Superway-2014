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
#define leftmotor 0			// PORT 2
#define rightmotor 1
#define aMUX 29				// PORT 0
#define bMUX 30
#define lleftsensor 3      // PORT 2
#define leftsensor 4
#define middlesensor 5
#define rightsensor 6
#define rrightsensor 7

void turnRight();
void loop();
void station();
void straight();
void RCmode();
void SWmode();
void setLeftMotor(bool set);
void setRightMotor(bool set);
void tickFunction();
int deleteme=5;

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

xQueueHandle instructions;

void setup() {

	LPC_SC->PCLKSEL0 &= ~(3<<12);
	LPC_SC->PCLKSEL0 |= (1<<13);					// set CLK/1

	// INPUTS //
	LPC_GPIO2->FIODIR &= ~(1 << lleftsensor);
	LPC_GPIO2->FIODIR &= ~(1 << leftsensor);
	LPC_GPIO2->FIODIR &= ~(1 << middlesensor);
	LPC_GPIO2->FIODIR &= ~(1 << rightsensor);
	LPC_GPIO2->FIODIR &= ~(1 << rrightsensor);

	// OUTPUTS //
	LPC_GPIO2->FIODIR |= (1 << leftmotor);
	LPC_GPIO2->FIODIR |= (1 << rightmotor);
	LPC_GPIO0->FIODIR |= (1 << aMUX);
	LPC_GPIO0->FIODIR |= (1 << bMUX);

	eint3_enable_port2(lleftsensor, eint_rising_edge, tickFunction);
  // 2 = turn right
  // 1 = straight
  // 0 = station
}

void loop() {    // all sensors are active low
  // LOW = white    HIGH = black
  int goright=2;
  int gostraight=1;
  int gostation=0;
  int pop;
  bool rightTurn=false;
  xQueueSend(instructions, &gostation, 500);
  xQueueSend(instructions, &gostraight, 500);
  xQueueSend(instructions, &gostraight, 500);
  xQueueSend(instructions, &gostraight, 500);
  xQueueSend(instructions, &gostraight, 500);
  xQueueSend(instructions, &gostraight, 500);
  xQueueSend(instructions, &goright, 500);
  xQueueSend(instructions, &gostation, 500);
  while(1){
	  RCmode();
	  xQueueReceive(instructions, &pop, 500);

	  if(pop==2){
		turnRight();
	  }
	  if(pop==1){
		if(rightTurn){
			turnRight();
			rightTurn=false;
		}
		else
		  straight();
	  }
	  if(pop==0){
		station();
		rightTurn=true;
	  }
  }
}

void turnRight(){
	bool exit=true;
	LD.setRightDigit('R');
	while(exit){
		LPC_GPIO0->FIOSET = (1 << aMUX);	// 0b11 = go straight
		LPC_GPIO0->FIOSET = (1 << bMUX);
		if((!getLLeft()&&!getLeft())||(!getLLeft()&&!getRight())){
			exit=false;
		}
	}
}

void straight(){                // not 100% sure this is correct
	LD.setRightDigit('S');
	LPC_GPIO0->FIOCLR = (1 << aMUX);		// 0b10 = turn right
	LPC_GPIO0->FIOSET = (1 << bMUX);
	bool exit=true;
	while(exit)
	{
		if((!getLeft()&&!getRight())
				||(!getLLeft()&&!getLeft()&&!getRRight())
				||(!getLLeft()&&!getRight()&&!getRRight()))
		{
			exit=false;
		}
	}
}

void station(){
	LD.setRightDigit('0');
	LPC_GPIO0->FIOCLR = (1 << aMUX);		// 0b00 = RC mode
	LPC_GPIO0->FIOCLR = (1 << bMUX);
}

void RCmode(){
	bool exit=true;
	bool newTick=true;
	int i=30;
	while(exit){
		LD.setRightDigit('G');
		LPC_GPIO0->FIOCLR = (1 << aMUX);			// 0b00 = RC mode
		LPC_GPIO0->FIOCLR = (1 << bMUX);
		if(getLLeft()&&getRRight()){
			exit=false;
		}
		if(getLLeft()){
			if(newTick){
				LD.setRightDigit(i);
				i++;
				newTick=false;
			}
		}
		if(!getLLeft()){
			newTick=true;
		}
		if(!getLLeft()&&getLeft()&&!getMiddle()&&getRight()&&!getRRight()){
			SWmode();
		}
	}
}

void SWmode(){
//	setLeftMotor(false);
//	setRightMotor(true);
	LD.setRightDigit('P');
	LPC_GPIO2->FIOCLR = (1 << leftmotor);
	LPC_GPIO2->FIOSET = (1 << rightmotor);

	// choose 0b01 = software mode
	LPC_GPIO0->FIOSET = (1 << aMUX);
	LPC_GPIO0->FIOCLR = (1 << bMUX);
	delay_ms(100);
}

void tickFunction(){
	/// working progress: fix this!!!!

	LD.setLeftDigit('A');
	printf(" TICK!!  ");
}
//
//void setLeftMotor(bool set)
//{
//	if(set==false){
//		LPC_GPIO2->FIOCLR = (1 << leftmotor);
//	}
//	if(set==true){
//		LPC_GPIO2->FIOSET = (1 << leftmotor);
//	}
//}
//
//void setRightMotor(bool set)
//{
//	if(set==false){
//		LPC_GPIO2->FIOCLR = (1 << rightmotor);
//	}
//	if(set==true){
//		LPC_GPIO2->FIOSET = (1 << rightmotor);
//	}
//}


#endif /* SMARTLINEFOLLOWER_HPP_ */
