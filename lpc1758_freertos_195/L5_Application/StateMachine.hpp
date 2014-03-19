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
#define QDelay 100

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
    startup =1,    // initializes everything
    ready   =2,    // waiting for direction
    error   =3,    // error detected or initialize failed
    roam    =4,    // no directions received driving around track
    pickup  =5,    // directions received going to pick up location
    load    =6,    // at stations loading
    dropoff =7,    // in route to drop off
    unload  =8     // at destination and unloading passengers
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
        char type;                  // S=station, M = merge, F = fork, D= depot.
        char name[4];               // identifies the nodes specific name.
        union nodeInfo info ;       //points to the info about the node
};


void StateMachine()
{
    /*initialize variables here*/
    Uart3& snap = Uart3::getInstance(); //initialize the snap UART3
    PRT_States current= startup, next;
    //TODO initialize variable/buffer to store messages in.
    //TODO

    //TODO initialize memory, if any is used
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

                //Send a char, to check comm works
                snap.putChar('H', QDelay);
                snap.putChar('\n', QDelay);
                char *b;

                //TODO add initialization function to line follower
                //-----have it return a value through queue handles to use here.

                if(snap.gets(b, 1, QDelay)){
                //for now checks UART3 then moves on
                //add more conditions later.
                    next = ready;
                }

                else{
                //At least one check has failed
                    next =  error;
                }

                break;

            case ready:
                /* if(next direction == NULL)
                 * --nextState
                 * else
                 * --next
                 * check if directions queue is empty
                 */
                //if(direction ==NULL)//no directions for client
                    next = roam;

                //else //has a direction to get clients
                    next = pickup;

                break;

            case error:
                //send error status to the snap/computer
                //save error info
                //send stop command to pod?
                    //-use commandQueue
                break;

            case roam:
                //Traveling mode

                //if NO directions
                //--run around for fun
                //--or head to a depot
                break;

            case pickup:
                //Traveling mode

                //have we reached station for pickup?
                // next = load;

                //else
                //next = pickup;
                break;

            case load:
                //Waiting mode:
                //to simulate a load sequence, wait a few seconds.

                //TODO:am i loaded?
                //if so
                //next = dropoff

                //else
                //next = load
                break;

            case dropoff:
                //Traveling mode

                //reached correct destination?
                //go to unload

                //else
                //next = dropoff
                break;

            case unload:
                //Waiting mode
                //to simulate an unload sequence, wait a few seconds
                //next = ready
                break;

//these states are doubles or unneeded
//            case startup: break;
//
//            case ready: break;
//
//            default: break;
        }

        current = next;// store the next state
    }

}


#undef QDelay
#endif /* STATEMACHINE_HPP_ */
