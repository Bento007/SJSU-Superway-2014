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

#include <stdio.h>              // setvbuf & printf

#include "sys_config.h"         // CPU Clock Configuration
#include "LPC17xx.h"            // LPC MemoryMap
#include "uart0_min.h"          // Uart0 init
#include "isr_priorities.h"
#include "lpc_sys.h"
#include "core_cm3.h"
#include "fault_registers.h"



extern "C" void syscalls_init(void);
extern void sys_clock_configure();

static void configure_flash_acceleration(unsigned int clock)
{
    const uint32_t clockMhz = clock / (1000 * 1000);
    const uint32_t const_val = 0x03A; /* Datasheet says : Must be 0x03A */

    // Flash accelerator parameters depends on CPU clock to optimize program code read (CPU instructions)
    switch(clockMhz)
    {
        case  0 ... 20  : LPC_SC->FLASHCFG = (0 << 12) | const_val; break;
        case 21 ... 40  : LPC_SC->FLASHCFG = (1 << 12) | const_val; break;
        case 41 ... 60  : LPC_SC->FLASHCFG = (2 << 12) | const_val; break;
        case 61 ... 80  : LPC_SC->FLASHCFG = (3 << 12) | const_val; break;
        case 81 ... 100 : LPC_SC->FLASHCFG = (4 << 12) | const_val; break;
        default:
            LPC_SC->FLASHCFG = (5 << 12) | const_val; /* works for all clock settings */
            break;
    }
}

static void configure_interrupt_priorities()
{
    /* ISRs indented left use altered priorities.
     * See isr_priorities.h
     */
    NVIC_SetPriority(WDT_IRQn,          IP_watchdog);
    NVIC_SetPriority(TIMER0_IRQn,       IP_timers);
    NVIC_SetPriority(TIMER1_IRQn,       IP_timers);
    NVIC_SetPriority(TIMER2_IRQn,       IP_timers);
    NVIC_SetPriority(TIMER3_IRQn,       IP_timers);
    NVIC_SetPriority(UART0_IRQn,    IP_uart);
    NVIC_SetPriority(UART1_IRQn,    IP_uart);
    NVIC_SetPriority(UART2_IRQn,    IP_uart);
    NVIC_SetPriority(UART3_IRQn,    IP_uart);
    NVIC_SetPriority(PWM1_IRQn,         IP_pwm1);
    NVIC_SetPriority(I2C0_IRQn,     IP_i2c);
    NVIC_SetPriority(I2C1_IRQn,     IP_i2c);
    NVIC_SetPriority(I2C2_IRQn,     IP_i2c);
    NVIC_SetPriority(SPI_IRQn,          IP_spi);
    NVIC_SetPriority(SSP0_IRQn,     IP_ssp);
    NVIC_SetPriority(SSP1_IRQn,     IP_ssp);
    NVIC_SetPriority(PLL0_IRQn,         IP_pll);
    NVIC_SetPriority(RTC_IRQn,          IP_rtc);
    NVIC_SetPriority(EINT0_IRQn,    IP_eint);
    NVIC_SetPriority(EINT1_IRQn,    IP_eint);
    NVIC_SetPriority(EINT2_IRQn,    IP_eint);
    NVIC_SetPriority(EINT3_IRQn,    IP_eint);
    NVIC_SetPriority(ADC_IRQn,          IP_adc);
    NVIC_SetPriority(BOD_IRQn,          IP_bod);
    NVIC_SetPriority(USB_IRQn,          IP_usb);
    NVIC_SetPriority(CAN_IRQn,      IP_can);
    NVIC_SetPriority(DMA_IRQn,          IP_dma);
    NVIC_SetPriority(I2S_IRQn,          IP_i2s);
    NVIC_SetPriority(ENET_IRQn,         IP_enet);
    NVIC_SetPriority(RIT_IRQn,      IP_RIT);
    NVIC_SetPriority(MCPWM_IRQn,        IP_mcpwm);
    NVIC_SetPriority(QEI_IRQn,          IP_qei);
    NVIC_SetPriority(PLL1_IRQn,         IP_pll1);
    NVIC_SetPriority(USBActivity_IRQn,  IP_usbact);
    NVIC_SetPriority(CANActivity_IRQn,  IP_canact);
}

static sys_boot_t g_sys_boot_type = boot_unknown; // Store reboot type for anyone to query
static rtc_t g_rtc_boot_time = {0};

sys_boot_t sys_get_boot_type()
{
    return g_sys_boot_type;
}
rtc_t sys_get_boot_time()
{
    return g_rtc_boot_time;
}

static void print_boot_info(void)
{
    if (LPC_SC->RSID & 1) {
        uart0_puts("System Power-on: Booting ...");
        g_sys_boot_type = boot_power_on;
        LPC_SC->RSID |= 1; // Clear POR bit
    }
    else if(LPC_SC->RSID & 2) {
        uart0_puts("System Reset: Booting ...");
        g_sys_boot_type = boot_reset;
        LPC_SC->RSID |= 2; // Clear RESET bit
    }
    else if(LPC_SC->RSID & 4) {
        if(FAULT_PRESENT_VAL == FAULT_EXISTS) {
            g_sys_boot_type = boot_watchdog_recover;
            uart0_puts("System Crash: Re-booting ...");
        }
        else {
            g_sys_boot_type = boot_watchdog;
            uart0_puts("Watchdog System Reset: Booting ...");
            LPC_SC->RSID |= 4; // Clear WDTR bit
        }
        FAULT_EXISTS = 0;
    }
    else {
        g_sys_boot_type = boot_unknown;
    }
}

/**
 * Initializes the minimal system including CPU Clock, UART, and Flash accelerator
 * Be careful of the order of operations!!!
 */
void low_level_init(void)
{
    rtc_init();
    g_rtc_boot_time = rtc_gettime();

    /**
     * Configure System Clock based on desired clock rate @ sys_config.h
     * Setup default interrupt priorities that will work with FreeRTOS
     */
    sys_clock_configure();
    configure_flash_acceleration(sys_get_cpu_clock());
    configure_interrupt_priorities();
    __enable_irq();

    // Setup UART with minimum I/O functions
    uart0_init(UART0_DEFAULT_RATE_BPS);
    sys_set_outchar_func(uart0_putchar);
    sys_set_inchar_func(uart0_getchar);

    /**
     * Turn off I/O buffering otherwise sometimes printf/scanf doesn't behave
     * correctly due to strange buffering and/or flushing effects.
     */
    setvbuf(stdout, 0, _IONBF, 0);
    setvbuf(stdin,  0, _IONBF, 0);

    // Initialize newlib fopen() fread() calls support
    syscalls_init();

    /**
     * Enable watchdog to allow us to recover in case of:
     *  - We attempt to run an application and it's not there
     *  - Application we ran crashes
     */
    sys_watchdog_enable();
    print_boot_info();
}

