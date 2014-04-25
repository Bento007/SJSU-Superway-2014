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
//#include "rtc.h"
#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "singleton_template.hpp"  // Singleton Template

//class SNAP : public scheduler_task    //for scheduler
class SNAP : public SingletonTemplate<SNAP>//TODO make event driven. When data is received
{
    public:
        /**
         * Initializes SNAP and Uart3
         */
        bool init();

//        addSharedObject();
/* '<-' is an incoming command to sjone board
 * '->' is an outgoing command to snap
 * '<->' is a command with the same incoming and outgoing command
 */

        bool send_Update();     //cmd -> 'U'updates the locals on the snap
        bool send_Estimated_Time_to_Merge(uint32_t speed, int ticks);  //cmd -> 'M'sends the estimated time to merge to the SNAP
        bool send_Merge();//cmd -> 'M' sends a merge command to SNAP
        bool send_Help(uint8_t status, uint32_t location );                         //cmd -> 'E' sends help to SNAP
        bool send_Test();            //tests that the snap is working
        void send_Time();   //X<-cmd, R->cmd sends current RTC

        bool get_newDest(uint32_t *dest);    //cmd <-> 'D' TODO:get new destination from network
        bool get_TrackUpdate(uint32_t* dest,uint32_t* weight); // cmd <- 'U' get an update for the graph
        char get_nextCMD();     //get the command from the snap     //get the command from the snap
        int get_Help();    //cmd <- 'E' get the help command from snap
        int get_Merge();    //cmd <- 'M' get the new time to merge

        void setup_Time();     //cmd -> 'X' get the system time from network through snap **Caution This function flushes the current RX buffer**

//        bool recentlyActive(uint32_t ms);
        bool RXempty(); //checks if rx is empty
		void update_SNAP(uint32_t loc,uint8_t sta,uint32_t spe, uint32_t tic);

//        inline TickType_t getLastUpdateTime(void) const { return mLastActivityTime; }
		inline TickType_t getLastUpdateTime(void) const
		{
            TickType_t lastTimeStampMs = MS_PER_TICK() * mLastActivityTime;
            TickType_t currentTimeMs = xTaskGetMsCount();
            return (currentTimeMs - lastTimeStampMs);
		}
        inline void resetUpdateTime(void) { mLastActivityTime = xTaskGetMsCount(); }
        bool recentlyUpdated(unsigned int ms) const
        {
            TickType_t lastTimeStampMs = MS_PER_TICK() * mLastActivityTime;
            TickType_t currentTimeMs = xTaskGetMsCount();
            return (currentTimeMs - lastTimeStampMs) < ms;
        }

    private:
        SNAP();  ///< Private constructor of this Singleton class
        friend class SingletonTemplate<SNAP>;  ///< Friend class used for Singleton Template

        uint8_t status;
        uint32_t speed;
        uint32_t ticks;
        uint32_t location;
        uint32_t destination;
        TickType_t mLastActivityTime;   ///< updated each time an update is sent
};

#endif /* SNAP_H_ */
