# Copyright (C) 2011 Synapse Wireless, Inc.
# Subject to your agreement of the disclaimer set forth below, permission is given by Synapse Wireless, Inc. ("Synapse") to you to freely modify, redistribute or include this SNAPpy code in any program. The purpose of this code is to help you understand and learn about SNAPpy by code examples.
# BY USING ALL OR ANY PORTION OF THIS SNAPPY CODE, YOU ACCEPT AND AGREE TO THE BELOW DISCLAIMER. If you do not accept or agree to the below disclaimer, then you may not use, modify, or distribute this SNAPpy code.
# THE CODE IS PROVIDED UNDER THIS LICENSE ON AN "AS IS" BASIS, WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, WITHOUT LIMITATION, WARRANTIES THAT THE COVERED CODE IS FREE OF DEFECTS, MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE OR NON-INFRINGING. THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE COVERED CODE IS WITH YOU. SHOULD ANY COVERED CODE PROVE DEFECTIVE IN ANY RESPECT, YOU (NOT THE INITIAL DEVELOPER OR ANY OTHER CONTRIBUTOR) ASSUME THE COST OF ANY NECESSARY SERVICING, REPAIR OR CORRECTION. UNDER NO CIRCUMSTANCES WILL SYNAPSE BE LIABLE TO YOU, OR ANY OTHER PERSON OR ENTITY, FOR ANY LOSS OF USE, REVENUE OR PROFIT, LOST OR DAMAGED DATA, OR OTHER COMMERCIAL OR ECONOMIC LOSS OR FOR ANY DAMAGES WHATSOEVER RELATED TO YOUR USE OR RELIANCE UPON THE SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES OR IF SUCH DAMAGES ARE FORESEEABLE. THIS DISCLAIMER OF WARRANTY AND LIABILITY CONSTITUTES AN ESSENTIAL PART OF THIS LICENSE. NO USE OF ANY COVERED CODE IS AUTHORIZED HEREUNDER EXCEPT UNDER THIS DISCLAIMER.

"""
Support routines for Pulse Width Modulation (PWM) output on pins
GPIO 14 and GPIO 15.

Refer to PWM.py for control of PWM on GPIO 0 (TPM1CH2)

To understand why these poke() statements work, refer to chapter 10
of the MC9S08GB60A data sheet from Freescale Semiconductor.

PAN4555 pin GPIO 14 is connected to pin TPM2C3 of the processor.
PAN4555 pin GPIO 15 is connected to pin TPM2C1 of the processor.
"""

# Additional notes:
# SNAP uses TPM2 for its 1 millisecond timebase, so we do not have
# as much flexibility on *these* PWM pins as we do on GPIO 0 (TPM1CH2)
# For these pins, the divisor is always 1, modulus is always 19,995

def setTPM2C1DutyCycleMsbLsb(polarity, valh, vall):
    """
    Sets the duty cycle for the pulse stream on GPIO 15
    'polarity' specifies logic level of first portion of pulse
    'value' specifies width of the first portion, and valid values
    are 0 (0%) to TPM2_MODULUS+1 (100%) but split into two bytes
    """
    if polarity:
        poke(0x68, 0x28) # Set TPM2C1SC for Edge PWM, High then Low
    else:
        poke(0x68, 0x24) # Set TPM2C1SC for Edge PWM, Low then High
    poke(0x69, valh) # Set TPM2C1VH
    poke(0x6A, vall) # Set TPM2C1VL

# See also setTPM2C1DutyCycleMsbLsb(), which this routine uses
def setTPM2C1DutyCycleWord(polarity, value):
    """
    Sets the duty cycle for the pulse stream on GPIO 15
    'polarity' specifies logic level of first portion of pulse
    'value' specifies width of the first portion, and valid values
    are 0 (0%) to TPM2_MODULUS+1 (100%)
    """
    msb = value / 256
    lsb = value % 256
    setTPM2C1DutyCycleMsbLsb(polarity, msb, lsb)

# A value of 0 will be a 0% duty cycle, a value of modulus+1 = 100%
def setTPM2C3DutyCycleMsbLsb(polarity, valh, vall):
    """
    Sets the duty cycle for the pulse stream on GPIO 14
    'polarity' specifies logic level of first portion of pulse
    'value' specifies width of the first portion, and valid values
    are 0 (0%) to TPM2_MODULUS+1 (100%) but split into two bytes
    """
    if polarity:
        poke(0x6E, 0x28) # Set TPM2C3SC for Edge PWM, High then Low
    else:
        poke(0x6E, 0x24) # Set TPM2C3SC for Edge PWM, Low then High
    poke(0x6F, valh) # Set TPM2C3VH
    poke(0x70, vall) # Set TPM2C3VL

# See also setTPM2C3DutyCycleMsbLsb(), which this routine uses
def setTPM2C3DutyCycleWord(polarity, value):
    """
    Sets the duty cycle for the pulse stream on GPIO 14
    'polarity' specifies logic level of first portion of pulse
    'value' specifies width of the first portion, and valid values
    are 0 (0%) to TPM2_MODULUS+1 (100%)
    """
    msb = value / 256
    lsb = value % 256
    setTPM2C3DutyCycleMsbLsb(polarity, msb, lsb)
