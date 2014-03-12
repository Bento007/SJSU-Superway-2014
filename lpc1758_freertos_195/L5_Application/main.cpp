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
#include "utilities.h"
#include "examples/examples.hpp"
#include "stdio.h"
#include "io.hpp"
#include "fat/disk/spi_flash.h"
#include "eint.h"
#include "uart0_min.h"

//xQueueHandle qh = 0; //global queue handle

void task_rx(void*p)
{
    while(1)
    {
        //trial 1.a message
        //uart0_puts("Inside of rx.");

        //trial 2.a message
        uart0_puts("Airybeartoe is my name, im task 1.");

        vTaskDelay(1000);
    }
}

void task_tx(void*p)
{
    while(1)
    {
        //Trial 1.b message
        //uart0_puts("Inside of the tx task.");

        //Trial 2.b message
        uart0_puts("Task two, I am. How are you today?");
        vTaskDelay(1500);
    }
}

int main(void)
{
   //qh= xQueueCreate(1, sizeof(int));
   xTaskCreate(task_tx, (const char*)"t1", STACK_BYTES(1024), 0, 1, 0);
   xTaskCreate(task_rx, (const char*)"t2", STACK_BYTES(1024), 0, 2, 0);
   vTaskStartScheduler();
   return -1;
}
