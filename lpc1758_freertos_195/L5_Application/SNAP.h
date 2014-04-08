/*
 * SNAP.h
 *
 *  Created on: Mar 28, 2014
 *      Author: Bento007
 *  The snap devices uses uart3 to send and receive data
 */

#ifndef SNAP_H_
#define SNAP_H_

#include "scheduler_task.hpp"
#include <stdint.h>
#include "rtc.h"

//class SNAP : public scheduler_task    //for scheduler
class SNAP
{
    public:
//        SNAP(uint8_t priority);   //for scheduler
//        bool run(void *p);                  ///< Before FreeRTOS
//        bool taskEntry(void);               ///< After FreeRTOS
//        bool run(void *p);                  ///< The main loop
        SNAP();

//        addSharedObject();

        bool send_Update(uint32_t location, uint8_t status, uint32_t speed);    //updates the locals on the snap
        bool send_Estimated_Time_to_Merge(uint32_t speed, int ticks);  //sends the estimated time to merge to the SNAP
        bool send_Merge(uint32_t location,uint8_t status,uint32_t speed, int ticks);                    //sends a merge command to SNAP
        bool send_Help(uint8_t status, uint32_t location );                               //sends help to SNAP
        bool send_Test();            //tests that the snap is working

        int  get_Dest();    //TODO:get new destination from network
        bool get_Update(uint32_t* dest,uint32_t* weight); //get an update for the graph
        void get_Time();     //get the system time from network through snap
        char get_CMD();     //get the command from the snap
        int get_Help();    //get the help command from snap
        int get_Merge();    //get the new time to merge

//        void setTime(); //TODO sets the time of the RTC

    private:
          rtc_t time;   // TODO time last updated
          uint8_t status;
          uint32_t speed;
          uint32_t ticks;

};

#endif /* SNAP_H_ */
