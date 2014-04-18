/*
 * SmartLineFollower.hpp
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
//  #include <StackArray.h> find replacement

#include "queue.h"

// PIN MAP //
#define leftmotor 6			// PORT 2
#define rightmotor 7
#define aMUX 29				// PORT 0
#define bMUX 30
#define lleftsensor 29    //  very left sensor  // PORT 1
#define leftsensor 28
#define middlesensor 23
#define rightsensor 22
#define rrightsensor 20    //  very right sensor
#define lleft LPC_GPIO1->FIOPIN&(1<<lleftsensor)
#define left LPC_GPIO1->FIOPIN&(1<<leftsensor)
#define right LPC_GPIO1->FIOPIN&(1<<rightsensor)
#define rright LPC_GPIO1->FIOPIN&(1<<rrightsensor)

// VARIABLES //
int middle=0;
int locationCounter = 0;
int pwmgo = 255;
int pwmslow = 200;
int pwmstop = 0;
int lightWhite=5;	// PORT 2
int lightGreen=4;
int lightYellow=3;
// StackArray <int> stack; create stack
int pop;

xQueueHandle instructions;
void turnRight();
void loop();
void station();
void straight();
void RCmode();

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
	LPC_GPIO2->FIODIR |= (1 << lightWhite);
	LPC_GPIO2->FIODIR |= (1 << lightGreen);
	LPC_GPIO2->FIODIR |= (1 << lightYellow);


  // 2 = turn right
  // 1 = straight
  // 0 = station

	// stacks
	int queue=1;
	xQueueSend(instructions, &queue, portMAX_DELAY);
}

void loop() {    // all sensors are active low
  // LOW = white
  // HIGH = black
  int command=1;
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
		//   lleft = digitalRead(lleftsensor);
		if(!(LPC_GPIO1->FIOPIN & (1 << lleftsensor))){
			exit=false;
		}
	}
}

void straight(){                // not 100% sure this is correct
	LD.setNumber(1);
	LPC_GPIO0->FIOCLR = (1 << aMUX);		// 0b10 = turn right
	LPC_GPIO0->FIOSET = (1 << bMUX);
	bool exit=true;
	while(exit){
		if(((!left)&&(!right))||((!lleft)&&(!left)&&(!right))||((!lleft)&&(!right)&&(!rright))){
			exit=false;
		}
	}
	LPC_GPIO2->FIOCLR = (1 << lightGreen);
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
	LPC_GPIO2->FIOSET = (1 << lightWhite);		// turn on Blue light
	LPC_GPIO0->FIOCLR = (1 << aMUX);			// 0b00 = RC mode
	LPC_GPIO0->FIOCLR = (1 << bMUX);
	bool exit=true;
	while(exit){
//		lleft = LPC_GPIO1->FIOPIN & (1 << lleftsensor);
////		left = LPC_GPIO1->FIOPIN & (1 << leftsensor);
////		middle = LPC_GPIO1->FIOPIN & (1 << middlesensor);
////		right =  LPC_GPIO1->FIOPIN & (1 << rightsensor);
//		rright = LPC_GPIO1->FIOPIN & (1 << rrightsensor);
		if((LPC_GPIO1->FIOPIN&(1<<lleftsensor))&&(LPC_GPIO1->FIOPIN&(1<<rrightsensor))){
			exit=false;
		}
		delay_ms(100);
	}
	LPC_GPIO2->FIOCLR = (1 << lightWhite);		// turn off Blue light
}


#endif /* SMARTLINEFOLLOWER_HPP_ */
