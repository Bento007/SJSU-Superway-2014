/*
 * superwayTasks.hpp
 *
 *  Created on: Apr 6, 2014
 *      Author: Eriberto
 */

#ifndef SUPERWAYTASKS_HPP_
#define SUPERWAYTASKS_HPP_

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "tasks.hpp"
#define QDelay 100
/**********************************
 * Task Priorities
 * --- State Machine:   Low
 * --- Pathing:         Medium
 * --- Wireless:        High
 ***********************************/

/**********************************
 * Shared Queues
 **********************************/
QueueHandle_t   SMtoWireless,       //State machine -> wireless
                WirelesstoSM,       //Wireless -> State machine
                SMtoLineFollower,   //State machine -> Line follower
                lineFollowertoSM,   //Line follower -> State Machine
                SMtoPath,           //State machine -> Pathing (djikstra)
                directions;         //Pathing (djikstra) -> State machine
//TODO: check if more queues are needed

/**********************************
 * Wireless Task
 * -------------
 * Receives data from state machine
 * -through SMtoWireless queue.
 * Can send data to state machine
 * -through WirelesstoSM queue.
 *********************************/
 void wirelessTask(void *p)
 {
//     podStatus pod;

//     if(xQueueReceive(SMtoWireless, &pod, 100);
//     Info was requested by a SNAP so send: the pod's speed, location, and name.
//     from inside podStatus "pod" struct. This is retrieved from State machine task.

//     if this pod receives, from another snap, the following
//     -Destination:
//        Then send new destination to pathing, for new path.
//     -Speed:
//        Send new speed to SM task to set in line follower.
//     -Update:
//        This request requires pod to send its location, speed, and "status"
//             Note: Status not yet implemented. (OPTIONAL).
//     -Time:
//        Not sure what to do with this... yet. (optional?)

 }

/***********************************
 * Djikstra Task
 * -------------
 * Sends info to state machine thru
 * -directions queue.
 * Receives data from SM thru
 * -SMtoPath queue.
 ***********************************/
void pathingTask(void *p)
{
//    From Wireless task, may receive:
//         -New destination to calculate path for.
//         -Some update status for the map. Maybe a route is blocked.
//
//    From Fat32, will receive:
//         -Contents, the map.
//
//    To Fat32, will send:
//         -New map weights. Routes may be inaccessible now.
//
//    To State machine, will send:
//         -List of directions via the directions queue. Can be array with
//          instructions for each "node"
}

/************************************
 * State Machine Code Below
 ***********************************/

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
    startup =1,     // initializes everything
    ready   =2,     // waiting for direction
    error   =3,     // error detected or initialize failed
    roam    =4,     // no directions received driving around track
    pickup  =5,     // directions received going to pick up location
    load    =6,     // at stations loading
    dropoff =7,     // in route to drop off
    unload  =8      // at destination and unloading passengers
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
}trackSection;

typedef struct{
        /***********************************************
         * podStatus structs are passed from the State
         * machine to the Wireless task upon request
         * from the Wireless task. Can occur at any time.
         ***********************************************/
        char name[4];
        char type;
        int speed;
}podStatus;


void StateMachine(void *p)
{
    /*initialize variables here*/
    //Uart3& snap = Uart3::getInstance(); //initialize the snap UART3
    PRT_States current= startup, next;
    podStatus pod;
    //int speedChange;
    //TODO setup SPI for arduino comms, need to test with other project first.
    //TODO initialize structs to be used with inter-task comms.
    //TODO initialize memory, if any is used
    //int errorCounter=0;
    /*begin state machine*/
    while(1)
    {

//        To send from SM to line follower just do the following.
//        if(!xQueueSend(directives, &command, 100))
//            puts("Failed!\n");

//          Check queues for any information from other tasks.
//        if(xQueueReceive(directions, type, 100))              //new directions arrived.
//        if(xQueueReceive(WirelesstoSM, &speedChange, 100))    //set new speed

        switch(current)
        {
            case startup:
                /* check for values that didn't initialize, and test hardware
                 * if(everything good)
                 *  next = ready
                 * else
                 *  next = error
                 */

                //send command to arduino to "test" motors
                //Read from sensors, do quick check?
                //Send/Receive from SNAP a comm check
                //Send/Receive a command to djikstra task, verify its alive.
                //errorCounter++;
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
                //next = pickup;
                break;

            case error:
//                errorCounter =0;
                //send error status to the snap/computer
                //save error info
                //send stop command to pod
                break;

            case roam:
                //Traveling mode

                //busy bit unset, means available.
                //if NO directions
                //--run around for fun
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

                //same as unload basically.
                //TODO:am i loaded?
                //if so
                //next = dropoff

                //else
                //next = load
                break;

            case dropoff:
                //Traveling mode
                //busy bit set, same as pickup basically.
                //reached correct destination?
                //go to unload

                //else
                //next = dropoff
                break;

            case unload:
                //Waiting mode
                //To simulate unloading, send stop to line follower
                //delay a couple miliseconds to simulate.
                //next = ready
                break;

        }

        current = next;// store the next state
    }

}

#endif /* SUPERWAYTASKS_HPP_ */
