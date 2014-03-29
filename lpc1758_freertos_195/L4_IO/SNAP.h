/*
 * SNAP.h
 *
 *  Created on: Mar 28, 2014
 *      Author: Bento007
 *  The snap devices uses uart3 to send and receive data
 */

#ifndef SNAP_H_
#define SNAP_H_

#include <stdint.h>
#include <time.h>
#include "rtc.h"

class SNAP
{
    public:
        SNAP();//need to initialize RTC
        bool sendUpdate(int location, int status, int speed);    //updates the locals on the snap
        bool sendTicks(int ticks);
        bool sendETM(int speed, int ticks);
        bool sendMerge(int speed, int ticks);
        bool sendHelp(int status);

        int  reciDest();    //TODO:tell the pod it's new destination
        void reciTime();     //get the system time from network through snap

        bool getData(int size); //gets data[size] from the snap
        char getCMD(char*);     //get the command from the snap
        bool test();            //tests that the snap is working
//        void setTime(); //TODO sets the time of the RTC
    private:
          rtc_t time;   // TODO time last updated
};

#endif /* SNAP_H_ */
