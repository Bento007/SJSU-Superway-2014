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
/************************************
 * State Machine Code Below
 ***********************************/
enum PRT_States{
    startup =1,     // initializes everything
    ready   =2,     // waiting for direction
    error   =3,     // error detected or initialize failed
    roam    =4,     // no directions received driving around track
    pickup  =5,     // directions received going to pick up location
    load    =6,     // at stations loading
    dropoff =7,     // in route to drop off
    unload  =8,     // at destination and unloading passengers
    emergency = 9   // an emergency has occured
};

//Directives will be sent to a queue that the line follower will receive and
//will respond accordingly.
enum Directives{
    stop    =0,     //Make a full stop
    forward =1,     //Continue going straight
    turn    =2,     //Break from straight line, make the turn off
    yield   =3,     //Slow the pod down
    tMotors =100,   //command to initiate motor check
    tSpdRd  =200,   //command to initiate speed sensor check
    tQueue  =300    //command to test communication with line follower
    //wait    =4      //Hold position
};

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

struct WT_pkt
{
        uint32_t ticks;
        track_t type;
        path_t name;
        PRT_States status;
};

struct SM_pkt
{
        uint32_t dir;   //direction
        WT_pkt loc;     //location data sent to wireless
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
