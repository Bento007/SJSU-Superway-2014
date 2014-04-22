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
#include "queue.h"
#include "semphr.h"
#include "task.h"
#include "singleton_template.hpp"  // Singleton Template

//class SNAP : public scheduler_task    //for scheduler
class SNAP : public SingletonTemplate<SNAP>
{
    public:
        /**
         * Initializes SNAP and Uart3
         */
        bool init();

        bool send_Update();    //updates the locals on the snap
        bool send_Estimated_Time_to_Merge(uint32_t speed, int ticks);  //sends the estimated time to merge to the SNAP
        bool send_Merge(uint32_t location,uint8_t status,uint32_t speed, int ticks);                    //sends a merge command to SNAP
        bool send_Help(uint8_t status, uint32_t location );                               //sends help to SNAP
        bool send_Test();            //tests that the snap is working

        bool get_newDest(uint32_t* dest);    //TODO:get new destination from network
        bool get_TrackUpdate(uint32_t* dest,uint32_t* weight); //get an update for the graph
        void get_Time();     //get the system time from network through snap
        char get_nextCMD();     //get the command from the snap
        int get_Help();    //get the help command from snap
        int get_Merge();    //get the new time to merge

        void update_SNAP(uint32_t loc,uint8_t sta,uint32_t spe, int tic);

        inline TickType_t getLastActivityTime(void) const { return mLastActivityTime; }
        inline void resetActivity(void) { mLastActivityTime = xTaskGetMsCount(); }
        bool recentlyActive(unsigned int ms) const
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
//class SNAP
//{
//    public:
////        SNAP(uint8_t priority);   //for scheduler
////        bool run(void *p);                  ///< Before FreeRTOS
////        bool taskEntry(void);               ///< After FreeRTOS
////        bool run(void *p);                  ///< The main loop
//        SNAP();
//
////        addSharedObject();
//
//        bool send_Update(uint32_t location, uint8_t status, uint32_t speed);    //updates the locals on the snap
//        bool send_Estimated_Time_to_Merge(uint32_t speed, int ticks);  //sends the estimated time to merge to the SNAP
//        bool send_Merge(uint32_t location,uint8_t status,uint32_t speed, int ticks);                    //sends a merge command to SNAP
//        bool send_Help(uint8_t status, uint32_t location );                               //sends help to SNAP
//        bool send_Test();            //tests that the snap is working
//
//        int  get_Dest();    //TODO:get new destination from network
//        bool get_Update(uint32_t* dest,uint32_t* weight); //get an update for the graph
//        void get_Time();     //get the system time from network through snap
//        char get_CMD();     //get the command from the snap
//        int get_Help();    //get the help command from snap
//        int get_Merge();    //get the new time to merge
//
////        void setTime(); //TODO sets the time of the RTC
//        inline TickType_t getLastActivityTime(void) const { return mLastActivityTime; }
//        inline void resetActivity(void) { mLastActivityTime = xTaskGetMsCount(); }
//        bool recentlyActive(unsigned int ms) const
//        {
//            TickType_t lastTimeStampMs = MS_PER_TICK() * mLastActivityTime;
//            TickType_t currentTimeMs = xTaskGetMsCount();
//            return (currentTimeMs - lastTimeStampMs) < ms;
//        }
//
//    private:
////          rtc_t time;   // TODO time last updated
//           uint8_t status;
//           uint32_t speed;
//           uint32_t ticks;
//           uint32_t location;
//           TickType_t mLastActivityTime;   ///< updated each time an update is sent
//};

#endif /* SNAP_H_ */
