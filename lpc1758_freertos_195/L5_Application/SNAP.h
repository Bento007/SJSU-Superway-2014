/*
 * SNAP.h
 *
 *  Created on: Mar 28, 2014
 *      Author: Bento007
 *  The snap devices uses uart3 to send and receive data
 */

#ifndef SNAP_H_
#define SNAP_H_

//#include "scheduler_task.hpp"
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
/* '<-' is an incoming command to sjone board
 * '->' is an outgoing command to snap
 * '<->' is a command with the same incoming and outgoing command
 */

        bool send_Update(uint32_t location, uint8_t status, uint32_t speed);    //cmd -> 'U'updates the locals on the snap
        bool send_Estimated_Time_to_Merge(uint32_t speed, int ticks);  //cmd -> 'M'sends the estimated time to merge to the SNAP
        bool send_Merge(uint32_t location,uint8_t status,uint32_t speed, int ticks);//cmd -> 'M' sends a merge command to SNAP
        bool send_Help(uint8_t status, uint32_t location );                         //cmd -> 'E' sends help to SNAP
        bool send_Test();            //tests that the snap is working
        void send_Time();   //X<-cmd, R->cmd sends current RTC

        int  get_Dest();    //cmd <-> 'D' TODO:get new destination from network
        bool get_Update(uint32_t* dest,uint32_t* weight); // cmd <- 'U' get an update for the graph
        char get_CMD();     //get the command from the snap
        int get_Help();    //cmd <- 'E' get the help command from snap
        int get_Merge();    //cmd <- 'M' get the new time to merge

        void setup_Time();     //cmd -> 'X' get the system time from network through snap **Caution This function flushes the current RX buffer**

//        //private member setters
//        void setStatus(uint8_t input){status = input;};
//        void setSpeed(uint32_t input){speed = input;};
//        void setTicks(uint32_t input){ticks = input;};
//
//        //private member getters
//        uint8_t getStatus(){return status;};
//        uint32_t getSpeed(){return speed;};
//        uint32_t getTicks(){return ticks;};

    private:
//          rtc_t time;   // TODO time last updated
//        static uint8_t status;
//        static uint32_t speed;
//        static uint32_t ticks;
};

#endif /* SNAP_H_ */
