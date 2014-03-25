/*
 * lineFollower.hpp
 *
 *  Created on: Mar 21, 2014
 *      Author: Eriberto
 *
 *      EDIT: Mar 25, 2014
 *      Author:Trent S.
 *      -#defined constants
 *      -Removed globals
 *      -Fix leftMotor & rightMotor in constructor
 *      -added private members int leftSpeed and rightSpeed
 *      -Replaced  delay_ms(1000) with  vTaskDelay(1000)
 *      -Changed bool run(void*p) return value to true
 *      -Added timers
 */

#include "scheduler_task.hpp"
#include "utilities.h"
#include "eint.h"
#include "lpc_pwm.hpp"
#include "io.hpp"

#ifndef LINEFOLLOWER_HPP_
#define LINEFOLLOWER_HPP_

//save space by #define constants, let the compiler handle it. -Trent S.
#define leftspeed 6
#define rightspeed 7
#define lleft 20
#define left 22
#define middle 23
#define right 28
#define rright 29
#define go 20
#define stop 0
#define notchDistance 12   // 12mm

class sensorMotorTask : public scheduler_task
{
    public:

        sensorMotorTask(uint8_t priority) :scheduler_task("lineFollower", 1024*4, priority)
        {
            /*initialize members*/
            leftMotorPtr = new PWM(PWM::pwm1,100);//TODO: see if leftMotor works
            rightMotorPtr= new PWM(PWM::pwm1,100); //TODO: see if rightMotorPtr works

            LTargetSpe=go;
            RTargetSpe=go;
            LActualSpe = 0; ///< Calculated speed for left wheel
            RActualSpe = 0; ///< Calculated speed for right wheel

            lastLeftTimer = 0;
            lastRightTimer = 0;

            deltaleft = 0;
            deltaright = 0;
        }

        bool init(void)
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

    //        eint3_enable_port2( leftspeed, eint_rising_edge , &calcLeftSpe);//TODO: fix interrupts by adding definition
    //        eint3_enable_port2( rightspeed, eint_rising_edge , &calcRightSpe);

            // EP: add shared object of queue/semaphore
            return true;
        }


        bool run(void *p)
        {
            leftMotorPtr->set(go);       // todo: not working, come back and fix
            rightMotorPtr->set(go);

            /// FOLLOWING LINE ///
    //      if(!(LPC_GPIO2->FIOPIN & (1 << speedpin)))

    //      if ((LPC_GPIO1->FIOPIN & (1 << left)) && (LPC_GPIO1->FIOPIN & (1 << right))){
    //          leftmotor.set(go);
    //          rightmotor.set(go);
    //          printf("go straight");
    //      }
    //      else if (!(LPC_GPIO1->FIOPIN & (1 << left)) && !(LPC_GPIO1->FIOPIN & (1 << right))){
    //          leftmotor.set(stop);
    //          rightmotor.set(stop);
    //          printf("motor stop\n");
    //      }
    //      else if(!(LPC_GPIO1->FIOPIN & (1 << left))){        // if left sensor hits line
    //          leftmotor.set(stop);
    //          rightmotor.set(go);
    //          printf("go right\n");
    //      }
    //      else if (!(LPC_GPIO1->FIOPIN & (1 << right))){
    //          leftmotor.set(go);
    //          rightmotor.set(stop);
    //          printf("go left\n");
    //      }
            vTaskDelay(1000);
            return true;
        }

        /*Calculates the actual speed of the wheels*/
    //    void calcLeftSpe()
    //    {
    //        int currentTime = sys_get_high_res_timer_us();
    //        deltaleft = currentTime - lastLeftTimer;
    ////        printf("deltaleft = %i  \n", deltaleft);
    //
    //        lastLeftTimer = sys_get_high_res_timer_us();
    //        return;
    //    }
    //    /*Calculates the actual speed of the wheels*/
    //    void calcRightSpe()
    //    {
    //        int currentTime = sys_get_high_res_timer_us();
    //        deltaright = currentTime - lastRightTimer;
    ////        printf("     deltaright = %i  \n", deltaright);
    //        lastRightTimer = sys_get_high_res_timer_us();
    //    }

    private:
        PWM *leftMotorPtr;      // testing ignore
        PWM *rightMotorPtr;

        int LTargetSpe;  ///< Target speed for left wheel
        int RTargetSpe;  ///< Target speed for right wheel

        int LActualSpe; ///< Calculated speed for left wheel
        int RActualSpe; ///< Calculated speed for right wheel

        int lastLeftTimer;
        int lastRightTimer;

        int deltaleft;
        int deltaright;
};


#endif /* LINEFOLLOWER_HPP_ */
