# Copyright (C) 2011 Synapse Wireless, Inc.
# Subject to your agreement of the disclaimer set forth below, permission is given by Synapse Wireless, Inc. ("Synapse") to you to freely modify, redistribute or include this SNAPpy code in any program. The purpose of this code is to help you understand and learn about SNAPpy by code examples.
# BY USING ALL OR ANY PORTION OF THIS SNAPPY CODE, YOU ACCEPT AND AGREE TO THE BELOW DISCLAIMER. If you do not accept or agree to the below disclaimer, then you may not use, modify, or distribute this SNAPpy code.
# THE CODE IS PROVIDED UNDER THIS LICENSE ON AN "AS IS" BASIS, WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, WITHOUT LIMITATION, WARRANTIES THAT THE COVERED CODE IS FREE OF DEFECTS, MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE OR NON-INFRINGING. THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE COVERED CODE IS WITH YOU. SHOULD ANY COVERED CODE PROVE DEFECTIVE IN ANY RESPECT, YOU (NOT THE INITIAL DEVELOPER OR ANY OTHER CONTRIBUTOR) ASSUME THE COST OF ANY NECESSARY SERVICING, REPAIR OR CORRECTION. UNDER NO CIRCUMSTANCES WILL SYNAPSE BE LIABLE TO YOU, OR ANY OTHER PERSON OR ENTITY, FOR ANY LOSS OF USE, REVENUE OR PROFIT, LOST OR DAMAGED DATA, OR OTHER COMMERCIAL OR ECONOMIC LOSS OR FOR ANY DAMAGES WHATSOEVER RELATED TO YOUR USE OR RELIANCE UPON THE SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES OR IF SUCH DAMAGES ARE FORESEEABLE. THIS DISCLAIMER OF WARRANTY AND LIABILITY CONSTITUTES AN ESSENTIAL PART OF THIS LICENSE. NO USE OF ANY COVERED CODE IS AUTHORIZED HEREUNDER EXCEPT UNDER THIS DISCLAIMER.

"""
CAT24C128.py - Example routines for the Catalyst Semiconductor 128 Kb Serial EEPROM

This chip is arranged as 16384 words of 8 bits each. It has three address lines,
allowing up to eight of them to share a single I2C bus.

Note that the focus of this script was to demonstrate the new I2C capabilities
of SNAP 2.1, not to exercise every feature of the EEPROM chip. Please refer to
the CAT24C128 data sheet to see what else it is capable of.

NOTE! - This script requires SNAP 2.1 or greater! It uses the I2C support first
added in SNAP 2.1, and will not work in a SNAP 2.0 node.
"""


from hexSupport import *


# 3 physical address pins on the chip allow 8 devices per I2C bus (0-7)
EEPROM_ADDRESS = 0x50 # slave address is (0)1010xxx

retries = 3


def buildPrefix(chipAddress, isRead):
    """internal helper routine"""
    slaveAddress = EEPROM_ADDRESS + chipAddress
    slaveAddress <<= 1
    if isRead:
        slaveAddress |= 1 # read

    cmd = ""
    cmd += chr( slaveAddress )

    return cmd

def readEEPROM(chipAddress, memoryAddress, dataLen):
    if (chipAddress < 0) or (chipAddress > 7):
        print "bad chip address - must be 0-7"
        return ""

    if dataLen > 63: # SNAPpy limitation, not I2C or EEPROM limitation
        print "too much data being read at once"
        return ""

    if memoryAddress >= 0x4000:
        print "invalid memory address - chip only holds 16K"
        return ""

    cmd = buildPrefix(chipAddress, False) # we have to WRITE the address we want to read

    # Add desired memory address onto the prefix string just built
    cmd += chr( memoryAddress >> 8 )
    cmd += chr( memoryAddress & 0xff)

    #dumpHex(cmd)

    result = i2cWrite(cmd, retries, False)

    cmd = buildPrefix(chipAddress, True) # NOW we can read data back

    result = i2cRead(cmd, dataLen, retries, False)
    return result

# Gotchas:
# 1) Can only write within a page in a single command
# 2) Can only write a page at a time
# 3) SNAPpy dynamic string limitations limit this further
def writeEEblock(chipAddress, memoryAddress, data):
    if (chipAddress < 0) or (chipAddress > 7):
        print "bad chip address - must be 0-7"
        return

    if len(data) > 61:
        print "too much data being written at once" # SNAPpy limitation
        return

    if memoryAddress >= 0x4000:
        print "invalid memory address - chip only holds 16K"
        return

    cmd = buildPrefix(chipAddress, False) # NOT a read

    # Add desired memory address onto the prefix string just built
    cmd += chr( memoryAddress >> 8 )
    cmd += chr( memoryAddress & 0xff)

    # Add on the actual data bytes
    cmd += data

    dumpHex(cmd)

    i2cWrite(cmd, retries, False)
