# Copyright (C) 2011 Synapse Wireless, Inc.
# Subject to your agreement of the disclaimer set forth below, permission is given by Synapse Wireless, Inc. ("Synapse") to you to freely modify, redistribute or include this SNAPpy code in any program. The purpose of this code is to help you understand and learn about SNAPpy by code examples.
# BY USING ALL OR ANY PORTION OF THIS SNAPPY CODE, YOU ACCEPT AND AGREE TO THE BELOW DISCLAIMER. If you do not accept or agree to the below disclaimer, then you may not use, modify, or distribute this SNAPpy code.
# THE CODE IS PROVIDED UNDER THIS LICENSE ON AN "AS IS" BASIS, WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, WITHOUT LIMITATION, WARRANTIES THAT THE COVERED CODE IS FREE OF DEFECTS, MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE OR NON-INFRINGING. THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE COVERED CODE IS WITH YOU. SHOULD ANY COVERED CODE PROVE DEFECTIVE IN ANY RESPECT, YOU (NOT THE INITIAL DEVELOPER OR ANY OTHER CONTRIBUTOR) ASSUME THE COST OF ANY NECESSARY SERVICING, REPAIR OR CORRECTION. UNDER NO CIRCUMSTANCES WILL SYNAPSE BE LIABLE TO YOU, OR ANY OTHER PERSON OR ENTITY, FOR ANY LOSS OF USE, REVENUE OR PROFIT, LOST OR DAMAGED DATA, OR OTHER COMMERCIAL OR ECONOMIC LOSS OR FOR ANY DAMAGES WHATSOEVER RELATED TO YOUR USE OR RELIANCE UPON THE SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES OR IF SUCH DAMAGES ARE FORESEEABLE. THIS DISCLAIMER OF WARRANTY AND LIABILITY CONSTITUTES AN ESSENTIAL PART OF THIS LICENSE. NO USE OF ANY COVERED CODE IS AUTHORIZED HEREUNDER EXCEPT UNDER THIS DISCLAIMER.

"""
MC13224_PWM.py - an example script demonstrating control of the three
available timer (TMR) modules on the MC13224 chip. (There is a fourth TMR
module inside the chip, but it is unavailable for PWM generation because
it is used to provide the 1 millisecond "tick" for the rest of SNAP to use).

This script uses what the MC13224 datasheets refer to as "variable-frequency PWM".
Other modes are possible, refer to the datasheets.

In variable-frequency PWM, the hardware controls "both parts" of a stream of pulses.

In this script these are referred to as the firstPart and the secondPart.

They are not referred to as "low" and "high" because the actual polarity is selectable.
The first part is low and the second part is high ***unless you set invert to True***.

The clock source for the PWM pulse train is 24 MHZ, divided by 1, 2, 4, 8, 16, 32, 64, or 128.
In this script, dividerCodes of 0-7 map to clock divisors of 1-128 (0 = /1, 1=/2, etc.)

This gives the following possible clock periods (approximate):

24 MHZ / 1 = 0.0416 microseconds
24 MHZ / 2 = 0.083 microseconds
24 MHZ / 4 = 0.166 microseconds
24 MHZ / 8 = 0.333 microseconds
24 MHZ / 16 = 0.666 microseconds
24 MHZ / 32 = 1.333 microseconds
24 MHZ / 64 = 2.666 microseconds
24 MHZ / 128 = 5.333 microseconds

Since the underlying counters used by the example are 16-bit, these clock rates
result in maximum "half pulse" durations of 65536 * clock_period.

(A counter value of n results in n+1 clocks being counted).
"""

# The upper 16 bits of the base address of ALL of the MC13224 I/O registers.
# The remaining address definitions will only be specifying the lower 16-bits.
# This lower 16 bits of the total 32-bit address will usually be referred to
# as an "offset" or as xxx_LO in constant definitions.
IOREG_BASE_HI = 0x8000

# The IO pins default to GPIO control. You have to explicitly "hand them over"
# to peripheral control. You use the various "FUNC" registers to do this
GPIO_FUNC_BASE_LO = 0x0018

# The base of all the TMR registers is 0x8000_7000
TMR_BASE_LO = 0x7000

# There are multiple sets of TMR registers, each set 0x20 bytes apart
TMR_COMP1 = 0x00
TMR_COMP2 = 0x02
TMR_CAPT = 0x04
TMR_LOAD = 0x06
TMR_HOLD = 0x08
TMR_CNTR = 0x0A
TMR_CTRL = 0x0C
TMR_SCTRL = 0x0E
TMR_CMPLD1 = 0x10
TMR_CMPLD2 = 0x12
TMR_CSCTRL = 0x14
TMR_ENBL = 0x1E

#
# All MC13224 IO pins default to GPIO, but there are 4 registers that let you select
# one of 4 different settings (some redundant) on a per-pin basis.
#
def makePin(io, func_code):
    """set io (0-63) to func_code (0-3)"""
    if io < 0:
        return
    if io > 63:
        return
    if func_code < 0:
        return
    if func_code > 3:
        return
    offset = (io / 16) # Which of the four registers is it (16 pins per register)
    offset *= 4 # The registers are 32 bits wide, and so are 4 bytes apart
    offset += GPIO_FUNC_BASE_LO
    offsetWithinRegister = io % 16

    hi = peek(IOREG_BASE_HI, offset, 2)
    lo = peek()
    #lo = peek(IOREG_BASE_HI, offset, 4)

    if offsetWithinRegister < 8:
        shift = offsetWithinRegister * 2
        lo &= ~(0x0003 << shift)
        lo |= (func_code << shift)
    else:
        shift = (offsetWithinRegister-8) * 2
        hi &= ~(0x0003 << shift)
        hi |= (func_code << shift)

    poke(IOREG_BASE_HI, offset, 2, hi, lo)

def makeTMRpin(tmr_channel):
    """Reprogram the IO pin for tmr_channel (0-2) instead of GPIO usage"""
    if tmr_channel < 0:
        return
    if tmr_channel > 2: # There is a Timer 3 but it is not available for script use
        return
    io = 8 + tmr_channel # TMR 0-3 are on IO 8-11
    makePin(io, 1) # For these pins, func_code 1 means "TMR" (instead of GPIO)

def writeTMR(tmr_channel, register, value):
    """write tmr_channel (0-2)'s register with value"""
    if tmr_channel < 0:
        return
    if tmr_channel > 2: # There *IS* a Timer 3 but it is not available for script use
        return
    offset = tmr_channel * 0x20
    offset += TMR_BASE_LO
    offset += register
    poke(IOREG_BASE_HI, offset, 1, value) # All the TMR registers are 16-bit

#
# divisorCode 0-7 corresponds to /1, /2, /4, /8 .. /128
# The peripheral clock is 12 MHZ (the 24 MHZ XTAL divided by 2)
# Pulse is usually low for firstPart clocks, then high for secondPart clocks.
# Set invert True to flip this
#
def initPWM(tmr_channel, divisorCode, firstPart, secondPart, invert):
    """Begin PWM on tmr_channel (0-2) with divisorCode (0-7) and 16-bit up and down counters"""
    divisorCode += 8
    makeTMRpin(tmr_channel)
    writeTMR(tmr_channel, TMR_CTRL, 0x0000)
    writeTMR(tmr_channel, TMR_LOAD, 0x0000)
    writeTMR(tmr_channel, TMR_CNTR, 0x0000)
    if invert:
        sctrl = 0x0007
    else:
        sctrl = 0x0005
    writeTMR(tmr_channel, TMR_SCTRL, sctrl)
    writeTMR(tmr_channel, TMR_CSCTRL, 0x0009)
    writeTMR(tmr_channel, TMR_COMP1, firstPart)
    writeTMR(tmr_channel, TMR_CMPLD1, firstPart)
    writeTMR(tmr_channel, TMR_COMP2, secondPart)
    writeTMR(tmr_channel, TMR_CMPLD2, secondPart)
    ctrl = 0x2024 | (divisorCode << 9)
    writeTMR(tmr_channel, TMR_CTRL, ctrl)

def setFirst(tmr_channel, newFirstPart):
    """Call this to vary the width of the first part of the pulse (0-0xffff)"""
    writeTMR(tmr_channel, TMR_CMPLD1, newFirstPart)

def setSecond(tmr_channel, newSecondPart):
    """Call this to vary the width of the second part of the pulse (0-0xffff)"""
    writeTMR(tmr_channel, TMR_CMPLD2, newSecondPart)

def setBoth(tmr_channel, newFirstPart, newSecondPart):
    """Call this to vary the width of both parts independantly (each part 0-0xffff)"""
    writeTMR(tmr_channel, TMR_CMPLD1, newFirstPart)
    writeTMR(tmr_channel, TMR_CMPLD2, newSecondPart)

def setSplit(tmr_channel, total, newSecondPart):
    """Call this to vary the width of both parts while maintaining a total width"""
    writeTMR(tmr_channel, TMR_CMPLD1, total-newSecondPart)
    writeTMR(tmr_channel, TMR_CMPLD2, newSecondPart)
