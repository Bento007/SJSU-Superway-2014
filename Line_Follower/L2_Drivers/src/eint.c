////////////////////////////////////////////// eint.c/////////////////

#include <stdlib.h>
#include "eint.h"
#include <printf_lib.h>
#include "utilities.h"

/**
* Enables the callback when the interrupt occurs. The entry added last is checked
* first if multiple interrupts occur at the same time. Each call will allocate
* 16 bytes for the interrupt service entry.
* @note EINT3 shares interrupt with Port0 and Port2
* @param pin_num The pin number from 0-31.
* @param type The type of interrupt.
* @param func The callback function.
*/
void (*leftptr)();
void (*rightptr)();

int leftspeed = 6;
int rightspeed = 7;
int timer1;
int timer2;
int timer3;
int timer4;
int deltaleft;
int deltaright;

void callback(void (*ptr)()){ // callback function
	ptr();
}
void left()
{
	timer2 = sys_get_high_res_timer_us();
	deltaleft = timer2 - timer1;
	printf("deltaleft = %i  \n", deltaleft);
//	printf("%f", (notchDistance/delta));

	timer1 = sys_get_high_res_timer_us();

}
void right()
{
	timer3 = sys_get_high_res_timer_us();
	deltaright = timer3 - timer4;
	printf("     deltaright = %i  \n", deltaright);
//	printf("\n%f", (notchDistance/delta));
	timer4 = sys_get_high_res_timer_us();
}


void EINT3_IRQHandler(void)
{
	if(LPC_GPIOINT->IntStatus & (1<<2))
	{
		LPC_GPIOINT->IO2IntClr = ( 0x3FFF << 0 );		// clear all interrupts
		if ( LPC_GPIO2 -> FIOPIN & ( 1 << leftspeed))
			callback(leftptr);
		 if ( LPC_GPIO2 -> FIOPIN & ( 1 << rightspeed))
			callback(rightptr);
	}
}

void eint3_enable_port0(uint8_t pin_num, eint_intr_t type, void_func_t func)
{
	if ( pin_num == leftspeed)        // used global variable to assign the value
		leftptr = func;
	else if( pin_num == rightspeed)
		rightptr = func;

	//ptr0 = func; // point function pointer to function passed in
	//LPC_PINCON -> PINSEL0 &= ~( 1 << pin_num);
	LPC_GPIO0 -> FIODIR0 &= ~(1 << pin_num);
	if ( type == eint_rising_edge )
	{
		LPC_GPIOINT->IO0IntEnR |= ( 1 << pin_num);
	}
	if ( type == eint_falling_edge)
	{
		LPC_GPIOINT->IO0IntEnF |= ( 1 << pin_num);
	}
	NVIC_EnableIRQ(EINT3_IRQn);
}

/// @copydoc eint3_enable_port0()
void eint3_enable_port2(uint8_t pin_num, eint_intr_t type, void_func_t func)
{
	if ( pin_num == leftspeed)       // used global variable to assign the value
	{
		leftptr = func;
	}
	else if( pin_num == rightspeed){
		rightptr = func;
	}
	LPC_GPIO2 -> FIODIR &= ~(1 << pin_num); // set pin as input

	if ( type == eint_rising_edge )
	{
		LPC_GPIOINT->IO2IntEnR |= ( 1 << pin_num);
	}
	if ( type == eint_falling_edge)
	{
		LPC_GPIOINT->IO2IntEnF |= ( 1 << pin_num);
	}
	NVIC_EnableIRQ(EINT3_IRQn);
}
