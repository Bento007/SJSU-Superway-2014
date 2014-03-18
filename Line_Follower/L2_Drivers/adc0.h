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
 * @ingroup Drivers
 *
 * 12/02/2013 : Enclosed adc conversion inside critical section
 * 11/01/2013 : Fix possible divide by zero.  i was set to 0 during loop init
 */
#ifndef ADC0_H_
#define ADC0_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "LPC17xx.h"
#include "sys_config.h"
#include "FreeRTOS.h"



/**
 * Initializes the ADC Peripheral
 * @note The PIN that will be used by ADC needs to be selected using PINSEL externally
 */
static inline void adc0_init()
{
    // Clear PDN bit before powering up ADC0
    lpc_pconp(pconp_adc, true);
    lpc_pclk(pclk_adc, clkdiv_8);
    LPC_ADC->ADCR = (1 << 21);      // Enable ADC

    // Calculate and set prescalar for ADC.  13Mhz is maximum value
    const uint32_t adcClock = (sys_get_cpu_clock() / 8);
    const uint32_t maxAdcClock = (13 * 1000UL * 1000UL);
    for(int i=2; i < 255; i+=2) {
        if( (adcClock / i) < maxAdcClock) {
            LPC_ADC->ADCR |= (i << 8);
            break;
        }
    }
}

/**
 * Gets an ADC reading from a channel number between 0 - 7
 * @returns 12-bit ADC value read from the ADC.
 */
static inline unsigned short adc0_get_reading(uint8_t channel_num)
{
    if(channel_num >= 8) {
        return 0;
    }

    uint16_t result = 0;
    const uint32_t start_conversion = (1 << 24);
    const uint32_t conversion_done = (1 << 31);

    vPortEnterCritical();
    {
        // Clear previously selected channel
        LPC_ADC->ADCR &= ~((0xFF) | (0x7 << 24));

        // Set the channel number and start the conversion now
        LPC_ADC->ADCR |= (1 << channel_num);
        LPC_ADC->ADCR |= start_conversion;

        // Wait for conversion to complete
        while(! (LPC_ADC->ADGDR & conversion_done));

        // Pick up the result from bits 15:4
        result = LPC_ADC->ADGDR & 0xFFFF;
        result >>= 4;
    }
    vPortExitCritical();

    return result;
}



#ifdef __cplusplus
}
#endif
#endif /* ADC0_H_ */
