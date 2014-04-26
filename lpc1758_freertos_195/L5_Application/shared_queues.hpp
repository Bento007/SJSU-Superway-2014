///*
// * shared_queues.hpp
// *
// *  Created on: Apr 22, 2014
// *      Author: Eriberto
// */
//
//#ifndef SHARED_QUEUES_HPP_
//#define SHARED_QUEUES_HPP_
//#include "FreeRTOS.h"
//#include "queue.h"
//#include "semphr.h"
///**********************************
// * Superway Shared Queues
// **********************************/
//QueueHandle_t   SMtoWireless,       //State machine -> wireless
//                WirelesstoSM,       //Wireless -> State machine
//                newDestinationQ,    //Wireless dest -> State machine
//                SMtoLineFollower,   //State machine -> Line follower
//                lineFollowertoSM,   //Line follower -> State Machine
//                SMtoPath,           //State machine -> Pathing (djikstra)
//                pathToSM,           //Pathing (djikstra) -> State machine
//                directionQ,         //State Machine to Line Follower (directions)
//                tickEventQ;         //Wakes the updateTask when a Tick has been reached.
////TODO: check if more queues are needed
//
//#define DEBUG false     //Used to debug software. Will enable printf and puts commands.
//
//#define manualCmd true  //Used to debug without Wireless communications.
//                        //Uses onboard buttons to control destination of pod.
//
///**********************************
// * Superway Shared Semaphores
// **********************************/
//
//SemaphoreHandle_t ticks_sem;    //used to signal updateTask to go.
//
///**********************************
// * Superway Strucutures
// **********************************/
//
//typedef struct path_t{
//        uint32_t source;
//        uint32_t destination;
//}pods;
//
//typedef enum track_t{
//    stations = 1,
//    merge = 2,
//    fork = 3,
//    forkL = 4,
//    forkR = 5
//};
//
//struct gNode
//{
//    uint32_t weight;
//    uint32_t ticks;
//    track_t type;
//    path_t name;
//    bool station;
//    bool merge;
//    bool fork;
//    bool right;
//};
//
//#endif /* SHARED_QUEUES_HPP_ */
