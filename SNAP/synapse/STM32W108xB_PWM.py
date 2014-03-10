# Copyright (C) 2011 Synapse Wireless, Inc.
# Subject to your agreement of the disclaimer set forth below, permission is given by Synapse Wireless, Inc. ("Synapse") to you to freely modify, redistribute or include this SNAPpy code in any program. The purpose of this code is to help you understand and learn about SNAPpy by code examples.
# BY USING ALL OR ANY PORTION OF THIS SNAPPY CODE, YOU ACCEPT AND AGREE TO THE BELOW DISCLAIMER. If you do not accept or agree to the below disclaimer, then you may not use, modify, or distribute this SNAPpy code.
# THE CODE IS PROVIDED UNDER THIS LICENSE ON AN "AS IS" BASIS, WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, WITHOUT LIMITATION, WARRANTIES THAT THE COVERED CODE IS FREE OF DEFECTS, MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE OR NON-INFRINGING. THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE COVERED CODE IS WITH YOU. SHOULD ANY COVERED CODE PROVE DEFECTIVE IN ANY RESPECT, YOU (NOT THE INITIAL DEVELOPER OR ANY OTHER CONTRIBUTOR) ASSUME THE COST OF ANY NECESSARY SERVICING, REPAIR OR CORRECTION. UNDER NO CIRCUMSTANCES WILL SYNAPSE BE LIABLE TO YOU, OR ANY OTHER PERSON OR ENTITY, FOR ANY LOSS OF USE, REVENUE OR PROFIT, LOST OR DAMAGED DATA, OR OTHER COMMERCIAL OR ECONOMIC LOSS OR FOR ANY DAMAGES WHATSOEVER RELATED TO YOUR USE OR RELIANCE UPON THE SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES OR IF SUCH DAMAGES ARE FORESEEABLE. THIS DISCLAIMER OF WARRANTY AND LIABILITY CONSTITUTES AN ESSENTIAL PART OF THIS LICENSE. NO USE OF ANY COVERED CODE IS AUTHORIZED HEREUNDER EXCEPT UNDER THIS DISCLAIMER.
#
# STM32W108xB_PWM.py - an example of using the 8 PWM channels (2 sets of 4)
# on this part. Note that the 4 timer channels within a set share a common frequency.
# Also note that the second set of 4 can be mapped onto two different pins each.
# (The output pins for the first set of 4 timers are fixed in hardware)
#

from synapse.hexSupport import *

from synapse.STM32W108xB_GPIO import *

# Note that these are specified as offsets from the 0x4000_0000 base, not as full addresses
TIM1_CR1 = 0xE000
TIM2_CR1 = 0xF000

TIM1_EGR = 0xE014
TIM2_EGR = 0xF014

TIM1_CCMR1 = 0xE018
TIM2_CCMR1 = 0xF018

TIM1_CCMR2 = 0xE01C
TIM2_CCMR2 = 0xF01C

TIM1_OR = 0xE050 # For PWM, 0 = PCLK 1 = 1KCLCK
TIM2_OR = 0xF050 # Like above but also adds 4 pin muxes

# These accept "n" of 0-15, the resulting divisor is 2**n (1 - 32768)
TIM1_PSC = 0xE028
TIM2_PSC = 0xF028

TIM1_CNT = 0xE024
TIM2_CNT = 0xF024

TIM1_ARR = 0xE02C
TIM2_ARR = 0xF02C

TIM1_CCER = 0xE020
TIM2_CCER = 0xF020

TIM1_CCR1 = 0xE034
TIM1_CCR2 = 0xE038
TIM1_CCR3 = 0xE03C
TIM1_CCR4 = 0xE040
TIM2_CCR1 = 0xF034
TIM2_CCR2 = 0xF038
TIM2_CCR3 = 0xF03C
TIM2_CCR4 = 0xF040

#
# The TIMer for each set of 4 PWM channels is shared. So, it makes sense to
# break the "shared" setup into a separate routine, so that you can NOT disturb
# PWMs that are already running.
#
def pwmInitTimer(io, prescalar, divisor):
    """Initialze the TIMx (TIM1 or TIM2) that goes with the specified io pin"""

    # Based on the io pin the user wants to do PWM on, determine if they are
    # needing to configure TIM1 or TIM2
    # (Yes, the mapping of IO pin to Timer and Channel really does jump around!)
    if (io == 14) or (io == 15) or (io == 6) or (io == 7):
        timer = 1
    elif (io == 0) or (io == 3) or (io == 1) or (io == 2):
        timer = 2
    elif (io == 9) or (io == 10) or (io == 11) or (io == 12):
        timer = 2
    else:
        return # There is no correct answer to the question...

    # Figure out what registers we need to manipulate so the code below can be common
    if timer == 1:
        TIMx_CR1 = TIM1_CR1
        TIMx_CNT = TIM1_CNT
        TIMx_PSC = TIM1_PSC
        TIMx_ARR = TIM1_ARR
        TIMx_EGR = TIM1_EGR
    else:
        TIMx_CR1 = TIM2_CR1
        TIMx_CNT = TIM2_CNT
        TIMx_PSC = TIM2_PSC
        TIMx_ARR = TIM2_ARR
        TIMx_EGR = TIM2_EGR
    
    # TIM_CEN = 0 to disable this timer
    TIMx_CR1_value = peek(IOBASE, TIMx_CR1, 1)
    TIMx_CR1_value &= ~(0x0001)
    # Notice these ALL have to be full 32-bit wide writes!
    poke(IOBASE, TIMx_CR1, 2, 0x0000, TIMx_CR1_value)

    # Other bits to 0 just to get to a known starting point
    TIMx_CR1_value = 0x0000

    # TIM_ARBE - Set Auto-Reload Buffer Enable
    TIMx_CR1_value |= 0x80

    # TIM_CMS - Center-aligned Mode Selection -> 00 to choose Edge-aligned mode
    # TIM_DIR - Direction -> 0 to choose up-counter
    # TIM_OPM - One Pulse Mode -> 0 to NOT do it only once
    # TIM_URS - Update Request Source -> 0 (this is a guess)
    # TIM_UDIS - Update Disable -> 0 (think this is a "don't care")

    poke(IOBASE, TIMx_CR1, 2, 0x0000, TIMx_CR1_value)

    # Zero out the counter
    poke(IOBASE, TIMx_CNT, 2, 0x0000, 0x0000)

    # set prescalar
    poke(IOBASE, TIMx_PSC, 2, 0x0000, prescalar)

    # set divisor to indirectly set frequency
    poke(IOBASE, TIMx_ARR, 2, 0x0000, divisor)

    # Force all these settings to be read into the actual registers
    poke(IOBASE, TIMx_EGR, 2, 0x0000, 0x0001)

    # TIM_CEN - Counter Enable -> 1
    TIMx_CR1_value |= 0x01
    poke(IOBASE, TIMx_CR1, 2, 0x0000, TIMx_CR1_value)

def pwmInitChannel(io, dutyCycle, invert):
    if io == 14: # TIM1 C1
        TIMx_CCER_OFFSET = TIM1_CCER # 1-4 on TIM1, 5-8 on TIM2
        TIMx_CCER_shift = 0
        TIMx_CCMR_OFFSET = TIM1_CCMR1
        TIMx_CCMR_shift = 0
    elif io == 15: # TIM1 C2
        TIMx_CCER_OFFSET = TIM1_CCER # 1-4 on TIM1, 5-8 on TIM2
        TIMx_CCER_shift = 4
        TIMx_CCMR_OFFSET = TIM1_CCMR1
        TIMx_CCMR_shift = 8
    elif io == 6: # TIM1 C3
        TIMx_CCER_OFFSET = TIM1_CCER # 1-4 on TIM1, 5-8 on TIM2
        TIMx_CCER_shift = 8
        TIMx_CCMR_OFFSET = TIM1_CCMR2
        TIMx_CCMR_shift = 0
    elif io == 7: # TIM1 C4
        TIMx_CCER_OFFSET = TIM1_CCER # 1-4 on TIM1, 5-8 on TIM2
        TIMx_CCER_shift = 12
        TIMx_CCMR_OFFSET = TIM1_CCMR2
        TIMx_CCMR_shift = 8
    elif (io == 0) or (io == 9): # TIM2 C1 can come out on two different pins
        TIMx_CCER_OFFSET = TIM2_CCER # 1-4 on TIM1, 5-8 on TIM2
        TIMx_CCER_shift = 0
        TIMx_CCMR_OFFSET = TIM2_CCMR1
        TIMx_CCMR_shift = 0
        alternate_output_mask = 0x10
    elif (io == 3) or (io == 10): # TIM2 C2 can come out on two different pins
        TIMx_CCER_OFFSET = TIM2_CCER # 1-4 on TIM1, 5-8 on TIM2
        TIMx_CCER_shift = 4
        TIMx_CCMR_OFFSET = TIM2_CCMR1
        TIMx_CCMR_shift = 8
        alternate_output_mask = 0x20
    elif (io == 1) or (io == 11): # TIM2 C3 can come out on two different pins
        TIMx_CCER_OFFSET = TIM2_CCER # 1-4 on TIM1, 5-8 on TIM2
        TIMx_CCER_shift = 8
        TIMx_CCMR_OFFSET = TIM2_CCMR2
        TIMx_CCMR_shift = 0
        alternate_output_mask = 0x40
    elif (io == 2) or  (io == 12): # TIM2 C4 can come out on two different pins
        TIMx_CCER_OFFSET = TIM2_CCER # 1-4 on TIM1, 5-8 on TIM2
        TIMx_CCER_shift = 12
        TIMx_CCMR_OFFSET = TIM2_CCMR2
        TIMx_CCMR_shift = 8
        alternate_output_mask = 0x80
    else:
        return # There *IS* no PWM corresponding to the specified io pin

    # The TIM1 PWM pins are not remappable
    if (io == 14) or (io == 15) or (io == 6) or (io == 7):
        alternate_output_possible = False
    else:
        alternate_output_possible = True
    if (io >= 9) and (io <= 12):
        alternate_output_pin = True
    else:
        alternate_output_pin = False

    # Datasheet says you must disable a timer channel before you can re-program it
    TIMx_CCER_value = peek(IOBASE, TIMx_CCER_OFFSET, 3)
    TIMx_CCER_value &= ~(1 << TIMx_CCER_shift)
    poke(IOBASE, TIMx_CCER_OFFSET, 2, 0x0000, TIMx_CCER_value)

    # Specify PWM Mode 1
    TIMx_CCMR_value = peek(IOBASE, TIMx_CCMR_OFFSET, 3)
    TIMx_CCMR_value &= ~(0x70 << TIMx_CCMR_shift)
    # PWM Mode 1
    #TIMx_CCMR_value |= (0x60 << TIMx_CCMR_shift)
    # PWM Mode 2
    TIMx_CCMR_value |= (0x70 << TIMx_CCMR_shift)

    # Specify Output Compare Buffer Enable
    TIMx_CCMR_value |= (0x08 << TIMx_CCMR_shift)

    # Specify Output Compare Fast Enable is OFF
    TIMx_CCMR_value &= ~(0x04 << TIMx_CCMR_shift)

    # Specify we want output
    TIMx_CCMR_value &= ~(0x03 << TIMx_CCMR_shift)

    poke(IOBASE, TIMx_CCMR_OFFSET, 2, 0x0000, TIMx_CCMR_value)

    # Specify correct output pin (if this is one of the remappable timers)
    if alternate_output_possible:
        TIM2_OR_value = peek(IOBASE, TIM2_OR, 3)
        if alternate_output_pin:
            TIM2_OR_value |= alternate_output_mask
        else:
            TIM2_OR_value &= ~alternate_output_mask
        poke(IOBASE, TIM2_OR, 2, 0x0000, TIM2_OR_value)

    # Set polarity
    if invert:
        TIMx_CCER_value |= (2 << TIMx_CCER_shift)
    else:
        TIMx_CCER_value &= ~(2 << TIMx_CCER_shift)

    # and finally re-enable this particular timer channel
    TIMx_CCER_value |= (1 << TIMx_CCER_shift)
    poke(IOBASE, TIMx_CCER_OFFSET, 2, 0x0000, TIMx_CCER_value)

    # Set intial duty cycle
    pwmSetDutyCycle(dutyCycle)

    setPinMode(io, 0x09) # Make the pin be a TIMx output

def pwmSetDutyCycle(io, value):
    if io == 14: # TIM1 C1
        TIMx_CCRy = TIM1_CCR1
    elif io == 15: # TIM1 C2
        TIMx_CCRy = TIM1_CCR2
    elif io == 6: # TIM1 C3
        TIMx_CCRy = TIM1_CCR3
    elif io == 7: # TIM1 C4
        TIMx_CCRy = TIM1_CCR4
    elif (io == 0) or (io == 9): # TIM2 C1 can come out on two different pins
        TIMx_CCRy = TIM2_CCR1
    elif (io == 3) or (io == 10): # TIM2 C2 can come out on two different pins
        TIMx_CCRy = TIM2_CCR2
    elif (io == 1) or (io == 11): # TIM2 C3 can come out on two different pins
        TIMx_CCRy = TIM2_CCR3
    elif (io == 2) or  (io == 12): # TIM2 C4 can come out on two different pins
        TIMx_CCRy = TIM2_CCR4
    else:
        return # There *IS* no PWM corresponding to the specified io pin

    poke(IOBASE, TIMx_CCRy, 2, 0x0000, value)

def dumpTIM1():
    value = peek(IOBASE, TIM1_CR1, 1)
    print "TIM1_CR1=",
    printHexWord(value)
    value = peek(IOBASE, TIM1_CCMR1, 1)
    print " TIM1_CCMR1=",
    printHexWord(value)
    # Commented out the following just to keep the output on a single line
    #value = peek(IOBASE, TIM1_CCMR2, 1)
    #print " TIM1_CCMR2=",
    #printHexWord(value)
    value = peek(IOBASE, TIM1_CCER, 1)
    print " TIM1_CCER=",
    printHexWord(value)
    value = peek(IOBASE, TIM1_PSC, 1)
    print " TIM1_PSC=",
    printHexWord(value)
    value = peek(IOBASE, TIM1_ARR, 1)
    print " TIM1_ARR=",
    printHexWord(value)
    value = peek(IOBASE, TIM1_CNT, 1)
    print " TIM1_CNT=",
    printHexWord(value)
    value = peek(IOBASE, TIM1_CCR1, 1)
    print " TIM1_CCR1=",
    printHexWord(value)

def dumpTIM2():
    value = peek(IOBASE, TIM2_CR1, 1)
    print "TIM2_CR1=",
    printHexWord(value)
    value = peek(IOBASE, TIM2_CCMR1, 1)
    print " TIM2_CCMR1=",
    printHexWord(value)
    value = peek(IOBASE, TIM2_CCMR2, 1)
    print " TIM2_CCMR2=",
    printHexWord(value)
    value = peek(IOBASE, TIM2_CCER, 1)
    print " TIM2_CCER=",
    printHexWord(value)
    value = peek(IOBASE, TIM2_PSC, 1)
    print " TIM2_PSC=",
    printHexWord(value)
    value = peek(IOBASE, TIM2_ARR, 1)
    print " TIM2_ARR=",
    printHexWord(value)
    value = peek(IOBASE, TIM2_CNT, 1)
    print " TIM2_CNT=",
    printHexWord(value)
    value = peek(IOBASE, TIM2_CCR1, 1)
    print " TIM2_CCR1=",
    printHexWord(value)
