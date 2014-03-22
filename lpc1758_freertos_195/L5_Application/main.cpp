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
<<<<<<< HEAD
#include "examples/examples.hpp"
=======
#include "StateMachine.hpp"
#include "eint.h"
#include "lpc_pwm.hpp"
#include "lineFollower.hpp"
>>>>>>> refs/heads/master

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

<<<<<<< HEAD
    /**
     * A few basic tasks for this bare-bone system :
     *      1.  Terminal task provides gateway to interact with the board through UART terminal.
     *
     * Disable remote task if you are not using it.  Also, it needs ENABLE_TELEMETRY
     * such that it can save remote control codes to non-volatile memory.  IR remote
     * control codes can be learned by typing "learn" command.
     */
    scheduler_add_task(new terminalTask(PRIORITY_HIGH));

    /* Your tasks should probably used PRIORITY_MEDIUM or PRIORITY_LOW because you
     * want the terminal task to always be responsive so you can poke around in
     * case something goes wrong.
     */


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

    scheduler_start(true); ///< This shouldn't return
=======
    //Create the tasks that will be running.
    xTaskCreate(lineFollower, (const char*)"Line Follower", STACK_BYTES(1024), 0, 1, 0);
    xTaskCreate(stateMachine, (const char*)"State Machine", STACK_BYTES(1024), 0, 2, 0);

    vTaskStartScheduler();  //schedule the tasks, should never return/exit.

>>>>>>> refs/heads/master
    return -1;
}
