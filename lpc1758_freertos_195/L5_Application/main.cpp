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
#include "StateMachine.hpp"
#include "eint.h"
#include "lpc_pwm.hpp"
#include "lineFollower.hpp"

QueueHandle_t directives=0;     //shared queue, for sending commands from
                                //the state machine to the line follower task

void lineFollower(void *p)
{
    //TODO call initialization function

    initLineFollower(); //initialize the line follower

    while(1){
        //TODO call the line follower run() function or place code here.
        //All operations code should be in here.
        //Task will never end/exit, and it shouldn't.

        leftmotor.set(go);
        rightmotor.set(go);

        //      if(!(LPC_GPIO2->FIOPIN & (1 << speedpin)))

        //      if ((LPC_GPIO1->FIOPIN & (1 << left)) && (LPC_GPIO1->FIOPIN & (1 << right))){
        //          leftmotor.set(go);
        //          rightmotor.set(go);
        //          printf("go straight");
        //      }
        //      else if (!(LPC_GPIO1->FIOPIN & (1 << left)) && !(LPC_GPIO1->FIOPIN & (1 << right))){
        //          leftmotor.set(stop);
        //          rightmotor.set(stop);
        //          printf("motor stop\n");
        //      }
        //      else if(!(LPC_GPIO1->FIOPIN & (1 << left))){        // if left sensor hits line
        //          leftmotor.set(stop);
        //          rightmotor.set(go);
        //          printf("go right\n");
        //      }
        //      else if (!(LPC_GPIO1->FIOPIN & (1 << right))){
        //          leftmotor.set(go);
        //          rightmotor.set(stop);
        //          printf("go left\n");
        //      }

    }
}

void stateMachine(void *p)
{
    //TODO do/call initializations here

    while(1){
        //TODO call the state machine function OR migrate code to here
        //All state machine operations code should be here.
        //Task should never end/exit.
        //This task must have highest priority, unless a "safety" task is made.
    }

}

int main(void)
{
    //TODO what queues will be shared? Add them here first.
    //Would need to pass directives in from the state machine such as:
    //Forward (straight), Left (turn), Yield, Stop, and (maybe) wait.
    directives = xQueueCreate(2, sizeof(int));  //Will transmit commands from State machine to line follower.

    //Create the tasks that will be running.
    xTaskCreate(lineFollower, (const char*)"Line Follower", STACK_BYTES(1024), 0, 1, 0);
    xTaskCreate(stateMachine, (const char*)"State Machine", STACK_BYTES(1024), 0, 2, 0);

    vTaskStartScheduler();  //schedule the tasks, should never return/exit.

    return -1;
}
