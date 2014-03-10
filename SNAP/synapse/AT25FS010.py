# Copyright (C) 2011 Synapse Wireless, Inc.
# Subject to your agreement of the disclaimer set forth below, permission is given by Synapse Wireless, Inc. ("Synapse") to you to freely modify, redistribute or include this SNAPpy code in any program. The purpose of this code is to help you understand and learn about SNAPpy by code examples.
# BY USING ALL OR ANY PORTION OF THIS SNAPPY CODE, YOU ACCEPT AND AGREE TO THE BELOW DISCLAIMER. If you do not accept or agree to the below disclaimer, then you may not use, modify, or distribute this SNAPpy code.
# THE CODE IS PROVIDED UNDER THIS LICENSE ON AN "AS IS" BASIS, WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, WITHOUT LIMITATION, WARRANTIES THAT THE COVERED CODE IS FREE OF DEFECTS, MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE OR NON-INFRINGING. THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE COVERED CODE IS WITH YOU. SHOULD ANY COVERED CODE PROVE DEFECTIVE IN ANY RESPECT, YOU (NOT THE INITIAL DEVELOPER OR ANY OTHER CONTRIBUTOR) ASSUME THE COST OF ANY NECESSARY SERVICING, REPAIR OR CORRECTION. UNDER NO CIRCUMSTANCES WILL SYNAPSE BE LIABLE TO YOU, OR ANY OTHER PERSON OR ENTITY, FOR ANY LOSS OF USE, REVENUE OR PROFIT, LOST OR DAMAGED DATA, OR OTHER COMMERCIAL OR ECONOMIC LOSS OR FOR ANY DAMAGES WHATSOEVER RELATED TO YOUR USE OR RELIANCE UPON THE SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES OR IF SUCH DAMAGES ARE FORESEEABLE. THIS DISCLAIMER OF WARRANTY AND LIABILITY CONSTITUTES AN ESSENTIAL PART OF THIS LICENSE. NO USE OF ANY COVERED CODE IS AUTHORIZED HEREUNDER EXCEPT UNDER THIS DISCLAIMER.

"""
AT25FS010.py - Example routines for the ATMEL AT25FS010
High Speed Small Sectored SPI Flash Memory (128K bytes)

Note that the focus of this script is to demonstrate the SPI capabilities
of the ZIC2410 port of SNAP, not to demonstrate every feature of the AT25FS010
chip. Please refer to the AT25FS010 data sheet to see what else it is capable of.
"""

from synapse.hexSupport import * # just used by the test routines

atCS = 23 # default to the actual SPI CS* pin on a ZIC2410

# AT25FS010 commands
# Note - where the chip supports two command values, I used the first one
# I will mention the alternate value in a comment, when there is one
AT_WREN = '\x06'
AT_WRDI = '\x04'
AT_RDSR = '\x05'
# I don't use the WRSR 0x01
AT_READ = '\x03'
# I don't use FAST_READ 0x0B
AT_PROGRAM = '\x02'
# I don't use the SECTOR_ERASE = 0x20 # altername 0xD7
# I don't use the BLOCK_ERASE = 0x52 # alternate 0xD8
AT_CHIP_ERASE = '\x60' # alternate 0xC7
AT_RDID = '\x9F' # alternate 0xAB

def initAT25FS010(chipSelect):
    """set up to work with the AT25FS010 chip on the specified chip select pin"""
    global atCS
    atCS = chipSelect # remember the correct chip select pin
    writePin(atCS, True) # start out with chip select deasserted
    setPinDir(atCS, True) # make the chip select pin an output
    spiInit(False, False, True, True) # SPI mode (0,0), MSbit first, 4-wire device (see data sheet)

def ATassertCS():
    """Chip Select 'ON' - used internally by this script"""
    writePin(atCS, False)
    
def ATdeassertCS():
    """Chip Select 'OFF' - used internally by this script"""
    writePin(atCS, True)

def ATwriteCmdForRead(cmd):
    """Write a command but leave things set for a subsequent ATread()"""
    ATassertCS()
    spiWrite(cmd)
    # notice we left the chip selected, we're not done yet
    
def ATwriteCmd(cmd):
    """Write a command and be done with it"""
    ATwriteCmdForRead(cmd)
    ATdeassertCS()
    
def ATread(byteCount):
    """Read the response after ATwriteCmdForRead() has done it's part"""
    # 4-wire SPI requires us to send bytes as we are receiving them
    # a) build up a dummy string to send
    dummy = ''
    count = byteCount
    while count > 0:
        dummy += '\xFF'
        count -= 1
    # b) send the dummy string and catch the reply
    resp = spiXfer(dummy)
    ATdeassertCS()
    return resp

def ATreadData(byteCount):
    """Stand-alone read routine (does CS manipulation too)"""
    ATassertCS()
    data = ATread(byteCount)
    return data

dummy16 = 16 * "\xFF" # pre-building the dummy string for speed
def ATread16():
    """Optimized version of ATread()"""
    resp = spiXfer(dummy16)
    ATdeassertCS()
    return resp

def ATreadID():
    """Read the three Chip ID bytes"""
    ATwriteCmdForRead(AT_RDID)
    idStr = ATread(3)
    return idStr

def ATreadSR():
    """Read the Status Register"""
    ATwriteCmdForRead(AT_RDSR)
    srStr = ATread(1)
    return srStr

def ATenableWrites():
    """Enable ONE WRITE to the chip (the chip auto-disables after just one write)"""
    ATwriteCmd(AT_WREN)

def ATdisableWrites():
    """Disable writes to the chip"""
    ATwriteCmd(AT_WRDI)

def ATreadFLASH(addrHigh, addrLow, byteCount):
    """Read some bytes from the FLASH. 'addrHigh' can be 0 or 1 on this 128K part"""
    cmd = AT_READ
    cmd += chr(addrHigh & 0xFF)
    cmd += chr(addrLow >> 8)
    cmd += chr(addrLow & 0xFF)
    ATwriteCmdForRead(cmd)
    dataStr = ATread(byteCount)
    return dataStr

def ATwriteFLASH(addrHigh, addrLow, byteStr):
    """Program some bytes into the FLASH. 'addrHigh' can be 0 or 1 on this 128K part"""
    ATenableWrites()
    cmd = AT_PROGRAM
    cmd += chr(addrHigh & 0xFF)
    cmd += chr(addrLow >> 8)
    cmd += chr(addrLow & 0xFF)
    cmd += byteStr
    ATwriteCmd(cmd)

def ATeraseFLASH():
    """Erase the entire FLASH"""
    ATenableWrites()
    ATwriteCmd(AT_CHIP_ERASE)

#
# Test code that could be removed from a production version
#

def testReadID():
    """Read AND DUMP the ID bytes"""
    dumpHex(ATreadID())

def testReadFLASH():
    """Read AND DUMP the first 16 bytes of FLASH"""
    dumpHex(ATreadFLASH(0, 0x0000, 16))

def testReadFLASH2(addr):
    """Read AND DUMP 16 bytes from the specified address of the first 64K of FLASH"""
    dumpHex(ATreadFLASH(0, addr, 16))

def testReadFLASH3(hiAddr, addr):
    """Read AND DUMP 16 bytes from anywhere within the FLASH"""
    dumpHex(ATreadFLASH(hiAddr, addr, 16))

def testReadSR():
    """Read, DUMP, and (partially) DECODE the Status Register"""
    statusStr = ATreadSR() 
    dumpHex(statusStr)
    if len(statusStr) > 1:
        print "Got more than one status byte (?)"
        return
    elif len(statusStr) != 1:
        print "Did not get a status byte (?)"
        return
    status = ord(statusStr[0])
    if status & 0x01:
        print "Write cycle in progress"
    else:
        print "Ready"
    if status & 0x02:
        print "Write Enabled"
    else:
        print "Not Write Enabled"
    # There are more status bits but this is just an example script
    print "External Write Protect Pin ",
    if status & 0x80:
        print "Enabled"
    else:
        print "Not Enabled"
