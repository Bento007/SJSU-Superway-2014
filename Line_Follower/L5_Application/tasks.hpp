/*
 *     SocialLedge.com - Copyright (C) 2013
 *
 *     This file is part of free software framework for embedded processors.
 *     You can use it and/or distribute it as long as this copyright header
 *     remains unmodified.  The code is free for personal use and requires
 *     permission to use in a commercial product.
 *
 *      THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 *      OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 *      MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 *      I SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 *      CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *     You can reach the author of this software at :
 *          p r e e t . w i k i @ g m a i l . c o m
 */

/**
 * @file
 * @brief Contains FreeRTOS Tasks
 */
#ifndef TASKS_HPP_
#define TASKS_HPP_

#include "scheduler_task.hpp"
#include "soft_timer.hpp"
#include "command_handler.hpp"
#include "wireless.h"
#include "char_dev.hpp"
#include "lpc_pwm.hpp"
#include "eint.h"
#include "tasks.hpp"
#include "examples/examples.hpp"
#include "utilities.h"
#include <stdint.h>
#include <stdio.h>
#include <sys_config.h>
#include "io.hpp"
#include "lpc_sys.h"
#include "soft_timer.hpp"
#include <printf_lib.h>
#include "LED_Display.hpp"


/**
 * Terminal task is our UART0 terminal that handles our commands into the board.
 * This also saves and restores the "disk" telemetry.  Disk telemetry variables
 * are automatically saved and restored across power-cycles to help us preserve
 * any non-volatile information.
 */
class terminalTask : public scheduler_task
{
    public:
        terminalTask(uint8_t priority);     ///< Constructor
        bool regTlm(void);                  ///< Registers telemetry
        bool taskEntry(void);               ///< Registers commands.
        bool run(void *p);                  ///< The main loop

    private:
        // Command channels device and input command str
        typedef struct {
            CharDev *iodev; ///< The IO channel
            str *cmdstr;    ///< The command string
            bool echo;      ///< If input should be echo'd back
        } cmdChan_t;

        VECTOR<cmdChan_t> mCmdIface;   ///< Command interfaces
        CommandProcessor mCmdProc;     ///< Command processor
        uint16_t mCommandCount;        ///< terminal command count
        uint16_t mDiskTlmSize;         ///< Size of disk variables in bytes
        char *mpBinaryDiskTlm;         ///< Binary disk telemetry
        SoftTimer mCmdTimer;           ///< Command timer

        cmdChan_t getCommand(void);
        void addCommandChannel(CharDev *channel, bool echo);
        void handleEchoAndBackspace(cmdChan_t *io, char c);

        bool saveDiskTlm(void);
};

class sensorMotorTask : public scheduler_task, public PWM
{
public:

	void (*leftptr)();
	void (*rightptr)();
	PWM leftMotor, rightMotor;
    int go = 30;		/// PWM cannot go above 100
    int stop = 0;

	sensorMotorTask(uint8_t priority) :scheduler_task("lineFollower", 1024, priority), PWM(PWM::pwm1, 50),
			leftptr(leftinterrupt), rightptr(rightinterrupt),
			leftMotor(PWM::pwm1, 0), rightMotor(PWM::pwm2, 0)
	{
	/* Nothing to do */
	}

	bool init(void)
	{
		LPC_SC->PCLKSEL0 &= ~(3<<12);
		LPC_SC->PCLKSEL0 |= (1<<13);					// set CLK/1
		LPC_GPIO2->FIODIR &= ~(1<<leftspeed);			// speed input pin 2.6
		LPC_GPIO2->FIODIR &= ~(1<<rightspeed);			// speed input pin 2.7

		// sensor pins as input
		LPC_GPIO1->FIODIR &= ~(1 << lleft);				// lleft sensor input
		LPC_GPIO1->FIODIR &= ~(1 << left);				// left sensor input
		LPC_GPIO1->FIODIR &= ~(1 << middle);			// middle sensor input
		LPC_GPIO1->FIODIR &= ~(1 << right);				// right sensor input
		LPC_GPIO1->FIODIR &= ~(1 << rright);			// rright sensor input

		LPC_GPIO0->FIODIR |= (1<<0);					// output to LCD

		leftptr=leftinterrupt;
		rightptr=rightinterrupt;

		leftMotor.set(stop);
		rightMotor.set(stop);

		eint3_enable_port2( leftspeed, eint_rising_edge , leftptr);	// crashing here
		eint3_enable_port2( rightspeed, eint_rising_edge , rightptr);

		LE.setAll(0xF);

		// EP: add shared object of queue/semaphore
		return true;
	}


	bool run(void *p)
	{
		/// FOLLOWING LINE ///
    	if ((LPC_GPIO1->FIOPIN & (1 << left)) && (LPC_GPIO1->FIOPIN & (1 << right))){
    		leftMotor.set(go);
    		rightMotor.set(go);
    		//printf("go straight");
    	}
		else if (!(LPC_GPIO1->FIOPIN & (1 << left)) && !(LPC_GPIO1->FIOPIN & (1 << right))){
			leftMotor.set(stop);
			rightMotor.set(stop);
			//printf("motor stop\n");
		}
    	else if((LPC_GPIO1->FIOPIN & (1 << left))){		// if left sensor hits line
    		leftMotor.set(stop);
    		rightMotor.set(go);
			//printf("go right\n");
		}
		else if ((LPC_GPIO1->FIOPIN & (1 << right))){
			leftMotor.set(go);
			rightMotor.set(stop);
			//printf("go left\n");
		}

		return -1;
	}

private:
	// pin layout //
	int leftspeed=6;
	int rightspeed=7;
	int lleft = 20;
	int left = 22;
	int middle = 23;
	int right = 28;
	int rright = 29;


};

#endif /* TASKS_HPP_ */
