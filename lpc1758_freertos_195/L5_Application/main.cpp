#include <stdio.h>
#include <stdlib.h>
#include "tasks.hpp"
#include "examples/examples.hpp"
#include "superwayTasks.hpp"
#include "dijkstra.hpp"

int main()
{
    //create the Queues in tasks...
    SMtoPath = xQueueCreate(1, sizeof(path));
    directionQ = xQueueCreate(20, sizeof(int));
    //TODO: create other queues, if used.

    xTaskCreate(StateMachine, (const char*) "SMTask", STACK_BYTES(1024), 0, 0, 0);
    xTaskCreate(pathingTask, (const char*) "DjikstraTask", STACK_BYTES(4096), 0, 1, 0);
    xTaskCreate(wirelessTask, (const char*) "WirelessTask", STACK_BYTES(4096), 0, 2, 0);
    vTaskStartScheduler();
    return -1;
}
