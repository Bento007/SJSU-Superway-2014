/*
 * pod.hpp
 *
 *  Created on: Feb 22, 2014
 *      Author: Trent Smith
 *      The Pod class creates an object to represent a pod.
 *      Pods are are stored in a linked list connecting to
 *      other pods through the next pointer.
 */

#include <stdint.h>

#ifndef POD_HPP_
#define POD_HPP_

class POD
{
    public:
        /**
         * The constructor for the POD class
         * @param Address  The unique 3 byte hex address of the pod
         * @param speed    The current speed of the pod
         * @param loc      The last location of the pod
         * @param status   The current status of the pod
         * @param time     The time between last update
         */
        POD(uint8_t *Address, uint8_t speed,uint8_t loc, uint8_t status, uint8_t *time);
        /**
         * Used to update the pods info
         * @param speed    The current speed of the pod
         * @param loc      The last location of the pod
         * @param status   The current status of the pod
         * @param time     The time between last update
         */
        void update(uint8_t speed,uint8_t loc, uint8_t status, uint8_t *time);

        //Getters
        POD*    getNext(){return next;}
        uint8_t getSpeed(){return speed;}
        uint8_t getLoc(){return loc;}
        uint8_t getStat(){return status;}
        uint8_t* getAddr(){return addr;}
        uint8_t* getTime(){return time;}

        //Setter
        void setNext(POD* input){ next = input; }
        void setSpeed(uint8_t input){ speed = input; }
        void setLoc(uint8_t input){ loc = input; }
        void setStat(uint8_t input){ status = input; }
        void setAddr(uint8_t* input){ addr[0] = input[0]; addr[1] = input[1]; addr[2] = input[2]; }
        void setTime(uint8_t* input){ time[0] = input[0]; time[1] = input[1]; time[2] = input[2]; }

    private:
        uint8_t addr[3];     ///< The address of the pod
        uint8_t speed;          ///< The last speed
        uint8_t loc;            ///< The last location
        uint8_t status;         ///< The current status of pod
        uint8_t  time[3];       ///< The time between last update Hour, min, sec
        POD *next;              ///< Pointer to the next pod
};



#endif /* POD_HPP_ */
