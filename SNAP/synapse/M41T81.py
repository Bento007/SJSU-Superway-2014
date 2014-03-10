# Copyright (C) 2011 Synapse Wireless, Inc.
# Subject to your agreement of the disclaimer set forth below, permission is given by Synapse Wireless, Inc. ("Synapse") to you to freely modify, redistribute or include this SNAPpy code in any program. The purpose of this code is to help you understand and learn about SNAPpy by code examples.
# BY USING ALL OR ANY PORTION OF THIS SNAPPY CODE, YOU ACCEPT AND AGREE TO THE BELOW DISCLAIMER. If you do not accept or agree to the below disclaimer, then you may not use, modify, or distribute this SNAPpy code.
# THE CODE IS PROVIDED UNDER THIS LICENSE ON AN "AS IS" BASIS, WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, WITHOUT LIMITATION, WARRANTIES THAT THE COVERED CODE IS FREE OF DEFECTS, MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE OR NON-INFRINGING. THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE COVERED CODE IS WITH YOU. SHOULD ANY COVERED CODE PROVE DEFECTIVE IN ANY RESPECT, YOU (NOT THE INITIAL DEVELOPER OR ANY OTHER CONTRIBUTOR) ASSUME THE COST OF ANY NECESSARY SERVICING, REPAIR OR CORRECTION. UNDER NO CIRCUMSTANCES WILL SYNAPSE BE LIABLE TO YOU, OR ANY OTHER PERSON OR ENTITY, FOR ANY LOSS OF USE, REVENUE OR PROFIT, LOST OR DAMAGED DATA, OR OTHER COMMERCIAL OR ECONOMIC LOSS OR FOR ANY DAMAGES WHATSOEVER RELATED TO YOUR USE OR RELIANCE UPON THE SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES OR IF SUCH DAMAGES ARE FORESEEABLE. THIS DISCLAIMER OF WARRANTY AND LIABILITY CONSTITUTES AN ESSENTIAL PART OF THIS LICENSE. NO USE OF ANY COVERED CODE IS AUTHORIZED HEREUNDER EXCEPT UNDER THIS DISCLAIMER.

"""
M41T81.py - Example routines for the STMicroelectronics M41T81 Serial Access
Real-Time Clock with Alarms.

Note that the focus of this script was to demonstrate the new I2C capabilities
of SNAP 2.1, not to exercise every feature of the RTC chip. Please refer to
the M41T81 data sheet to see what else it is capable of.

NOTE! - This script requires SNAP 2.1 or greater! It uses the I2C support first
added in SNAP 2.1, and will not work in a SNAP 2.0 node.
"""


from hexSupport import *


RTC_ADDRESS = 0xD0 # slave address is (0)1101000 which shifts to 1101000(R/W)

retries = 3


def buildRtcCmd(registerAddress, isRead):
    """internal helper routine"""
    slaveAddress = RTC_ADDRESS
    if isRead:
        slaveAddress |= 1 # read

    cmd = ""
    cmd += chr( slaveAddress )
    cmd += chr( registerAddress )

    return cmd

def readClock(firstReg, numRegs):
    """read a string of registers from the RTC"""
    cmd = buildRtcCmd(firstReg, False)
    i2cWrite(cmd, retries, False)

    cmd = chr( RTC_ADDRESS | 1 )
    result = i2cRead(cmd, numRegs, retries, False)

    #dumpHex(result)
    return result

def readClockReg(register):
    """read a single numeric register from the RTC"""
    cmd = buildRtcCmd(register, False)
    i2cWrite(cmd, retries, False)

    cmd = chr( RTC_ADDRESS | 1 )
    resultStr = i2cRead(cmd, 1, retries, False)
    result = ord( resultStr[0] )
    #printHex(result)
    return result

def writeClockReg(register, value):
    """write a numeric register value to the RTC"""
    cmd = buildRtcCmd(register, False)
    cmd += chr(value)
    i2cWrite(cmd, retries, False)

def printTime():
    """read RTC chip and display time as MM/DD/YYYY HH:MM:SS.hh"""
    registers = readClock(0, 8)

    printHex(ord(registers[6])) # MM
    print "/",
    printHex(ord(registers[5])) # DD
    print "/",

    # YYYY
    print '2',
    if (ord(registers[3]) & 0x20) == 0x20:
        print '1',
    else:
        print '0',
    printHex(ord(registers[7]))

    print ' ',

    printHex(ord(registers[3]) & 0x3F) # HH
    print ":",
    printHex(ord(registers[2])) # MM
    print ":",
    printHex(ord(registers[1]) & 0x7F) # SS
    print ".",
    printHex(ord(registers[0])) # hh
    print

def setDateDecimal(year, month, date):
    """set year (00-99), month, date using decimal instead of BCD"""
    setDate(decimalToBcd(year), decimalToBcd(month), decimalToBcd(date))

def setDate(year, month, date):
    """set year (0x00-0x99), month, date - each must be in hex-BCD!"""
    writeClockReg(7, year)
    writeClockReg(6, month)
    writeClockReg(5, date)
    readClockReg(8) # allow clock updates

def setTimeDecimal(hour, minute, second):
    """set hour, minute, second using decimal instead of BCD"""
    setTime(decimalToBcd(hour), decimalToBcd(minute), decimalToBcd(second))

def setTime(hour, minute, second):
    """set hour, minute, second - each must be in hex-BCD!"""
    writeClockReg(3, hour)
    writeClockReg(2, minute)
    writeClockReg(1, second)
    writeClockReg(0, 0)
    readClockReg(8) # allow clock updates

def initRTC():
    """Init just the M41T81 (asssumes i2cInit() already done)"""
    # See if the oscillator is STOPPED. If so, un-stop it plus zero out the chip
    if (readClockReg(1) & 0x80) == 0x80:
        reg = 0
        while reg < 20:
            writeClockReg(reg, 0)
            reg += 1
    # Ensure the clock is not halted
    reg12 = readClockReg(12)
    reg12 &= ~0x40
    writeClockReg(12, reg12)
    # Note we do not automatically setDate(), setTime()!

def decimalToBcd(decimal):
    if decimal < 0 or decimal > 99:
        return 0
    else:
        return 16 * (decimal / 10) + (decimal % 10)