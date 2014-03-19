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
 * @brief  Provides the following system services :
 *              - RIT (repetitive interrupt)
 *              - Memory info of the system.
 *              - Configure function to use for printf/scanf
 *              - Get boot type and time
 *              - Watchdog
 *
 * 12082013 : Added functionality to see next heap pointer from mem_info_t
 */
#ifndef LPC_SYS_H__
#define LPC_SYS_H__
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include <stdbool.h>

#include "LPC17xx.h"
#include "rtc.h"
#include "sys_config.h"



/**
 * Enumeration of the reboot type
 */
typedef enum {
    boot_unknown = 0,
    boot_power_on,          ///< Cold boot (power on)
    boot_reset,             ///< Boot after reset condition
    boot_watchdog,          ///< Boot after watchdog reset (intentional)
    boot_watchdog_recover,  ///< Boot after watchdog reset after an error (or crash)
    boot_brown_out          ///< Boot after under-voltage
} sys_boot_t;

/**
 * This is the memory structure that is returned from getMemoryInfo()
 * Heap memory obtains pool of memory from System, so the memory pool
 * not obtained by the Heap is listed by the systemAvailable variable.
 */
typedef struct
{
    uint32_t used_global; ///< Global Memory allocated
    uint32_t used_heap;   ///< Memory granted by Heap (malloc, new etc.)
    uint32_t avail_heap;  ///< Memory available at Heap
    uint32_t avail_sys;   ///< Memory available to Heap (from sbrk function)
    void*    next_malloc_ptr; ///< The next pointer that will be returned to malloc() from system
} sys_mem_t;

/** Void function pointer */
typedef void (*void_func_t)(void);

/** Function pointer of a function returning a char and taking a char as parameter */
typedef char (*char_func_t)(char);



/** @{ Defined at system_init.c */
sys_boot_t sys_get_boot_type();  ///< @returns the reboot reason as detected during system startup
rtc_t sys_get_boot_time();         ///< @returns the boot-time recorded in the system
/** @} */



/** @{ Defined at syscalls.c */
/**
 * Sets the function used to output a char by printf() or stdio output functions
 * @param func  The function pointer to use to output a char
 */
void sys_set_outchar_func(char_func_t func);

/**
 * Sets the function used to input a char by scanf() or stdio input functions
 * @param func  The function pointer to use to get a char
 */
void sys_set_inchar_func(char_func_t func);
/** @} */



/**
 * Setup Periodic RIT callback.
 * @param function  Your function to callback.  You should not use FreeRTOS API in this callback.
 * @param time_ms    The frequency of callback in milliseconds.
 */
void sys_rit_setup(void_func_t function, unsigned int time_ms);
void sys_rit_disable(void); ///< Disable RIT timer and interrupt
bool sys_rit_running(void); ///< @return true if RIT is running

/**
 * Gets the system up time in milliseconds.
 * This is defined at high_level_init.cpp
 */
uint64_t sys_get_uptime_ms(void);

/**
 * Gets the value of the high resolution timer.
 * This timer is shared with FreeRTOS CPU usage, and may reset by the terminal 'info' command.
 * This can eventually overflow after ~12 hours of runtime granted 10us per tick
 */
static inline uint64_t sys_get_high_res_timer_us(void)
{
	// TIMER0_US_PER_TICK = 10, Microseconds per tick for Timer0
	// TC = Timer Counter reg, incremented every PR+1 (Prescale Register) cycles
    //return ((uint64_t)TIMER0_US_PER_TICK * LPC_TIM0->TC);


	// TIMER0_MS_PER_TICK = 10000, Miliseconds per tick for Timer0
	 return (TIMER0_US_PER_TICK * LPC_TIM0->TC);

}



/**
 * Gets System Memory information
 * The information includes Global Memory usage, and dynamic memory usage.
 * This function is defined at memory.cpp
 * @returns MemoryInfoType structure
 */
sys_mem_t sys_get_mem_info();

/**
 * Prints memory information to the given buffer
 * The buffer needs to be at least 160 bytes
 */
void sys_get_mem_info_str(char buffer[160]);



/**
 * Watchdog feed resets the watchdog timeout.
 * Periodic feeds should keep the system running, if the  watchdog
 * feeds stop, system will reset after watchdog timeout.
 */
static inline void sys_watchdog_feed()
{
    LPC_WDT->WDFEED = 0xAA;
    LPC_WDT->WDFEED = 0x55;
}

/**
 * Resets the system immediately.
 */
static inline void sys_reboot()
{
    /**
     * From the datasheet:
     * After writing 0xAA to WDFEED, access to any Watchdog register other than writing
     * 0x55 to WDFEED causes an immediate reset/interrupt when the Watchdog is enabled.
     */
    LPC_WDT->WDFEED = 0xAA;
    LPC_WDT->WDMOD = 0;
}

/**
 * Enables watchdog with reset mode
 * Value of timeout is defined at this file.
 */
static inline void sys_watchdog_enable()
{
    /**
     * 1 / (4Mhz/4) = Watchdog clock by default (WCLK)
     * Therefore watchdog clock = 1Mhz = 1uS
     * Each value of WDTC means 1uS
     */
    LPC_WDT->WDTC = WATCHDOG_TIMEOUT_MS * 1000;

    /**
     * Enable Watchdog and Watchdog reset enable
     * Once enabled, this cannot be cleared
     */
    LPC_WDT->WDMOD = 3;
    sys_watchdog_feed();
}



#ifdef __cplusplus
}
#endif
#endif /* LPC_SYS_H__ */
