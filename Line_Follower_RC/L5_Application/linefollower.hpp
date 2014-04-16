/*
 * linefollower.hpp
 *
 *  Created on: Mar 18, 2014
 *      Author: Elizabeth
 */

#ifndef LINEFOLLOWER_HPP_
#define LINEFOLLOWER_HPP_

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
