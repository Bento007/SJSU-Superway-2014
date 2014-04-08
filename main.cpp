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
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "Header.h"

int main(void)
{
    int mainSrc, destination;
    while(1){
        dijkstra *mainGraph = new dijkstra;
        makeGraph(mainGraph);
        printf("\nSource Vertex: ");
        scanf("%i", &mainSrc);

        printf("\nDestination: ");
        scanf("%i", &destination);

        dijkstraFunc(mainGraph, mainSrc);
        print(mainGraph, mainSrc, destination);
        scanf("%i", &mainSrc);

    }
    scheduler_add_task(new terminalTask(PRIORITY_HIGH));

    scheduler_start(true); ///< This shouldn't return
    return -1;
}
