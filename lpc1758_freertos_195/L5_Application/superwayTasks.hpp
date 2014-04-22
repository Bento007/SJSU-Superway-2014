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
                directionQ;         //Pathing (djikstra) -> State machine
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
     SNAP& wireless = SNAP::getInstance();
//     podStatus pod;
     uint32_t temp1,temp2;
     while(1)
     {
         temp1 = 0;
         temp2 = 0;
         if(!wireless.RXempty())
         {
             switch(wireless.get_nextCMD())
             {
                 case 'U'://get a graph update and send it to pathing
                     wireless.get_TrackUpdate(&temp1, &temp2);
                     //TODO: add shared queue to send to pathing
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
                     temp1 = wireless.get_newDest(&temp1);
                     break;
                 default://send to SNAP invalid CMD
                     break;

             }
         }
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

     }

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

               if(!start && send == 0){
                   xQueueSend(directionQ, &send,0);
                   done = true;
                   break;
               }

               xQueueSend(directionQ, &send, 10);
               start = false;
               i++;
           }
           printf("\n\n");
       }
   }
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
//    Uart3& snap = Uart3::getInstance(); //initialize the snap UART3
    PRT_States current= startup, next;
    podStatus pod;
    int array[11], receive;
    uint8_t send;

    /*Initializes SPI (SSP) for comm with Arduino*/

    /*One time functions*/
    LD.setNumber(0);

    //int speedChange;
    //TODO initialize structs to be used with inter-task comms.
    //TODO initialize memory, if any is used
    //int errorCounter=0;
    /*begin state machine*/
    while(1)
    {
        /*reset these values at the beginning of each loop
         * to prevent accidently reuse */

//        To send from SM to line follower just do the following.
//        if(!xQueueSend(directives, &command, 100))
//            puts("Failed!\n");

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

                //send command to arduino to "test" motors
                //Read from sensors, do quick check?
                //Send/Receive from SNAP a comm check
                //Send/Receive a command to djikstra task, verify its alive.
                //errorCounter++;
                if(xQueueReceive(directionQ, &receive, 10))
                {
                    int i =0;
        //            ssp1_ExchangeByte(0x55); //ready
                    do{
        //                LPC_GPIO0->FIODIR |= (1 << 30);  //set as output
        //                LPC_GPIO0->FIOSET = (1 << 30);   //initially as high
        //                LPC_GPIO0->FIOCLR = (1 << 30);   //drive low to CS
                        array[i] = receive;
                        printf("received: %i\n", receive);
                        send = array[i];
        //                ssp1_ExchangeByte(send); // send to arduino
        //                ssp1_ExchangeByte(0x00);
        //                acked = ssp1_ExchangeByte(0x00);

        //                if(acked== 0x10)
        //                    printf("acked %x\n", acked);
                        i++;
        //                LPC_GPIO0->FIOSET = (1 << 30);  //slave select goes High.
        //                delay_ms(50);
                    }while(xQueueReceive(directionQ, &receive, 10));
                }
                break;

            case ready: LD.setNumber(2);
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

            case error: LD.setLeftDigit('A');
//                errorCounter =0;
                //send error status to the snap/computer
                //save error info
                //send stop command to pod
                break;

            case roam:  LD.setNumber(3);
                //Traveling mode

                //busy bit unset, means available.
                //if NO directions
                //--run around for fun
                break;

            case pickup:    LD.setNumber(4);
                //Traveling mode

                //have we reached station for pickup?
                // next = load;

                //else
                //next = pickup;
                break;

            case load:  LD.setNumber(5);
                //Waiting mode:
                //PUSH A BUTTON TO INDICATE LOADED
                //same as unload basically.
                //TODO:am i loaded?
                //if so
                //next = dropoff

                //else
                //next = load
                break;

            case dropoff:   LD.setNumber(6);
                //Traveling mode
                //busy bit set, same as pickup basically.
                //reached correct destination?
                //go to unload

                //else
                //next = dropoff
                break;

            case unload:    LD.setNumber(7);
                //Waiting mode
                //To simulate unloading, send stop to line follower
                //delay a couple miliseconds to simulate.
                //next = ready
                break;
            default:
                next = error;
                break;
        }
        current = next;// store the next state
    }

}

#endif /* SUPERWAYTASKS_HPP_ */
