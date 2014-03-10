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

#define turnOnRightMotor()	 	LPC_GPIO2->FIOSET = (1 << 1);
#define goLeft()				LPC_GPIO2->FIOCLR = (1 << 1);
#define turnOnLeftMotor()		LPC_GPIO2->FIOSET = (1 << 0);
#define goRight()				LPC_GPIO2->FIOCLR = (1 << 0);
#define stop()					LPC_GPIO2->FIOCLR = (1 << 0); \
								LPC_GPIO2->FIOCLR = (1 << 1);
#define goStraight()			LPC_GPIO2->FIOSET = (1 << 0); \
								LPC_GPIO2->FIOSET = (1 << 1);

//void sensorMotorTask(void *pointer){
//	while(1){
//
//	}
//}

int main(void)
{
//    xTaskCreate(sensorMotorTask, (signed char*)"sensorMotorTask", 1024, 0, 1, 0);

	LPC_PINCON->PINSEL4 &= ~(0xf);
    LPC_PINCON->PINSEL4 |= (5);						// enable PWM1.1 and PWM1.2
    LPC_SC->PCLKSEL0 &= ~(3<<12);
    LPC_SC->PCLKSEL0 |= (1<<13);					// set CLK/1

    // sensor pins as input
    int lleft = 20;
    int left = 22;
    int middle = 23;
    int right = 28;
    int rright = 29;
    LPC_GPIO1->FIODIR &= (1 << lleft);				  // lleft sensor input
    LPC_GPIO1->FIODIR &= (1 << left);				  // left sensor input
    LPC_GPIO1->FIODIR &= (1 << middle);				  // middle sensor input
    LPC_GPIO1->FIODIR &= (1 << right);				  // right sensor input
    LPC_GPIO1->FIODIR &= (1 << rright);				  // rright sensor input


    PWM leftmotor(PWM::pwm1, 50);		// pwm1 = P2.0 = left
    PWM rightmotor(PWM::pwm2, 50);		// pwm2 = P2.1 = right

    int go = 100;		/// PWM cannot go above 100
    int stop = 0;
    while(1){

    	if (!(LPC_GPIO1->FIOPIN & (1 << left)) && !(LPC_GPIO1->FIOPIN & (1 << right))){
        	leftmotor.set(go);
        	rightmotor.set(go);
    		printf("go straight");
    	}
		else if ((LPC_GPIO1->FIOPIN & (1 << left)) && (LPC_GPIO1->FIOPIN & (1 << right))){
	    	leftmotor.set(stop);
	    	rightmotor.set(stop);
			printf("motor stop\n");
		}
    	else if((LPC_GPIO1->FIOPIN & (1 << left))){		// if left sensor hits line
        	leftmotor.set(stop);
        	rightmotor.set(go);
			printf("go right\n");
		}
		else if ((LPC_GPIO1->FIOPIN & (1 << right))){
	    	leftmotor.set(go);
	    	rightmotor.set(stop);
			printf("go left\n");
		}
		else
			printf("no go...\n");

		delay_ms(10);
    }

    /**
     * A few basic tasks for this bare-bone system :
     *      1.  Terminal task provides gateway to interact with the board through UART terminal.
     *      2.  Remote task allows you to use remote control to interact with the board.
     *      3.  Wireless task responsible to receive, retry, and handle mesh network.
     *
     * Disable remote task if you are not using it.  Also, it needs ENABLE_TELEMETRY
     * such that it can save remote control codes to non-volatile memory.  IR remote
     * control codes can be learned by typing "learn" command.
     */
    scheduler_add_task(new terminalTask(PRIORITY_HIGH));
    scheduler_add_task(new remoteTask  (PRIORITY_LOW));
//    scheduler_add_task(new sensorMotorTask(PRIORITY_LOW));

    /* Consumes very little CPU, but need highest priority to handle mesh network ACKs */
    scheduler_add_task(new wirelessTask(PRIORITY_CRITICAL));

    /* Your tasks should probably used PRIORITY_MEDIUM or PRIORITY_LOW because you
     * want the terminal task to always be responsive so you can poke around in
     * case something goes wrong.
     */

    /**
     * This is a the board demonstration task that can be used to test the board.
     * This also shows you how to send a wireless packets to other boards.
     */
    #if 0
        scheduler_add_task(new example_io_demo());
    #endif

    /**
     * Change "#if 0" to "#if 1" to enable examples.
     * Try these examples one at a time.
     */
    #if 0
        scheduler_add_task(new example_task());
        scheduler_add_task(new example_alarm());
        scheduler_add_task(new example_logger_qset());
        scheduler_add_task(new example_nv_vars());
    #endif

    /**
	 * Try the rx / tx tasks together to see how they queue data to each other.
	 */
    #if 0
        scheduler_add_task(new queue_tx());
        scheduler_add_task(new queue_rx());
    #endif

    /**
     * Another example of shared handles and producer/consumer using a queue.
     * In this example, producer will produce as fast as the consumer can consume.
     */
    #if 0
    scheduler_add_task(new producer());
    scheduler_add_task(new consumer());
    #endif

    /**
     * If you have RN-XV on your board, you can connect to Wifi using this task.
     * This does two things for us:
     *   1.  The task allows us to perform HTTP web requests (@see wifiTask)
     *   2.  Terminal task can accept commands from TCP/IP through Wifly module.
     *
     * To add terminal command channel, add this at terminal.cpp :: taskEntry() function:
     * @code
     *     // Assuming Wifly is on Uart3
     *     addCommandChannel(Uart3::getInstance(), false);
     * @endcode
     */
    #if 0
        Uart3::getInstance().init(WIFI_BAUD_RATE, WIFI_RXQ_SIZE, WIFI_TXQ_SIZE);
        scheduler_add_task(new wifiTask(Uart3::getInstance(), PRIORITY_LOW));
    #endif

    scheduler_start(); /** This shouldn't return */
    return -1;
}
