# Copyright (C) 2011 Synapse Wireless, Inc.
# Subject to your agreement of the disclaimer set forth below, permission is given by Synapse Wireless, Inc. ("Synapse") to you to freely modify, redistribute or include this SNAPpy code in any program. The purpose of this code is to help you understand and learn about SNAPpy by code examples.
# BY USING ALL OR ANY PORTION OF THIS SNAPPY CODE, YOU ACCEPT AND AGREE TO THE BELOW DISCLAIMER. If you do not accept or agree to the below disclaimer, then you may not use, modify, or distribute this SNAPpy code.
# THE CODE IS PROVIDED UNDER THIS LICENSE ON AN "AS IS" BASIS, WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, WITHOUT LIMITATION, WARRANTIES THAT THE COVERED CODE IS FREE OF DEFECTS, MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE OR NON-INFRINGING. THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE COVERED CODE IS WITH YOU. SHOULD ANY COVERED CODE PROVE DEFECTIVE IN ANY RESPECT, YOU (NOT THE INITIAL DEVELOPER OR ANY OTHER CONTRIBUTOR) ASSUME THE COST OF ANY NECESSARY SERVICING, REPAIR OR CORRECTION. UNDER NO CIRCUMSTANCES WILL SYNAPSE BE LIABLE TO YOU, OR ANY OTHER PERSON OR ENTITY, FOR ANY LOSS OF USE, REVENUE OR PROFIT, LOST OR DAMAGED DATA, OR OTHER COMMERCIAL OR ECONOMIC LOSS OR FOR ANY DAMAGES WHATSOEVER RELATED TO YOUR USE OR RELIANCE UPON THE SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES OR IF SUCH DAMAGES ARE FORESEEABLE. THIS DISCLAIMER OF WARRANTY AND LIABILITY CONSTITUTES AN ESSENTIAL PART OF THIS LICENSE. NO USE OF ANY COVERED CODE IS AUTHORIZED HEREUNDER EXCEPT UNDER THIS DISCLAIMER.

"""
This SNAPpy script implements "wakeup on pin change" capability for the ATmega128RFA1
Currently there IS no ATMEL SNAP Engine. This script works in terms of *IO* pins,
not GPIO pins. The following IO pins can wake the processor: 0-11,16,20-23
Note that power consumption will be higher if IO 20-23 are used in Edge mode.
"""

# There are 8 "External Interrupt" pins

# EIMSK - External Interrupt Mask
EIMSK = 0x3D

# EIFR - External Interrupt Flag Register
EIFR = 0x3C

# EICRA - External Interrupt Control Register A
# Controls INT3:0
EICRA = 0x69

# EICRB - External Interrupt Control Register B
# Controls INT7:4
EICRB = 0x6A

# Possible EICRx settings (per INTx)
INT_IF_LOW = 0
INT_IF_EDGE = 1
INT_IF_FALLING = 2
INT_IF_RISING = 3

# There are also 9 PCINT pins

# PCICR - Pin Change Interrupt Control Register
PCICR = 0x68

# PCIFR - Pin change Interrupt Flag Register
PCIFR = 0x3B

# Bit positions within the above registers
PCI0 = 1
PCI1 = 2

# PCMSK1 Pin Change Mask 1 (PCINT8)
PCMSK1 = 0x6C

# PCMSK0 Pin Change Mask 0 (PCINT7-0)
PCMSK0 = 0x6B


# The current version is intended mainly as an example. It even uses more than
# one style of coding -compare the programming of IO 0-7 versus the higher ones.
# This was just done to show that more than one form of bit-manipulation is possible.
# Also, the API may need to change. The ATMEL chip supports additional options
# not exercised by this example version.
def wakeupOn(pin, isEnabled, polarityIsRising):
    """Controls "wakeup" feature for the specified "IO" (0-11,16,23)"""
    # Precompute masks
    if pin == 0: # PCINT0
        mask = 1
        mask2 = 1
    elif pin == 1: # PCINT1
        mask = 2
        mask2 = 1
    elif pin == 2: # PCINT2
        mask = 4
        mask2 = 1
    elif pin == 3: # PCINT3
        mask = 8
        mask2 = 1
    elif pin == 4: # PCINT4
        mask = 16
        mask2 = 1
    elif pin == 5: # PCINT5
        mask = 32
        mask2 = 1
    elif pin == 6: # PCINT6
        mask = 64
        mask2 = 1
    elif pin == 7: # PCINT7
        mask = 128
        mask2 = 1
    elif pin == 8: # INT0
        shift = 0
        shift2 = 0
    elif pin == 9: # INT1
        shift = 1
        shift2 = 2
    elif pin == 10: # INT2
        shift = 2
        shift2 = 4
    elif pin == 11: # INT3
        shift = 3
        shift2 = 6
    elif pin == 16: # PCINT7
        mask = 1
        mask2 = 2
    elif pin == 20: # INT4
        shift = 0
        shift2 = 0
    elif pin == 21: # INT5
        shift = 1
        shift2 = 2
    elif pin == 22: # INT6
        shift = 2
        shift2 = 4
    elif pin == 23: # INT7
        shift = 3
        shift2 = 6
    else:
        return # only certain IOs can do this trick

    # Disable the interrupt while reconfiguring it
    if (pin >= 0) and (pin <= 7):
        poke(PCMSK0, peek(PCMSK0) & ~mask)
    elif (pin >= 8) and (pin <= 11):
        poke(EIMSK, peek(EIMSK) & ~(1 << shift))
    elif pin == 16:
        poke(PCMSK1, peek(PCMSK1) & ~mask)
    elif (pin >= 20) and (pin <= 23):
        poke(EIMSK, peek(EIMSK) & ~(1 << (4+shift)))

    # Polarity (currently only supporting EDGE interrupts)
    if (pin >= 0) and (pin <= 7):
        pass # PCINT0-7 not configurable
    elif (pin >= 8) and (pin <= 11):
        # Make a hole
        poke(EICRA, peek(EICRA) & ~(3 << shift2))
        # Fill it in with the new setting
        if polarityIsRising:
            poke(EICRA, peek(EICRA) | (INT_IF_RISING << shift2))
        else:
            poke(EICRA, peek(EICRA) | (INT_IF_FALLING << shift2))
    elif pin == 16:
        pass # PCINT8 not configurable
    elif (pin >= 20) and (pin <= 23):
        # Make a hole
        poke(EICRB, peek(EICRB) & ~(3 << shift2))
        # Fill it in with the new setting
        if polarityIsRising:
            poke(EICRB, peek(EICRB) | (INT_IF_RISING << shift2))
        else:
            poke(EICRB, peek(EICRB) | (INT_IF_FALLING << shift2))

    # Clear any pending flag
    if (pin >= 0) and (pin <= 7):
        poke(PCIFR, peek(PCIFR) | mask2) # writing to the bit resets it
    elif (pin >= 8) and (pin <= 11):
        poke(EIFR, peek(EIFR) | (1 << shift)) # you WRITE a 1 to reset it
    elif pin == 16:
        poke(PCIFR, peek(PCIFR) | mask2) # writing to the bit resets it
    elif (pin >= 20) and (pin <= 23):
        poke(EIFR, peek(EIFR) | (1 << (4+shift))) # you WRITE a 1 to reset it
    
    # Enable interrupt if requested (unconditionally turned off up above)
    if isEnabled:  
        if (pin >= 0) and (pin <= 7):
            poke(PCMSK0, peek(PCMSK0) | mask)
            poke(PCICR, peek(PCICR) | mask2)
        elif (pin >= 8) and (pin <= 11):
            poke(EIMSK, peek(EIMSK) | (1 << shift))
        elif pin == 16:
            poke(PCMSK1, peek(PCMSK1) | mask)
            poke(PCICR, peek(PCICR) | mask2)
        elif (pin >= 20) and (pin <= 23):
            poke(EIMSK, peek(EIMSK) | (1 << (4+shift)))
    else: # Turn off the SHARED interrupt bit if nobody is left
        if (pin >= 0) and (pin <= 7):
            if peek(PCMSK0) == 0:
                poke(PCICR, peek(PCICR) & ~mask2)
        elif pin == 16:
            if (peek(PCMSK1) & 1) == 0:
                poke(PCICR, peek(PCICR) & ~mask2)
