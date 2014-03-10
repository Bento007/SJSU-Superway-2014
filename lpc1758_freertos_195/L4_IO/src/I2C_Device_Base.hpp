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
 */
 
#ifndef I2C_DEVICE_BASE_HPP_
#define I2C_DEVICE_BASE_HPP_

#include "I2C2.hpp"



/**
 * I2C Device Base Class
 * This class can be inherited by an I2C Device to be able to read and write registers
 * more easily as this class will puts an abstraction layer on I2C and provides simple
 * functionality to read and write registers over I2C Bus.
 *
 * @ingroup BoardIO
 *
 */



class I2C_Device_Base
{
protected:
    /// Constructor of this base class that takes addr as a parameter
    I2C_Device_Base(unsigned char addr) : mI2C(I2C2::getInstance()), mOurAddr(addr)
    {
    }

    /// @returns the register content of this device
    inline unsigned char readReg(unsigned char reg)
    {
        return mI2C.readReg(mOurAddr, reg);
    }

    /// Writes a register of this device
    inline void writeReg(unsigned char reg, unsigned char data)
    {
        mI2C.writeReg(mOurAddr, reg, data);
    }

    /// @returns true if the device responds to its address
    inline bool checkDeviceResponse()
    {
        return mI2C.checkDeviceResponse(mOurAddr);
    }

    /**
     * Reads 16-bit register from reg and reg+1 granted that reg has MSB
     */
    uint16_t get16BitRegister(unsigned char reg)
    {
        char buff[2] = {0};
        mI2C.readRegisters(mOurAddr, reg, &buff[0], sizeof(buff));

        const uint16_t MSB = buff[0];
        const uint16_t LSB = buff[1];
        return ((MSB << 8) | (LSB & 0xFF));
    }

private:
    I2C_Base& mI2C; /// Instance of I2C Bus used for communication
    const unsigned char mOurAddr; ///< I2C Address of this device
};

#endif /* I2C_DEVICE_BASE_HPP_ */
