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
//  #include <StackArray.h> find replacement

#include "queue.h"

// PIN MAP //
#define leftmotor 0
#define rightmotor 1
#define aMUX 6
#define bMUX 7
#define lleftsensor 29    //  very left sensor
#define leftsensor 28
#define middlesensor 23
#define rightsensor 22
#define rrightsensor 20    //  very right sensor


// VARIABLES //
int lleft=0;
int left=0;
int middle=0;
int right=0;
int rright=0;
int locationCounter = 0;
int pwmgo = 255;
int pwmslow = 200;
int pwmstop = 0;
int lightBlue=3;
int lightRed=4;
int lightYellow=5;
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
	LPC_GPIO1->FIODIR &= ~(1 << lleft);				  // lleft sensor input
	LPC_GPIO1->FIODIR &= ~(1 << middle);				  // middle sensor input
	LPC_GPIO1->FIODIR &= ~(1 << rright);				  // rright sensor input

	// OUTPUTS //
	LPC_GPIO1->FIODIR |= (1 << leftmotor);
	LPC_GPIO1->FIODIR |= (1 << rightmotor);
	LPC_GPIO1->FIODIR |= (1 << aMUX);
	LPC_GPIO1->FIODIR |= (1 << bMUX);
	LPC_GPIO1->FIODIR |= (1 << lightBlue);
	LPC_GPIO1->FIODIR |= (1 << lightYellow);
	LPC_GPIO1->FIODIR |= (1 << lightRed);

	LPC_GPIO1->FIODIR |= (1 << lleftsensor);
	LPC_GPIO1->FIODIR |= (1 << leftsensor);
	LPC_GPIO1->FIODIR |= (1 << middlesensor);
	LPC_GPIO1->FIODIR |= (1 << rightsensor);
	LPC_GPIO1->FIODIR |= (1 << rrightsensor);

  // 2 = turn right
  // 1 = straight
  // 0 = station

	// stacks
	xQueueSend(instructions, 0, portMAX_DELAY);
}

void loop() {    // all sensors are active low
  // LOW = white
  // HIGH = black
  int right=2;
  xQueueSend(instructions, &right, portMAX_DELAY);
  RCmode();
  xQueueReceive(instructions, &pop, portMAX_DELAY);

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

void turnRight(){

	bool exit=true;
	LPC_GPIO0->FIOSET = (1 << lightRed);		// turn on Blue light
	while(exit){
		LPC_GPIO0->FIOSET = (1 << aMUX);	// 0b11 = go straight
		LPC_GPIO0->FIOSET = (1 << bMUX);
		//   lleft = digitalRead(lleftsensor);
		if(!(LPC_GPIO0->FIOPIN & (1 << lleftsensor))){
			exit=false;
		}
	}
	LPC_GPIO0->FIOCLR = (1 << lightRed);		// turn on Blue light
}

void straight(){                // not 100% sure this is correct
	LPC_GPIO0->FIOSET = (1 << lightYellow);		// turn on Blue light
	LPC_GPIO0->FIOCLR = (1 << aMUX);		// 0b10 = turn right
	LPC_GPIO0->FIOSET = (1 << bMUX);
	bool exit=true;
	while(exit){
		lleft = LPC_GPIO0->FIOPIN & (1 << lleftsensor);
		left = LPC_GPIO0->FIOPIN & (1 << leftsensor);
		//    middle = digitalRead(middlesensor);
		right =  LPC_GPIO0->FIOPIN & (1 << rightsensor);
		rright = LPC_GPIO0->FIOPIN & (1 << rrightsensor);

		if(((left==0)&&(right==0))||((lleft==0)&&(left==0)&&(rright==0))||((lleft==0)&&(right==0)&&(rright==0))){
			exit=false;
		}
	}
	LPC_GPIO0->FIOCLR = (1 << lightYellow);
}

void station(){
	LPC_GPIO0->FIOCLR = (1 << aMUX);
	LPC_GPIO0->FIOCLR = (1 << bMUX);
	delay_ms(5000);
	straight();
}

void RCmode(){
	LPC_GPIO0->FIOSET = (1 << lightBlue);		// turn on Blue light
	LPC_GPIO0->FIOCLR = (1 << aMUX);
	LPC_GPIO0->FIOCLR = (1 << bMUX);
	bool exit=true;
	while(exit){
		lleft = LPC_GPIO0->FIOPIN & (1 << lleftsensor);
		left = LPC_GPIO0->FIOPIN & (1 << leftsensor);
		middle = LPC_GPIO0->FIOPIN & (1 << middlesensor);
		right =  LPC_GPIO0->FIOPIN & (1 << rightsensor);
		rright = LPC_GPIO0->FIOPIN & (1 << rrightsensor);
		if((lleft==1)&&(rright==1)){
			exit=false;
		}
	}
	LPC_GPIO0->FIOCLR = (1 << lightBlue);		// turn off Blue light
}


#endif /* SMARTLINEFOLLOWER_HPP_ */
