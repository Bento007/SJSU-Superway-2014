/*
 * superway_tasks.hpp
 *
 *  Created on: Apr 18, 2014
 *      Author: Bento007
 */

#ifndef SUPERWAY_TASKS_HPP_
#define SUPERWAY_TASKS_HPP_

#include "scheduler_task.hpp"
#include "soft_timer.hpp"
#include "superwaySPI.hpp"
#include "dijkstra.hpp"
#include "SNAP.H"
#include "io.hpp"

/**********************************
 * Wireless Task
 * -------------
 * Receives data from state machine
 * -through SMtoWireless queue.
 * Can send data to state machine
 * -through WirelesstoSM queue.
 *********************************/
class wirelessTask : public scheduler_task
{
    public:
        wirelessTask(uint8_t priority): scheduler_task("wireless", 512, priority)     ///< Constructor
        {
            temp1 = 0;
            temp2 = 0;
        };

        bool taskEntry(void)               ///< Registers commands.
        {
            Uart3& wireless = Uart3::getInstance();
            wireless.init(19200,64,32);
        }

        bool run(void *p)                  ///< The main loop
        {
//            if(something on uart)
            switch(me.get_CMD())
            {
                case 'U'://get a graph update and send it to pathing
                    me.get_Update(&temp1, &temp2);
                    //TODO: add shared queue to send to pathing
                    break;
                case 'M'://tell stateMachineTask to change speed for merge
                    temp1 = me.get_Merge();
                    //TODO: add shared queue to send to statemachine
                    break;
                case 'E'://give pathingTask a graph update
                    temp1 = me.get_Help();
                    //TODO: add shared queue to send to pathing
                    break;
                case 'D'://give pathingTask new destination
                    //TODO: add shared queue to send to pathing
                    temp1 = me.get_Dest();
                    break;
                default://send to SNAP invalid CMD
                    break;
            }
            return true;
        }

    private:
//        SoftTimer mCmdTimer;           ///< Command timer
        SNAP me;
        uint32_t temp1,temp2;

};
#endif /* SUPERWAY_TASKS_HPP_ */
