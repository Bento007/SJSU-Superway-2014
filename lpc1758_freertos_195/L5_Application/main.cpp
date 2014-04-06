#include <stdio.h>
#include <stdlib.h>
#include "tasks.hpp"
#include "examples/examples.hpp"
#include "superwayTasks.hpp"

int main()
{

    xTaskCreate(StateMachine, (const char*) "SMTask", STACK_BYTES(1024), 0, 0, 0);
    xTaskCreate(pathingTask, (const char*) "DjikstraTask", STACK_BYTES(4096), 0, 1, 0);
    xTaskCreate(wirelessTask, (const char*) "WirelessTask", STACK_BYTES(4096), 0, 2, 0);
    vTaskStartScheduler();
    return -1;
}
