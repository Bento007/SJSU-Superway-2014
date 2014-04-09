/*
 * superwaySPI.hpp
 *
 *  Created on: Apr 8, 2014
 *      Author: Eriberto
 *
 *      This file contains code for using SSP1 on the SJSU One board.
 */

#ifndef SUPERWAYSPI_HPP_
#define SUPERWAYSPI_HPP_
#include "utilities.h"
#include "LPC17xx.h"

#define OneMHz 1

/***************************************************
 * ssp1_ExchangeByte:
 *
 * Takes one parameter, "out" is the data to be sent.
 *
 * Return:  It's not required to assign to anything.
 * However, if you want to received a Byte, just call
 * this function again, with a dummy value.
 *
 * Example:
 *
 * ssp1_ExchangeByte(byteToSend);
 *
 * byteToReceive = ssp1_ExchangeByte(dummyValue);
 *
 ***************************************************/

uint8_t ssp1_ExchangeByte(uint8_t out)
{
    LPC_SSP1->DR = out;
    while (LPC_SSP1->SR & (1 << 4));    //while SSP is idle, continue to wait.
    return LPC_SSP1->DR;
}

/***************************************************
 * ssp_set_max_clock:
 *
 * Takes a value for the clock to be set, in MHz.
 *
 * Description: Sets your SPI clock (SCK) to the
 * desired frequency. Must be within a tolerable
 * value for the other SPI devices.
 *
 * Only used at initialization.
 *
 ***************************************************/

void ssp_set_max_clock(unsigned int max_clock_mhz)
{
    //Differs from Preet's version in that this always uses SSP1.
    unsigned int divider = 2;
    const unsigned int cpuClockMhz = sys_get_cpu_clock() / (1000 * 1000UL);

    // Keep scaling down divider until calculated is higher
    // Example:
    // 60, need 18
    // 60/2   18 < 30 YES
    // 60/4   18 < 15 NO
    while(max_clock_mhz < (cpuClockMhz / divider) && divider <= 254)
    {
        divider += 2;
    }

    LPC_SSP1->CPSR = divider;
}

/***************************************************
 *  my_ssp1_init:
 *
 *  Description- Used to initialize SSP1 ports and
 *  registers. Sets data mode (CR0) and the operation
 *  mode (CR1). Also sets frequency for SCK.
 *
 ***************************************************/

void my_ssp1_init()
{
    LPC_SC->PCONP       |= (1 << 10);   //enable ssp1
    LPC_SC->PCLKSEL0    &=~(3 << 20);   //clear PCLK_SSP1
    LPC_SC->PCLKSEL0    |= (1 << 20);   //set CCLK/1 mode (01)

    LPC_PINCON->PINSEL0 &=~(3 << 12);   //reset pin for p0.6
    LPC_PINCON->PINSEL0 &=~(3 << 14);   //reset pin for SCK1
    LPC_PINCON->PINSEL0 &=~(3 << 16);   //reset pin for MISO1
    LPC_PINCON->PINSEL0 &=~(3 << 18);   //reset pin for MOSI1

    LPC_PINCON->PINSEL0 |= (0 << 12);   //set pin to
    LPC_PINCON->PINSEL0 |= (2 << 14);   //set pin to SCK1
    LPC_PINCON->PINSEL0 |= (2 << 16);   //set pin to MISO1
    LPC_PINCON->PINSEL0 |= (2 << 18);   //set pin to MOSI1

    LPC_SSP1->CR0       |= (7 << 0);    //8-bit mode
    LPC_SSP1->CR1       |= (1 << 1);    //set as Master

    ssp_set_max_clock(OneMHz);
}


#endif /* SUPERWAYSPI_HPP_ */
