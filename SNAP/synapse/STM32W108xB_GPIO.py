# Copyright (C) 2011 Synapse Wireless, Inc.
# Subject to your agreement of the disclaimer set forth below, permission is given by Synapse Wireless, Inc. ("Synapse") to you to freely modify, redistribute or include this SNAPpy code in any program. The purpose of this code is to help you understand and learn about SNAPpy by code examples.
# BY USING ALL OR ANY PORTION OF THIS SNAPPY CODE, YOU ACCEPT AND AGREE TO THE BELOW DISCLAIMER. If you do not accept or agree to the below disclaimer, then you may not use, modify, or distribute this SNAPpy code.
# THE CODE IS PROVIDED UNDER THIS LICENSE ON AN "AS IS" BASIS, WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, WITHOUT LIMITATION, WARRANTIES THAT THE COVERED CODE IS FREE OF DEFECTS, MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE OR NON-INFRINGING. THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE COVERED CODE IS WITH YOU. SHOULD ANY COVERED CODE PROVE DEFECTIVE IN ANY RESPECT, YOU (NOT THE INITIAL DEVELOPER OR ANY OTHER CONTRIBUTOR) ASSUME THE COST OF ANY NECESSARY SERVICING, REPAIR OR CORRECTION. UNDER NO CIRCUMSTANCES WILL SYNAPSE BE LIABLE TO YOU, OR ANY OTHER PERSON OR ENTITY, FOR ANY LOSS OF USE, REVENUE OR PROFIT, LOST OR DAMAGED DATA, OR OTHER COMMERCIAL OR ECONOMIC LOSS OR FOR ANY DAMAGES WHATSOEVER RELATED TO YOUR USE OR RELIANCE UPON THE SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES OR IF SUCH DAMAGES ARE FORESEEABLE. THIS DISCLAIMER OF WARRANTY AND LIABILITY CONSTITUTES AN ESSENTIAL PART OF THIS LICENSE. NO USE OF ANY COVERED CODE IS AUTHORIZED HEREUNDER EXCEPT UNDER THIS DISCLAIMER.

#
# STM32W108xB_GPIO.py - some helper definitions and routines for working with the
# peripherals built into the ST Microelectronics STM32W108 chip.
#

IOBASE = 0x4000 # All the peripheral registers are at 0x4000_wxyz

# All of the following are offets from the base address of 0x4000_0000

# Low and High Config registers for Port A
GPIO_PACFGL = 0xB000
GPIO_PACFGH = 0xB004

# Low and High Config registers for Port B
GPIO_PBCFGL = 0xB400
GPIO_PBCFGH = 0xB404

# Low and High Config registers for Port C
GPIO_PCCFGL = 0xB800
GPIO_PCCFGH = 0xB804

# Data OUTput registers for Ports A-C
GPIO_PAOUT = 0xB00C
GPIO_PBOUT = 0xB40C
GPIO_PCOUT = 0xB80C

# The CLR registers let you atomically manipulate the Data OUTput register
# Writing a 1 to a bit in the CLR register actually RESETS the corresponding
# bit in the corresponding data OUT register.
GPIO_PACLR = 0xB014
GPIO_PBCLR = 0xB414
GPIO_PCCLR = 0xB814

# The SET registers let you atomically manipulate the Data OUTput register
# Writing a 1 to a bit in the SET register SETS the corresponding bit in the
# corresponding data OUT register.
GPIO_PASET = 0xB010
GPIO_PBSET = 0xB410
GPIO_PCSET = 0xB810

#
# Valid pin modes for this chip are:
# 0x0 = analog
# 0x1 = Output (push-pull)
# 0x2 = undefined
# 0x3 = undefined
# 0x4 = input (floating)
# 0x5 = output (open-drain)
# 0x6 = undefined
# 0x7 = undefined
# 0x8 = input (pulled-up or down)
# 0x9 = Alternate Output (push-pull)
# 0xA = undefined
# 0xB = Alternate Output (push-pull) SPI SCLK specific
# 0xC = undefined
# 0xD = Alternate Output (open-drain)
# 0xE = undefined
# 0xF = undefined
#
def setPinMode(io, mode):
    """Set the specified io pin (0-23) to the specified mode """
    if io == 0:
        GPIO_PxCFG = GPIO_PACFGL
        shift = 0
    elif io == 1:
        GPIO_PxCFG = GPIO_PACFGL
        shift = 4
    elif io == 2:
        GPIO_PxCFG = GPIO_PACFGL
        shift = 8
    elif io == 3:
        GPIO_PxCFG = GPIO_PACFGL
        shift = 12
    elif io == 4:
        GPIO_PxCFG = GPIO_PACFGH
        shift = 0
    elif io == 5:
        GPIO_PxCFG = GPIO_PACFGH
        shift = 4
    elif io == 6:
        GPIO_PxCFG = GPIO_PACFGH
        shift = 8
    elif io == 7:
        GPIO_PxCFG = GPIO_PACFGH
        shift = 12
    elif io == 8:
        GPIO_PxCFG = GPIO_PBCFGL
        shift = 0
    elif io == 9:
        GPIO_PxCFG = GPIO_PBCFGL
        shift = 4
    elif io == 10:
        GPIO_PxCFG = GPIO_PBCFGL
        shift = 8
    elif io == 11:
        GPIO_PxCFG = GPIO_PBCFGL
        shift = 12
    elif io == 12:
        GPIO_PxCFG = GPIO_PBCFGH
        shift = 0
    elif io == 13:
        GPIO_PxCFG = GPIO_PBCFGH
        shift = 4
    elif io == 14:
        GPIO_PxCFG = GPIO_PBCFGH
        shift = 8
    elif io == 15:
        GPIO_PxCFG = GPIO_PBCFGH
        shift = 12
    elif io == 16:
        GPIO_PxCFG = GPIO_PCCFGL
        shift = 0
    elif io == 17:
        GPIO_PxCFG = GPIO_PCCFGL
        shift = 4
    elif io == 18:
        GPIO_PxCFG = GPIO_PCCFGL
        shift = 8
    elif io == 19:
        GPIO_PxCFG = GPIO_PCCFGL
        shift = 12
    elif io == 20:
        GPIO_PxCFG = GPIO_PCCFGH
        shift = 0
    elif io == 21:
        GPIO_PxCFG = GPIO_PCCFGH
        shift = 4
    elif io == 22:
        GPIO_PxCFG = GPIO_PCCFGH
        shift = 8
    elif io == 23:
        GPIO_PxCFG = GPIO_PCCFGH
        shift = 12
    else:
        return

    GPIO_PxCFG_value = peek(IOBASE, GPIO_PxCFG, 1)
    GPIO_PxCFG_value &= ~(0x0F << shift)
    GPIO_PxCFG_value |= (mode << shift)
    poke(IOBASE, GPIO_PxCFG, 2, 0x0000, GPIO_PxCFG_value)

def dumpGPIO():
    value = peek(IOBASE, GPIO_PACFGH, 1)
    print "GPIO_PACFGH=",
    printHexWord(value)
    value = peek(IOBASE, GPIO_PACFGL, 1)
    print " GPIO_PACFGL=",
    printHexWord(value)
    print
    value = peek(IOBASE, GPIO_PBCFGH, 1)
    print " GPIO_PBCFGH=",
    printHexWord(value)
    value = peek(IOBASE, GPIO_PBCFGL, 1)
    print " GPIO_PBCFGL=",
    printHexWord(value)
    print
    value = peek(IOBASE, GPIO_PCCFGH, 1)
    print " GPIO_PCCFGH=",
    printHexWord(value)
    value = peek(IOBASE, GPIO_PCCFGL, 1)
    print " GPIO_PCCFGL=",
    printHexWord(value)
    print
    value = peek(IOBASE, GPIO_PAOUT, 1)
    print "GPIO_PAOUT=",
    printHexWord(value)
    value = peek(IOBASE, GPIO_PBOUT, 1)
    print " GPIO_PBOUT=",
    printHexWord(value)
    value = peek(IOBASE, GPIO_PCOUT, 1)
    print " GPIO_PCOUT=",
    printHexWord(value)
    print
