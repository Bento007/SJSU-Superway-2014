# Copyright (C) 2011 Synapse Wireless, Inc.
# Subject to your agreement of the disclaimer set forth below, permission is given by Synapse Wireless, Inc. ("Synapse") to you to freely modify, redistribute or include this SNAPpy code in any program. The purpose of this code is to help you understand and learn about SNAPpy by code examples.
# BY USING ALL OR ANY PORTION OF THIS SNAPPY CODE, YOU ACCEPT AND AGREE TO THE BELOW DISCLAIMER. If you do not accept or agree to the below disclaimer, then you may not use, modify, or distribute this SNAPpy code.
# THE CODE IS PROVIDED UNDER THIS LICENSE ON AN "AS IS" BASIS, WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, WITHOUT LIMITATION, WARRANTIES THAT THE COVERED CODE IS FREE OF DEFECTS, MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE OR NON-INFRINGING. THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE COVERED CODE IS WITH YOU. SHOULD ANY COVERED CODE PROVE DEFECTIVE IN ANY RESPECT, YOU (NOT THE INITIAL DEVELOPER OR ANY OTHER CONTRIBUTOR) ASSUME THE COST OF ANY NECESSARY SERVICING, REPAIR OR CORRECTION. UNDER NO CIRCUMSTANCES WILL SYNAPSE BE LIABLE TO YOU, OR ANY OTHER PERSON OR ENTITY, FOR ANY LOSS OF USE, REVENUE OR PROFIT, LOST OR DAMAGED DATA, OR OTHER COMMERCIAL OR ECONOMIC LOSS OR FOR ANY DAMAGES WHATSOEVER RELATED TO YOUR USE OR RELIANCE UPON THE SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES OR IF SUCH DAMAGES ARE FORESEEABLE. THIS DISCLAIMER OF WARRANTY AND LIABILITY CONSTITUTES AN ESSENTIAL PART OF THIS LICENSE. NO USE OF ANY COVERED CODE IS AUTHORIZED HEREUNDER EXCEPT UNDER THIS DISCLAIMER.

"""
LTC2412.py - Example routines for the LTC2412 2-Channel Differential Input
24-Bit No Latency Delta Sigma ADC by Linear Technology.

Note that the focus of this script was to demonstrate the new SPI capabilities
of SNAP 2.1, not to exercise every feature of the LTC2412 chip. Please refer
to the LTC2412 data sheet to see what else it is capable of.

NOTE! - This script requires SNAP 2.1 or greater! It uses the SPI support
first added in SNAP 2.1, and will not work in a SNAP 2.0 node.
"""

from hexSupport import *
from platforms import *

LTC2412_CS = GPIO_11
LTC2412_BUSY = GPIO_12

# Max times to "spin" waiting for "conversion complete"
LTC2412_TIMEOUT = 500

def readLTC2412():
    """read a 32-bit result word from the LTC2412"""
    writePin(LTC2412_CS, False)
    result = spiRead(4) # 4 bytes, 32 bits
    writePin(LTC2412_CS, True)
    return result

# This chip does not let you CHOOSE which ADC channel to readADC
# It always alternates channels, and so this routine reads the
# chip until the desired channel gets reported
def readADC(whichChannel):
    tries = 5
    timeout = 0
    while tries > 0:
        writePin(LTC2412_CS, False)
        timeout = 0
        while timeout < LTC2412_TIMEOUT:
            if readPin(LTC2412_BUSY):
                timeout += 1
                if timeout == LTC2412_TIMEOUT:
                    print "ADC timeout!"
                    return "\xFF\xFF\xFF\xFF"
            else:
                #print timeout
                timeout = LTC2412_TIMEOUT

        rawStr = readLTC2412()
        #dumpHex(rawStr)

        temp = ord( rawStr[0] )
        # Check for EOC* - End Of Conversion, active low
        if (temp & 0x80) == 0x00:
            # Data is good, which channel was it?
            if (temp & 0x40) == 0x40:
                # It was channel 1 - is that the one we wanted?
                if whichChannel == 1:
                    break # return rawStr
            else:
                # It was channel 0 - is that the one we wanted?
                if whichChannel == 0:
                    break # return rawStr
        tries -= 1
    if tries > 0:
        return rawStr
    else:
        print "readADC failed"

def printADC(rawStr):
    """Decode the 32-bit report from the chip, and print it"""
    temp = ord( rawStr[0] )
    # Check for EOC* - End Of Conversion, active low
    if (temp & 0x80) == 0x80:
        print "not ready yet"
        return

    # Which channel was it?
    if (temp & 0x40) == 0x40:
        print "1: ",
    else:
        print "0: ",

    # Get sign, range check
    if (temp & 0x20) == 0x20:
        print "+",
        if (temp & 0x10) == 0x10:
            print "overrange"
            return
    else:
        print "-",
        if (temp & 0x10) != 0x10:
            print "underrange"
            return

    # the bits are aligned horribly!
    valueHigh = temp & 0x1f
    valueHigh <<= 3

    temp = ord( rawStr[1] )
    valueMid = temp & 0x1f
    valueMid <<= 3
    temp >>= 5
    valueHigh = valueHigh | temp

    temp = ord( rawStr[2] )
    valueLow = temp & 0x1f
    valueLow <<= 3
    temp >>= 5
    valueMid = valueMid | temp

    temp = ord( rawStr[3] )
    # bottom 5 bits are useless
    temp >>= 5
    valueLow = valueLow | temp

    printHex(valueHigh)
    printHex(valueMid)
    printHex(valueLow)
    print

def testADC():
    # Read chip (channel values reported will alternate)
    rawStr = readLTC2412()
    dumpHex(rawStr)
    printADC(rawStr)

def testADC0():
    """Read ADC channel 0, print result"""
    rawStr = readADC(0)
    printADC(rawStr)

def testADC1():
    """Read ADC channel 1, print result"""
    rawStr = readADC(1)
    printADC(rawStr)

def pTestADC0():
    """Read ADC channel 0, send to Portal for plotting"""
    rawStr = readADC(0)
    rpc("\x00\x00\x0d", "reportAdc", 0, rawStr)

def pTestADC1():
    """Read ADC channel 1, send to Portal for plotting"""
    rawStr = readADC(1)
    rpc("\x00\x00\x0d", "reportAdc", 1, rawStr)

def pTest():
    """Read both ADC channels, send to Portal for plotting"""
    rawStr = readADC(0)
    rpc("\x00\x00\x0d", "reportAdc", 0, rawStr)
    rawStr = readADC(1)
    rpc("\x00\x00\x0d", "reportAdc", 1, rawStr)

def initLTC2412():
    """Init both the SPI interface and the LTC2412"""
    spiInit(False, False, True, False) # cpol low, cpha low, MsbFirst, NOT 4-wire (IOW, 3 wire)
