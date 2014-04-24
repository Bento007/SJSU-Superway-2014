/*
 * shared_queues.hpp
 *
 *  Created on: Apr 22, 2014
 *      Author: Eriberto
 */

#ifndef SHARED_QUEUES_HPP_
#define SHARED_QUEUES_HPP_
#include "FreeRTOS.h"
#include "queue.h"
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
                directionQ;         //State Machine to Line Follower (directions)
//TODO: check if more queues are needed

/* ====================================
 * Shared Struct and Types
 * ===================================
 */
//struct S{   //station
//        int bays;   // number of bays pods can load/load at.
//        int weight; // describes the time and distance it will take to travel.
//};
//
//struct F{   //fork
//        int weight; // describes the time and distance it will take to travel.
//};
//
//struct M{   //merge
//        /* Every length of track feeding into the merge point needs a weight
//         * and the name number of ticks till merge.
//         */
//        int weight1, // describes the time and distance it will take to travel.
//        int ticks;  // number of ticks till merge.
//};
//
//union nodeInfo{
//        struct M *merge;
//        struct F *fork;
//        struct S *station;
//};
//
//
///*node structs describes the different sections of the track*/
//typedef struct node{
//        char type;                  // S=station, M = merge, F = fork, D= depot.
//        char name[4];               // identifies the nodes specific name.
//        union nodeInfo info ;       //points to the info about the node
//}trackSection;
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

#endif /* SHARED_QUEUES_HPP_ */
