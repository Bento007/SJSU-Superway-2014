/*
 * StateMachine.hpp
 *
 *  Created on: Mar 12, 2014
 *      Author: Bento007
 *
 *      This is is the brains of the pod, and the core of it's
 *      functionality
 */

#ifndef STATEMACHINE_HPP_
#define STATEMACHINE_HPP_

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

/* To communicate with wireless network!
 *
 * #include "uart3.hpp"
 *
 * grab an instance of uart 3. //TODO: create a semifore to control UART3 between tasks
 *  Uart3& snap = Uart3::getInstance();
 *
 * READ:
 *  snap.gets(char* ,int NumberToRead ,int timeToWait);
 * Example:
 *  char str[16];   //where the incoming str is read into.
 *  snap.gets(str ,16 ,100); //read 16 characters from SNAP
 *
 * WRITE:
 *  snap.putChar(char, int timeToWait); //TODO: make compatible with char*
 * NOTE! Only one character can be put into the queue per putChar()
 * NOTE! SNAP will not receive the string unless you end the message with '\n'
 * Example:
 *  snap.putChar('H', 100);
 *  snap.putChar('E', 100);
 *  snap.putChar('L', 100);
 *  snap.putChar('L', 100);
 *  snap.putChar('O', 100);
 *  snap.putChar('\n', 100);// terminating character
 *
 */

enum PRT_States{
    startup=1,  // initializes everything
    ready,      // waiting for direction
    error,      // error detected or initialize failed
    roam,       // no directions received driving around track
    pickup,     // directions received going to pick up location
    load,       // at stations loading
    dropoff,    // in route to drop off
    unload      // at destination and unloading passengers
};

struct S{   //station
        int bays;   // number of bays pods can load/load at.
        int weight; // describes the time and distance it will take to travel.
};

struct F{   //fork
        int weight; // describes the time and distance it will take to travel.
};

struct M{   //merge
        /* Every length of track feeding into the merge point needs a weight
         * and the name number of ticks till merge.
         */
        int weight1, // describes the time and distance it will take to travel.
            weight2;
//        .
//        .
//        .
//        weightN;
        int ticks;  // number of ticks till merge.
};

union nodeInfo{
        struct M *merge;
        struct F *fork;
        struct S *station;
};

/*node structs describes the different sections of the track*/
typedef struct node{
        char type;      // S=station, M = merge, F = fork, D= depot.
        char name[4];   // identifies the nodes specific name.
        union nodeInfo info ;    //points to the info about the node
};

void StateMachine()
{
    /*initialize variables here*/
    PRT_States current= startup, next;
    //QueueHandle_t directions;
    //node map[x][y];

    /*begin state machine*/
    while(1)
    {
        switch(current)
        {
            case startup:
                /* check for values that didn't initialize, and test hardware
                 * if(everything good)
                 *  next = ready
                 * else
                 *  next = error
                 */
                break;
            case ready:
                /* if(next direction == NULL)
                 *  nextState
                 * else
                 *  next
                 * check if directions queue is empty*/
                break;
            case error: break;
            case roam: break;
            case pickup: break;
            case load: break;
            case dropoff: break;
            case unload: break;
            case startup: break;
            case ready: break;
            default: break;
        }
        current = next;// store the next state
    }

}



#endif /* STATEMACHINE_HPP_ */
