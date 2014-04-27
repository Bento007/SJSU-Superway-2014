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
#include "shared_handles.h"
#include "lineFollower.hpp"


#define QSEND_DELAY 10     //<how long queues wait to send
#define QRECI_DELAY 100     //<how long queues wait to receive
#define SEM_DELAY 5000  //<how long a tasks waits to get a semaphore
#define SIZE    15      //<Detemines the size of an array

/**********************************
 * Task Priorities
 * --- State Machine:   Low
 * --- Pathing:         Medium
 * --- Wireless:        High
 * --- updateTask:      High
 * --- lineFollowerTask: ???
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
 * update Task
 * -------------
 * Receives data from state machine
 * -through SMtoWireless queue.
 * Can send data to state machine
 * -through WirelesstoSM queue.
 *********************************/
void updateTaskTest(void *p)
{
    // Initialized variables
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = 500;

    xLastWakeTime = xTaskGetTickCount();
    while(1)
    {
        #if DEBUG
            puts("UP Give ticks_sem");
        #endif
        xSemaphoreGive(ticks_sem);
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

void updateTask(void *p)
{
    // Initialized variables
    SNAP& wireless = SNAP::getInstance();

    //Task for testing tick updates
    xTaskCreate(updateTaskTest, (const char*) "updateTaskTest", STACK_BYTES(1024), 0, 1, 0);

    while(1)
    {
        if(xSemaphoreTake(ticks_sem,SEM_DELAY))
        {
            #if DEBUG
                printf("UP update: %i\n",wireless.getLastUpdateTime());
            #endif
            wireless.send_Update();
        }
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
    #if DEBUG
         puts("WT Initializing WT");
    #endif
     SNAP& wireless = SNAP::getInstance();
     wireless.init();

     //Wireless dependent tasks
     xTaskCreate(updateTask, (const char*) "updateTask", STACK_BYTES(1024), 0, 1, 0);

//     podStatus pod;
     uint32_t temp1,temp2;

     while(1)
     {
        #if DEBUG
             puts("WT While loop");
        #endif
         temp1 = 0;
         temp2 = 0;
         if(!wireless.RXempty())
         {
            #if DEBUG
                 puts("WT Inside if");
            #endif
             switch(wireless.get_nextCMD())
             {
                 vTaskDelay(100);
                 case 'U'://get a graph update and send it to pathing //TODO: make graph dynamic
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
                 case 'M'://tell stateMachineTask to change speed for merge//TODO: test
                     temp1 = wireless.get_Merge();
                     //TODO: add shared queue to send to statemachine
                     break;
                 case 'E'://give pathingTask a graph update//TODO: make graph dynamic
                     temp1 = wireless.get_Help();
                     //TODO: add shared queue to send to pathing
                     break;
                 case 'D'://give pathingTask new destination//TODO: check for valid destination inputs
                    #if DEBUG
                         puts("WT Case D");
                    #endif
                     //TODO: add shared queue to send to pathing
                     if(wireless.get_newDest(&temp1))
                     {
//                       Send new destination value to the State Machine
                        #if DEBUG
                             puts("WT Got Value!");
                             printf("%i\n", temp1);
                        #endif

                         LE.on(1);
                         xQueueSend(newDestinationQ, &temp1, QSEND_DELAY);
                     }

                     break;
                 default://send to SNAP invalid CMD
                    #if DEBUG
                         puts("WT inside switch");
                    #endif
                     break;

             }
         }

    #if DEBUG
         puts("WT before if");
    #endif
     if(!wireless.recentlyUpdated(50))//update SNAP at least every 50ms
         wireless.send_Update();
    #if DEBUG
         puts("WT After switch");
    #endif
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

void StateMachine(void *p){
    /*initialize variables here*/
    PRT_States current= startup, next =  startup;
    SM_pkt dir_ary[SIZE], receive;
    path_t travelPath;
    travelPath.destination=0;
    travelPath.source=0;

    #if DEBUG
//        int start, end;
        int dest;
    #endif

    /*
     * Variables the SNAP devices is tracking
     * They will be updated every time the
     * state machine task runs.
     */
    SNAP& wireless = SNAP::getInstance();
    uint32_t speed = 0;         //< current speed or speed mode ex. "Fast, slow, stop"
    uint32_t ticks = 0;         //< ticks till merge, counting down.
    uint32_t loca = 0;          //< current section of track pod is traveling along.

    bool newDest = false;       //< used to determine if the new destination is the same as current location
    /*One time functions*/
    LD.setRightDigit('0');

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
            case startup:   LD.setRightDigit('1');
                /* check for values that didn't initialize, and test hardware
                 * if(everything good)
                 *  next = ready
                 * else
                 *  next = error
                 */

                #if DEBUG
                    puts("SM Startup State");
                #endif

                dest = 0;
//                setup();    //initialize the line follower
                //Read from sensors, do quick check?
                //Send/Receive from SNAP a comm check
                //errorCounter++;
                //if all good, go to ready.
                travelPath.source = 1;            //for debugging
                next = ready;
                break;  //end startup-state

            case ready: LD.setRightDigit('2');
                next = ready; //default until directions received.

                #if DEBUG
                    puts("SM In Ready state");
                #endif

//                printf("\nSource: ");             //for debugging
//                scanf("%i", &start);              //for debugging
//                printf("\nEnd: ");                //for debugging
//                scanf("%i", &end);                //for debugging
//                travelPath.source = start;        //for debugging
//                travelPath.destination = end;     //for debugging
//                travelPath.source = 1;            //for debugging
//                travelPath.destination = 8;       //for debugging
                #if manualCmd

                    while(dest == 0){

                        if(SW.getSwitch(1)){
                            dest = 8;
                        }
                        else if(SW.getSwitch(2))
                            dest = 5;
                        }
                        xQueueSend(newDestinationQ, &dest, QSEND_DELAY);
                        dest = 0;
                    }
                #endif
                if(xQueueReceive(newDestinationQ, &travelPath.destination, QRECI_DELAY)
                        && travelPath.destination != travelPath.source){
                    #if DEBUG
                        puts("SM Destination received");
                        printf("SM Destination: %i\n", travelPath.destination);
                    #endif

                    xQueueSend(SMtoPath, &travelPath, QSEND_DELAY);

                    //Retrieve the list of directions to be sent to the line follower
                    //Contents contained within "array"
//                    if(xQueueReceive(pathToSM, &receive, QRECI_DELAY))
                    if(xQueueReceive(pathToSM, &dir_ary[0], QRECI_DELAY))
//                    dir_ary[SIZE]
                    {
                        int i =0;

                        do{
//                            dir_ary[i] = receive.dir;
                            #if DEBUG
                                printf("SM received: %i @ %i\n",
                                        dir_ary[i].dir, dir_ary[i].loc.name.source);
//                                        receive.dir, receive.loc.name.source);
                            #endif
                            i++;
//                        }while(xQueueReceive(pathToSM, &receive, QRECI_DELAY));
                        }while(xQueueReceive(pathToSM, &dir_ary[i], QRECI_DELAY));
                        #if DEBUG
                            puts("SM Sending to LF");
                        #endif
                        LE.off(1);
                        for(int k=0; k<i; k++)
                        {
                            #if DEBUG
                                printf("SM Sending %i\n", dir_ary[k].dir);
                            #endif
                            xQueueSend(directionQ, &dir_ary[k].dir, QSEND_DELAY);  //send instructions to line follower.
                        }

                        travelPath.source = travelPath.destination;
                        //At this point, directions are received.
                        //TODO: Send directions to line follower task
                        #if DEBUG
                            puts("SM Going to roam");
                        #endif
                        next = roam;
//                        delay_ms(100);
                    }
                }//end if wireless received

                //else //has a direction to get clients
                //next = pickup;
                break;  //end ready-state

            case error: LD.setLeftDigit('A');
//                errorCounter =0;
                //send error status to the snap/computer
                //save error info
                //send stop command to pod
                break;  //end error-state

            case roam:  LD.setRightDigit('3');//Traveling mode
                if(xQueueReceive(lineFollowertoSM, &receive, QRECI_DELAY))
                {
                    #if DEBUG
                        puts("SM Going to ready state");
//                        delay_ms(100);
                    #endif
                    next = pickup;
                }
                #if DEBUG
                    puts("SM roaming");
                #endif
                //busy bit unset, means available.
                //if NO directions
                //--run around for fun
                break;  //end roam-state

            case pickup:    LD.setRightDigit('4');
                //Traveling mode

                //have we reached station for pickup?
                // next = load;

                //else
                //next = pickup;
                next = load;
                #if DEBUG
                    puts("SM pickup");
//                    delay_ms(100);
                #endif
                break;  //end pickup-state

            case load:  LD.setRightDigit('5');
                //Waiting mode:

                //same as unload basically.
                //TODO:am i loaded? PUSH A BUTTON TO INDICATE LOADED
                //if so
                //next = dropoff
                #if DEBUG
                    puts("SM load");
//                    delay_ms(100);
                #endif
                //else if at station, wait.
                //next = load
                next = dropoff;
//                delay_ms(100);
//                vTaskDelay(10);
                break;  //end load-state

            case dropoff:   LD.setRightDigit('6');
                //Traveling mode
                //busy bit set, same as pickup basically.
                //reached correct destination?
                //go to unload

                //else
                next = ready;
                #if DEBUG
                    puts("SM dropoff");
                    delay_ms(100);
                #endif
//                next = unload;
                break;  //end dropoff-state

            case unload:    LD.setRightDigit('7');
                //Waiting mode
                //To simulate unloading, send stop to line follower
                //delay a couple miliseconds to simulate.
                //next = ready

//                next = ready;
//                delay_ms(100);
                #if DEBUG
                    puts("SM unload");
                    delay_ms(100);
                #endif
                break;  //end unload-state

            default:
                next = error;
                break;  //end default-state
        }
        current = next;                                     // store the next state
        wireless.update_SNAP(loca, current,speed, ticks);   //update SNAP object.
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

    //Initialize variables
    SM_pkt directions_ary[SIZE];
    SM_pkt send;
    bool done;
    bool start;
    
    //Initialize Structs
    path_t initPath;
    dijkstra *mainGraph = new dijkstra;

   while(1)
   {
       done = false;
       start = true;
       if(xQueueReceive(SMtoPath, &initPath, QRECI_DELAY)){
        #if DEBUG
           printf("PT Entered Pathing, value: %i %i", initPath.source, initPath.destination);
        #endif
           makeGraph(mainGraph);
           dijkstraFunc(mainGraph, initPath.source);
           print(mainGraph, initPath.source, initPath.destination,directions_ary, SIZE);
       #if DEBUG
           printf("PT Got the directions_ary!\n");
       #endif
       int i=0;

        while(!done)
        {
            send = directions_ary[i];
            #if DEBUG
                printf("PT Printing before SM: Dir:%i location:%i\n",
                        directions_ary[i].dir, directions_ary[i].loc.name.source);
            #endif
            if(!start && send.dir == 0){
                xQueueSend(pathToSM, &send,QSEND_DELAY);
                done = true;
                break;
            }

            xQueueSend(pathToSM, &send, QSEND_DELAY);
            start = false;
            i++;
        }
        #if DEBUG
            printf("\n\n");
        #endif
       }
   }
}



#endif /* SUPERWAYTASKS_HPP_ */
