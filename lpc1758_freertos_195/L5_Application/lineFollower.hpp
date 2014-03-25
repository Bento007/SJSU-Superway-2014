/*
 * lineFollower.hpp
 *
 *  Created on: Mar 21, 2014
 *      Author: Eriberto
 */

#ifndef LINEFOLLOWER_HPP_
#define LINEFOLLOWER_HPP_

#include "utilities.h"
#include "eint.h"
#include "lpc_pwm.hpp"
#include "io.hpp"

//save space by #define constants, let the compiler handle it. -Trent S.
#define leftspeed 6
#define rightspeed 7
#define lleft  20
#define left  22;
#define middle  23;
#define right  28;
#define rright  29;

    int go = 50;        /// PWM cannot go above 100
//    int stop = 0;
    void (*leftptr)();
    void (*rightptr)();

    PWM leftmotor(PWM::pwm1, 50);       // pwm1 = P2.0 = left       /////////// ERROR HERE ////////////////
    PWM rightmotor(PWM::pwm2, 50);      // pwm2 = P2.1 = right      //////////// ERROR HERE /////////////////

    bool initLineFollower(void)
    {
        LPC_SC->PCLKSEL0 &= ~(3<<12);
        LPC_SC->PCLKSEL0 |= (1<<13);                    // set CLK/1
        LPC_GPIO2->FIODIR &= ~(1<<leftspeed);           // speed input pin 2.6
        LPC_GPIO2->FIODIR &= ~(1<<rightspeed);          // speed input pin 2.7

        // sensor pins as input
        LPC_GPIO1->FIODIR &= ~(1 << lleft);             // lleft sensor input
        LPC_GPIO1->FIODIR &= ~(1 << left);              // left sensor input
        LPC_GPIO1->FIODIR &= ~(1 << middle);            // middle sensor input
        LPC_GPIO1->FIODIR &= ~(1 << right);             // right sensor input
        LPC_GPIO1->FIODIR &= ~(1 << rright);            // rright sensor input

        LPC_GPIO0->FIODIR |= (1<<0);                    // output to LCD

        //leftptr = left;
        //rightptr =right;

        eint3_enable_port2( leftspeed, eint_rising_edge , *leftptr);
        eint3_enable_port2( rightspeed, eint_rising_edge , *rightptr);

//        add shared object of queue/semaphore
    }



#endif /* LINEFOLLOWER_HPP_ */
