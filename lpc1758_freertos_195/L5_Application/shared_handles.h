/*
 * shared_queues.hpp
 *
 *  Created on: Apr 22, 2014
 *      Author: Eriberto
 */

#ifndef SHARED_HANDLES_HPP_
#define SHARED_HANDLES_HPP_
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
/**********************************
 * Superway Shared Queues
 **********************************/
QueueHandle_t   SMtoWireless,       //State machine -> wireless
                WirelesstoSM,       //Wireless -> State machine
                newDestinationQ,    //Wireless dest -> State machine
                SMtoLineFollower,   //State machine -> Line follower
                lineFollowertoSM,   //Line follower -> State Machine
                SMtoPath,           //State machine -> Pathing (djikstra)
                pathToSM,           //Pathing (djikstra) -> State machine
                directionQ,         //State Machine to Line Follower (directions)
                tickEventQ;         //Wakes the updateTask when a Tick has been reached.
//TODO: check if more queues are needed

#define DEBUG true     //Used to debug software. Will enable printf and puts commands.

#define manualCmd false  //Used to debug without Wireless communications.
                        //Uses onboard buttons to control destination of pod.

/**********************************
 * Superway Shared Semaphores
 **********************************/

SemaphoreHandle_t ticks_sem;    //used to signal updateTask to go.

/**********************************
 * Superway Strucutures
 **********************************/

typedef struct path_t{
        uint32_t source;
        uint32_t destination;
}pods;

typedef enum track_t{
    stations = 1,
    merge = 2,
    fork = 3,
    forkL = 4,
    forkR = 5
};

struct gNode
{
    uint32_t weight;
    uint32_t ticks;
    track_t type;
    path_t name;
    bool station;
    bool merge;
    bool fork;
    bool right;
};

/*
 *     SocialLedge.com - Copyright (C) 2014
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
 *
 * @file
 * @brief Contains enumeration of shared handles used in the project.
 * @see   scheduler_task::addSharedHandle()
 *
 * 20140215     : Initial
 */

/**
 * Enumeration of shared handles
 */
enum {
    shared_SensorQueue,    ///< Shared handle used by examples (producer and consumer tasks)
    shared_learnSemaphore, ///< Terminal command gives this semaphore to remoteTask (IR sensor task)
};


#endif /* SHARED_HANDLES_HPP_ */
