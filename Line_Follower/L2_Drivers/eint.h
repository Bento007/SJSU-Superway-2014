//////////////////////// eint.h///////////////////////

#ifndef EINT_H__
#define EINT_H__
#ifdef __cplusplus
extern "C" {
	#endif
	#include <stdint.h>
	#include "lpc_sys.h"



	/// The type of the interrupt for the port pin.
	typedef enum {
		eint_rising_edge, ///< Interrupt on rising edge
		eint_falling_edge ///< Interrupt on falling edge
	} eint_intr_t;

	/**
	* Enables the callback when the interrupt occurs. The entry added last is checked
	* first if multiple interrupts occur at the same time. Each call will allocate
	* 16 bytes for the interrupt service entry.
	* @note EINT3 shares interrupt with Port0 and Port2
	* @param pin_num The pin number from 0-31.
	* @param type The type of interrupt.
	* @param func The callback function.
	*/
	void eint3_enable_port0(uint8_t pin_num, eint_intr_t type, void_func_t func);

	/// @copydoc eint3_enable_port0()
	void eint3_enable_port2(uint8_t pin_num, eint_intr_t type, void_func_t func);
	void EINT3_IRQHandler(void);
	void callback(void (*ptr)());
	//void function0();
	//void function2();

	#ifdef __cplusplus
}
#endif
#endif /* EINT_H__ */
