#include <stdio.h>
#include <stdlib.h>
#include "superwayTasks.hpp"
#include "dijkstra.hpp"
#include "shared_queues.hpp"

int main()
{
    //create the Queues in tasks...
    SMtoPath = xQueueCreate(1, sizeof(path_t));
    directionQ = xQueueCreate(20, sizeof(int));
    newDestinationQ = xQueueCreate(1, sizeof(uint32_t));
    pathToSM = xQueueCreate(20, sizeof(int));
    lineFollowertoSM= xQueueCreate(1, sizeof(int));
    WirelesstoSM = xQueueCreate(1, sizeof(int));
    SMtoLineFollower = xQueueCreate(1, sizeof(int));
    tickEventQ = xQueueCreate(1, sizeof(int));  //experimental

    //create the Semaphores in tasks...
    ticks_sem = xSemaphoreCreateBinary();

    //TODO: create other queues, if used.
    if(debug)   puts("In main, initializing Wireless Task");


    xTaskCreate(wirelessTask, (const char*) "WirelessTask", STACK_BYTES(2048), 0, 2, 0);
    xTaskCreate(StateMachine, (const char*) "SMTask", STACK_BYTES(2048), 0, 1, 0);
    xTaskCreate(pathingTask, (const char*) "DjikstraTask", STACK_BYTES(4096), 0, 3, 0);
    xTaskCreate(lineFollowerTask, (const char*) "LineFollowerTask", STACK_BYTES(2048), 0, 0, 0);
    xTaskCreate(updateTask, (const char*) "updateTask", STACK_BYTES(1024), 0, 0, 0);
    ////Pathing has been made a function instead. Phased out code remains as comments.


    vTaskStartScheduler();
    return -1;
}
