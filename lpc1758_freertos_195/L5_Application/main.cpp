#include <stdio.h>
#include <stdlib.h>
#include "tasks.hpp"
#include "examples/examples.hpp"
#include "superwayTasks.hpp"
#include "dijkstra.hpp"
#include "shared_queues.hpp"

int main()
{
    //create the Queues in tasks...
    SMtoPath = xQueueCreate(1, sizeof(path));
    directionQ = xQueueCreate(20, sizeof(int));
    newDestinationQ = xQueueCreate(1, sizeof(uint32_t));
    //TODO: create other queues, if used.
//    puts("In main, initializing Wireless Task");


//    xTaskCreate(wirelessTask, (const char*) "WirelessTask", STACK_BYTES(2048), 0, 2, 0);
    xTaskCreate(StateMachine, (const char*) "SMTask", STACK_BYTES(2048), 0, 1, 0);
    xTaskCreate(lineFollowerTask, (const char*) "LineFollowerTask", STACK_BYTES(2048), 0, 0, 0);

    ////Pathing has been made a function instead. Phased out code remains as comments.
    xTaskCreate(pathingTask, (const char*) "DjikstraTask", STACK_BYTES(4096), 0, 3, 0);

    vTaskStartScheduler();
    return -1;
}
