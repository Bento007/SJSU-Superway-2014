# Copyright (C) 2011 Synapse Wireless, Inc.
# Subject to your agreement of the disclaimer set forth below, permission is given by Synapse Wireless, Inc. ("Synapse") to you to freely modify, redistribute or include this SNAPpy code in any program. The purpose of this code is to help you understand and learn about SNAPpy by code examples.
# BY USING ALL OR ANY PORTION OF THIS SNAPPY CODE, YOU ACCEPT AND AGREE TO THE BELOW DISCLAIMER. If you do not accept or agree to the below disclaimer, then you may not use, modify, or distribute this SNAPpy code.
# THE CODE IS PROVIDED UNDER THIS LICENSE ON AN "AS IS" BASIS, WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, WITHOUT LIMITATION, WARRANTIES THAT THE COVERED CODE IS FREE OF DEFECTS, MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE OR NON-INFRINGING. THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE COVERED CODE IS WITH YOU. SHOULD ANY COVERED CODE PROVE DEFECTIVE IN ANY RESPECT, YOU (NOT THE INITIAL DEVELOPER OR ANY OTHER CONTRIBUTOR) ASSUME THE COST OF ANY NECESSARY SERVICING, REPAIR OR CORRECTION. UNDER NO CIRCUMSTANCES WILL SYNAPSE BE LIABLE TO YOU, OR ANY OTHER PERSON OR ENTITY, FOR ANY LOSS OF USE, REVENUE OR PROFIT, LOST OR DAMAGED DATA, OR OTHER COMMERCIAL OR ECONOMIC LOSS OR FOR ANY DAMAGES WHATSOEVER RELATED TO YOUR USE OR RELIANCE UPON THE SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES OR IF SUCH DAMAGES ARE FORESEEABLE. THIS DISCLAIMER OF WARRANTY AND LIABILITY CONSTITUTES AN ESSENTIAL PART OF THIS LICENSE. NO USE OF ANY COVERED CODE IS AUTHORIZED HEREUNDER EXCEPT UNDER THIS DISCLAIMER.

"""
LIS302DL.py - Example routines for the STMicroelectronics LIS302DL I2C MEMS.

Note that the focus of this script was to demonstrate the I2C capabilities
of SNAP, not to exercise every feature of the MEMS chip. Please refer to
the LIS302DL data sheet to see what else it is capable of.
"""

from hexSupport import *

MEMS_ADDRESS = 0x38 # slave address on MB851 evaluation board

retries = 3

def buildMemsCmd(registerAddress, isRead):
    """internal helper routine"""
    slaveAddress = MEMS_ADDRESS
    if isRead:
        slaveAddress |= 1 # read is LSBit == 1, write is LSBit == 0

    cmd = ""
    cmd += chr( slaveAddress )
    cmd += chr( registerAddress )

    return cmd

def readMems(firstReg, numRegs):
    """read a string of registers from the MEMS"""
    cmd = buildMemsCmd(0x80 | firstReg, False)
    i2cWrite(cmd, retries, False)

    cmd = chr( MEMS_ADDRESS | 1 )
    result = i2cRead(cmd, numRegs, retries, False)

    return result

def readMemsReg(register):
    """read a single numeric register from the MEMS"""
    cmd = buildMemsCmd(register, False)
    i2cWrite(cmd, retries, False)

    cmd = chr( MEMS_ADDRESS | 1 )
    resultStr = i2cRead(cmd, 1, retries, False)
    result = ord( resultStr[0] )

    return result

def writeMemsReg(register, value):
    """write a numeric register value to the MEMS"""
    cmd = buildMemsCmd(register, False)
    cmd += chr(value)
    i2cWrite(cmd, retries, False)

def convertU8toS8(value):
    """Convert raw unsigned byte from the chip to a signed value"""
    if value > 127:
        value = -(256 - value)
    return value

def askXYZstatus():
    """A remote node can call this function to ask for readings"""
    registers = readMems(0x27, 7)
    x = ord(registers[2])
    x = convertU8toS8(x)
    y = ord(registers[4])
    y = convertU8toS8(y)
    z = ord(registers[6])
    z = convertU8toS8(z)
    status = ord(registers[0])
    rpc(rpcSourceAddr(), 'tellXYZstatus', x, y, z, status)

def printXYZ():
    registers = readMems(0x27, 7)
    x = ord(registers[2])
    x = convertU8toS8(x)
    y = ord(registers[4])
    y = convertU8toS8(y)
    z = ord(registers[6])
    z = convertU8toS8(z)
    print "Status=",
    printHex(ord(registers[0]))
    print " X=",
    print x,
    print " Y=",
    print y,
    print " Z=",
    print z

def initMEMS():
    """Init just the LIS302DL (asssumes i2cInit() already done)"""
    # FF_WU_CFG_1 (Free Fall/Wake Up Config #1)
    writeMemsReg(0x30, 0x00) # <- disabled
    # CLICK_CFG
    writeMemsReg(0x38, 0x00) # <- disabled
    # CTRL_REG2 (Control Register #2)
    writeMemsReg(0x21, 0x01) # filter setting, refer to datasheet
    # CTRL_REG1 (Control Register #1)
    # NOT powered down, Z/Y/X enabled
    writeMemsReg(0x20, 0x47)

def queryMems():
    """See if the MEMS is present"""
    result = readMemsReg(0x0F)
    printHex(result)
    print
    if result == 0x3B:
        print "LIS302DL detected"
