/*
 * superwayTasks.hpp
 *
 *  Created on: Apr 6, 2014
 *      Author: Eriberto
 *
 *  Edit on Apr 7, 2014
 *      Editor: Trent
 *      -Added "SNAP.h"
 *      -added functionality to wirelesstask.
 */

#ifndef SUPERWAYTASKS_HPP_
#define SUPERWAYTASKS_HPP_

#include <stdint.h>

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "superwaySPI.hpp"
#include "dijkstra.hpp"
#include "SNAP.H"
#include "io.hpp"
#include "shared_queues.hpp"
#include "lineFollower.hpp"

#define QDelay 100
/**********************************
 * Task Priorities
 * --- State Machine:   Low
 * --- Pathing:         Medium
 * --- Wireless:        High
 ***********************************/


/**********************************
 * Line Follower Task
 * -Uses queue "instructions"
 *
 *
 ***********************************/

void lineFollowerTask(void *p)
{   setup();    //initialize the line follower
    while(1)
    {
        loop();//run lineFollower
    }
}


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
     puts("Initializing WT");
     SNAP& wireless = SNAP::getInstance();
     wireless.init();
//     podStatus pod;
     uint32_t temp1,temp2;
     while(1)
     {
         puts("While loop of WT");
         temp1 = 0;
         temp2 = 0;
         if(!wireless.RXempty())
         {
//             puts("Inside if");
             switch(wireless.get_nextCMD())
             {

                 case 'U'://get a graph update and send it to pathing
                     wireless.get_TrackUpdate(&temp1, &temp2);
                     //TODO: add shared queue to send to pathing
                     /* Description:
                      * In this case, a wireless command was received
                      * to change the weight of a particular part of
                      * the system track, this requires updating the
                      * graph in the pathing algorithm. Needs to be
                      * implemented.
                      */
                     break;
                 case 'M'://tell stateMachineTask to change speed for merge
                     temp1 = wireless.get_Merge();
                     //TODO: add shared queue to send to statemachine
                     break;
                 case 'E'://give pathingTask a graph update
                     temp1 = wireless.get_Help();
                     //TODO: add shared queue to send to pathing
                     break;
                 case 'D'://give pathingTask new destination
                     //TODO: add shared queue to send to pathing
                     wireless.get_newDest(&temp1);
                     //Send new destination value to the State Machine
                     xQueueSend(newDestinationQ, &temp1, 100);
                     break;
                 default://send to SNAP invalid CMD
//                     puts("inside switch");
                     break;

             }
         }
//             puts("before if");
     if(!wireless.recentlyUpdated(50))//update SNAP at least every 50ms
         wireless.send_Update();
//     puts("After switch");
     //     if(xQueueReceive(SMtoWireless, &pod, 100));
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
//     vTaskDelay()
     }

 }



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


void StateMachine(void *p){
    /*initialize variables here*/
    PRT_States current= startup, next =  startup;
//    podStatus pod;                          //Is this needed???
    int array[11], receive;
    path travelPath;
//    int start, end;


    /*
     * Variables the SNAP devices is tracking
     * They will be updated every time the
     * state machine task runs.
     */
    SNAP& wireless = SNAP::getInstance();
//  status = current;           //< status of the pod
    uint32_t speed = 0;         //< current speed or speed mode ex. "Fast, slow, stop"
    uint32_t ticks = 0;         //< ticks till merge, counting down.
    uint32_t loca = 0;          //< current section of track pod is traveling along.


    /*One time functions*/
    LD.setNumber(0);

    //int speedChange;
    //TODO initialize structs to be used with inter-task comms.
    //TODO initialize memory, if any is used
    //int errorCounter=0;
    /*begin state machine*/
    while(1)
    {
        //TODO determine the how long it takes to send a string of 32
        /*reset these values at the beginning of each loop
         * to prevent accidently reuse */

//          Check queues for any information from other tasks.
//        if(xQueueReceive(directions, type, 100))              //new directions arrived.
//        if(xQueueReceive(WirelesstoSM, &speedChange, 100))    //set new speed

        switch(current)
        {
            case startup:   LD.setNumber(1);
                /* check for values that didn't initialize, and test hardware
                 * if(everything good)
                 *  next = ready
                 * else
                 *  next = error
                 */
//            printf("Startup state");
//                setup();    //initialize the line follower
                //Read from sensors, do quick check?
                //Send/Receive from SNAP a comm check
                //errorCounter++;
                //if all good, go to ready.
                next = ready;
//                if(xQueueReceive(directionQ, &receive, 10))
//                {
//                    int i =0;
//                    do{
//                        array[i] = receive;
//                        printf("received: %i\n", receive);
//                        send = array[i];
//                        i++;
//                    }while(xQueueReceive(directionQ, &receive, 10));
//                    next = ready;
//                }
                break;  //end startup-state

            case ready:
                LD.setNumber(2);
                next = ready; //default until directions received.
//                printf("\nSource: ");             //for debugging
//                scanf("%i", &start);              //for debugging
//                printf("\nEnd: ");                //for debugging
//                scanf("%i", &end);                //for debugging
//                initPath.source = start;          //for debugging
//                initPath.destination = end;       //for debugging
                travelPath.source = 1;
                travelPath.destination = 8;
                xQueueSend(SMtoPath, &travelPath, 10);
//                if(xQueueReceive(newDestinationQ, &travelPath.destination, 10)){
                //Makes a call to the pathing algorithm
                //for a new list of directions.
//                dijkstraFunction(travelPath);
//
                //Retrieve the list of directions to be sent to the line follower
                //Contents contained within "array"
                if(xQueueReceive(pathToSM, &receive, 1000))
                {
                    int i =0;
                    do{
                        array[i] = receive;
                        printf("received: %i\n", receive);
                        i++;
                    }while(xQueueReceive(pathToSM, &receive, 1000));

                    for(int k=0; k<i; k++)
                    {
                        xQueueSend(directionQ, &array[k], 1000);  //send instructions to line follower.
                        puts("Sending to LF");
                    }
                    //At this point, directions are received.
                    //TODO: Send directions to line follower task
                    puts("Going to roam");
                    next = roam;
//                    delay_ms(100);
                }
//                }//end if wireless

                //else //has a direction to get clients
                //next = pickup;
                break;  //end ready-state

            case error: LD.setLeftDigit('A');
//                errorCounter =0;
                //send error status to the snap/computer
                //save error info
                //send stop command to pod
                break;  //end error-state

            case roam:  LD.setNumber(3);
                //Traveling mode
                if(xQueueReceive(lineFollowertoSM, &receive, 2000))
                {
                    next = error;
                }
//                puts("roaming");

                //busy bit unset, means available.
                //if NO directions
                //--run around for fun
                break;  //end roam-state

            case pickup:    LD.setNumber(4);
                //Traveling mode

                //have we reached station for pickup?
                // next = load;

                //else
                //next = pickup;
//                next = load;
//                delay_ms(100);
                break;  //end pickup-state

            case load:  LD.setNumber(5);
                //Waiting mode:

                //same as unload basically.
                //TODO:am i loaded? PUSH A BUTTON TO INDICATE LOADED
                //if so
                //next = dropoff

                //else if at station, wait.
                //next = load
//                next = dropoff;
//                delay_ms(100);
//                vTaskDelay(10);
                break;  //end load-state

            case dropoff:   LD.setNumber(6);
                //Traveling mode
                //busy bit set, same as pickup basically.
                //reached correct destination?
                //go to unload

                //else
                //next = dropoff

//                next = unload;
//                delay_ms(100);
                break;  //end dropoff-state

            case unload:    LD.setNumber(7);
                //Waiting mode
                //To simulate unloading, send stop to line follower
                //delay a couple miliseconds to simulate.
                //next = ready

//                next = ready;
//                delay_ms(100);
                break;  //end unload-state

            default:
                next = error;
                break;  //end default-state
        }
        current = next;                                     // store the next state
//        wireless.update_SNAP(loca, current,speed, ticks);   //update SNAP object.
    }

}


/***************************************
 * Djikstra Task (Phased out code)
 * -------------
 * Sends info to state machine thru
 * -directions queue.
 * Receives data from SM thru
 * -SMtoPath queue.
 ***************************************/

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
//         -List of directions via the directionQ queue. Can be array with
//          instructions for each "node"

    path initPath;

   while(1)
   {
       int size = 15;
       int array[size];
       int send = 50;
       bool done = false;
       bool start = true;
       if(xQueueReceive(SMtoPath, &initPath, 100)){
           dijkstra *mainGraph = new dijkstra;
           makeGraph(mainGraph);
           dijkstraFunc(mainGraph, initPath.source);

           int *directions=print(mainGraph, initPath.source, initPath.destination, array, size);
//            printf("Got the array!\n");
           int i=0;

        while(!done)
        {
            send = directions[i];
            printf("Printing before SM: %i\n", send);

            if(!start && send == 0){
                xQueueSend(pathToSM, &send,0);
                done = true;
                break;
            }

            xQueueSend(pathToSM, &send, 10);
            start = false;
            i++;
        }
           printf("\n\n");
       }
   }
}



#endif /* SUPERWAYTASKS_HPP_ */
