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
        SNAP();
//        bool run(void *p);                  ///< The main loop for scheduler


        bool send_Update(int location, int status, int speed);    //updates the locals on the snap
        bool send_Estimated_Time_to_Merge(int speed, int ticks);  //sends the estimated time to merge to the SNAP
        bool send_Merge(int speed, int ticks);                    //sends a merge command to SNAP
        bool send_Help(int status);                               //sends help to SNAP
        bool send_Test();            //tests that the snap is working

        int  get_Dest();    //TODO:get new destination from network
        void get_Time();     //get the system time from network through snap
        bool get_Data(int size); //gets data[size] from the snap
        char get_CMD();     //get the command from the snap


//        void setTime(); //TODO sets the time of the RTC

    private:
          rtc_t time;   // TODO time last updated

};

#endif /* SNAP_H_ */
