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




#endif /* SHARED_QUEUES_HPP_ */
