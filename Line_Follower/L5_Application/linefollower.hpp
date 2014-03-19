/*
 * linefollower.hpp
 *
 *  Created on: Mar 18, 2014
 *      Author: Elizabeth
 */

#ifndef LINEFOLLOWER_HPP_
#define LINEFOLLOWER_HPP_

/*
 *     SocialLedge.com - Copyright (C) 2013
 *
 *     This file is part of free software framework for embedded processors.
 *     You can use it and/or distribute it as long as this copyright header
 *     remains unmodified.  The code is free for personal use and requires
 *     permission to use in a commercial product.
 *
 *      THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 *      OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 *      MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 *      I SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 *      CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *     You can reach the author of this software at :
 *          p r e e t . w i k i @ g m a i l . c o m
 */

/**
 * @file
 * @brief This is the application entry point.
 * 			FreeRTOS and stdio printf is pre-configured to use uart0_min.h before main() enters.
 * 			@see L0_LowLevel/lpc_sys.h if you wish to override printf/scanf functions.
 *
 * @note  printf of %f may be turned off to save memory, this can be configured at sys_config.h
 */
#include "tasks.hpp"
#include "examples/examples.hpp"
#include "utilities.h"
#include <stdint.h>
#include <stdio.h>
#include <sys_config.h>
#include "../L4_IO/fat/disk/spi_flash.h"
#include "../L2_Drivers/base/i2c_base.hpp"
#include "../L2_Drivers/lpc_pwm.hpp"
#include "io.hpp"
#include "lpc_sys.h"
#include "soft_timer.hpp"
#include "../L2_Drivers/eint.h"
#include <printf_lib.h>
#include "LED_Display.hpp"

int timer1;
int timer2;
int delta;
float notchDistance = 12;	// 12mm
int speed;
void timers();
void init(){
	//	LPC_PINCON->PINSEL4 &= ~(0xf);
	//    LPC_PINCON->PINSEL4 |= (5);						// enable PWM1.1 and PWM1.2
	///// delete linenneee
		int speed=6;
	    LPC_SC->PCLKSEL0 &= ~(3<<12);
	    LPC_SC->PCLKSEL0 |= (1<<13);					// set CLK/1
	    // pin 2.6
	    LPC_GPIO2->FIODIR &= ~(1<<speed);					// speed input

	    // sensor pins as input
	    int lleft = 20;
	    int left = 22;
	    int middle = 23;
	    int right = 28;
	    int rright = 29;
	    LPC_GPIO1->FIODIR &= ~(1 << lleft);				  // lleft sensor input
	    LPC_GPIO1->FIODIR &= ~(1 << left);				  // left sensor input
	    LPC_GPIO1->FIODIR &= ~(1 << middle);				  // middle sensor input
	    LPC_GPIO1->FIODIR &= ~(1 << right);				  // right sensor input
	    LPC_GPIO1->FIODIR &= ~(1 << rright);				  // rright sensor input
	    LPC_GPIO0->FIODIR |= (1<<0);						// output to LCD

		if(SW.getSwitch(0)){
			LPC_GPIO0->FIOSET = (1<<0);
		}
}


void left()
{
	timer2 = sys_get_high_res_timer_us();
	delta = timer2 - timer1;
//	printf("delta = %i  ; left speed = %f\n", delta, (notchDistance/delta));
	printf("\n%f", (notchDistance/delta));

	timer1 = sys_get_high_res_timer_us();

}
void right()
{
	timer2 = sys_get_high_res_timer_us();
	delta = timer2 - timer1;
	//printf("delta = %i  ; right speed = %f\n", delta, (notchDistance/delta));
	printf("\n%f", (notchDistance/delta));
	timer1 = sys_get_high_res_timer_us();
}


int run(void)
{
	init();
    PWM leftmotor(PWM::pwm1, 50);		// pwm1 = P2.0 = left
    PWM rightmotor(PWM::pwm2, 50);		// pwm2 = P2.1 = right
    int go = 50;		/// PWM cannot go above 100
    int stop = 0;

	////////// * interrupt stuff   *//
	void (*leftptr)();
	void (*rightptr)();
	leftptr = left;
	rightptr = right;
	int leftspeed = 6;
	int rightspeed = 7;
	////////////* end interrupt stuff *//

	eint3_enable_port2( leftspeed, eint_rising_edge , *leftptr);
	eint3_enable_port2( rightspeed, eint_rising_edge , *rightptr);

    while(1){
		leftmotor.set(go);
		rightmotor.set(go);

//    	if(!(LPC_GPIO2->FIOPIN & (1 << speedpin)))


//    	if ((LPC_GPIO1->FIOPIN & (1 << left)) && (LPC_GPIO1->FIOPIN & (1 << right))){
//        	leftmotor.set(go);
//        	rightmotor.set(go);
//    		printf("go straight");
//    	}
//		else if (!(LPC_GPIO1->FIOPIN & (1 << left)) && !(LPC_GPIO1->FIOPIN & (1 << right))){
//	    	leftmotor.set(stop);
//	    	rightmotor.set(stop);
//			printf("motor stop\n");
//		}
//    	else if(!(LPC_GPIO1->FIOPIN & (1 << left))){		// if left sensor hits line
//        	leftmotor.set(stop);
//        	rightmotor.set(go);
//			printf("go right\n");
//		}
//		else if (!(LPC_GPIO1->FIOPIN & (1 << right))){
//	    	leftmotor.set(go);
//	    	rightmotor.set(stop);
//			printf("go left\n");
//		}
    }
    return -1;
}


void timers()
{
    /* You can compute time delta in microseconds too */
    uint64_t timer_micro_sec = sys_get_high_res_timer_us();
    delay_ms(2000);
    //uint64_t delta = sys_get_high_res_timer_us() - timer_micro_sec;

    printf("\n%i", sys_get_high_res_timer_us());


}




#endif /* LINEFOLLOWER_HPP_ */
