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

// TODO: adjust PWM to work with speed

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

int lleft = 20;
int middle = 23;
int rright = 29;

#define lleftSensor 	~(LPC_GPIO1->FIOPIN&(1<<lleft))
#define middleSensor 	~(LPC_GPIO1->FIOPIN&(1<<middle))
#define rrightSensor 	~(LPC_GPIO1->FIOPIN&(1<<rright))

void init(){
	LPC_SC->PCLKSEL0 &= ~(3<<12);
	LPC_SC->PCLKSEL0 |= (1<<13);					// set CLK/1

	LPC_GPIO1->FIODIR &= ~(1 << lleft);				  // lleft sensor input
	LPC_GPIO1->FIODIR &= ~(1 << middle);				  // middle sensor input
	LPC_GPIO1->FIODIR &= ~(1 << rright);				  // rright sensor input
}

int run(void)
{
	init();
    PWM leftmotor(PWM::pwm1, 50);		// pwm1 = P2.0 = left
    PWM rightmotor(PWM::pwm2, 50);		// pwm2 = P2.1 = right
    int go = 50;		/// PWM cannot go above 100
    int stop = 0;

    while(1){
		leftmotor.set(go);
		rightmotor.set(go);

    	if (lleftSensor && rrightSensor){
        	leftmotor.set(go);
        	rightmotor.set(go);
    		printf("go straight");
    	}
//		if (lleftSensor && middleSensor && rrightSensor){
//	    	leftmotor.set(stop);
//	    	rightmotor.set(stop);
//			printf("motor stop\n");
//		}
    	if (lleftSensor &&  !rrightSensor){
        	leftmotor.set(stop);
        	rightmotor.set(go);
			printf("go right\n");
		}
//		if (!(LPC_GPIO1->FIOPIN & (1 << right))){
//	    	leftmotor.set(go);
//	    	rightmotor.set(stop);
//			printf("go left\n");
//		}
    }
    return -1;
}


#endif /* LINEFOLLOWER_HPP_ */
