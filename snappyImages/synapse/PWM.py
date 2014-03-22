# Copyright (C) 2011 Synapse Wireless, Inc.
# Subject to your agreement of the disclaimer set forth below, permission is given by Synapse Wireless, Inc. ("Synapse") to you to freely modify, redistribute or include this SNAPpy code in any program. The purpose of this code is to help you understand and learn about SNAPpy by code examples.
# BY USING ALL OR ANY PORTION OF THIS SNAPPY CODE, YOU ACCEPT AND AGREE TO THE BELOW DISCLAIMER. If you do not accept or agree to the below disclaimer, then you may not use, modify, or distribute this SNAPpy code.
# THE CODE IS PROVIDED UNDER THIS LICENSE ON AN "AS IS" BASIS, WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, WITHOUT LIMITATION, WARRANTIES THAT THE COVERED CODE IS FREE OF DEFECTS, MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE OR NON-INFRINGING. THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE COVERED CODE IS WITH YOU. SHOULD ANY COVERED CODE PROVE DEFECTIVE IN ANY RESPECT, YOU (NOT THE INITIAL DEVELOPER OR ANY OTHER CONTRIBUTOR) ASSUME THE COST OF ANY NECESSARY SERVICING, REPAIR OR CORRECTION. UNDER NO CIRCUMSTANCES WILL SYNAPSE BE LIABLE TO YOU, OR ANY OTHER PERSON OR ENTITY, FOR ANY LOSS OF USE, REVENUE OR PROFIT, LOST OR DAMAGED DATA, OR OTHER COMMERCIAL OR ECONOMIC LOSS OR FOR ANY DAMAGES WHATSOEVER RELATED TO YOUR USE OR RELIANCE UPON THE SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES OR IF SUCH DAMAGES ARE FORESEEABLE. THIS DISCLAIMER OF WARRANTY AND LIABILITY CONSTITUTES AN ESSENTIAL PART OF THIS LICENSE. NO USE OF ANY COVERED CODE IS AUTHORIZED HEREUNDER EXCEPT UNDER THIS DISCLAIMER.

"""
Support routines for Pulse Width Modulation (PWM) output on pin GPIO 0
To understand why these poke() statements work, refer to chapter 10 of
the MC9S08GB60A data sheet from Freescale Semiconductor. RF Engine pin
GPIO 0 is connected to pin TPM1C2 of the processor.
"""

# Additional notes:
# 'divisorCode' controls the resolution, 'modulus' controls the period
# Higher divisor value allows longer pulses, but coarser granularity
# 'modulus' is actually passed in as MSB, LSB so that the full unsigned
# 16 bits is attainable
def setTimebaseMsbLsb(divisorCode, modh, modl):
    """
    Sets the timebase and period for the pulse stream
    'divisorCode' 0-7 maps to a divisor of 1,2,4,8,16,32,64, or 128
    The frequency will be = 19995428.5 / (divisor * modulus)
    """
    poke(0x30, 0x08 | divisorCode) # Set TPM1SC to use BUSCLK and specified divisor
    # Divisor has been specified, now specify the modulus (rollover) value 
    poke(0x33, modh) # Set TPM1MODH
    poke(0x34, modl) # Set TPM1MODL 
    poke(0x31, 0x00) # Restart TPM1CNT

# See also setTimebaseMsbLsb(), which this routine uses
def setTimebaseWord(divisorCode, modulus):
    """
    Sets the timebase and period for the pulse stream
    'divisorCode' controls the resolution, 'modulus' controls the period
    Here 'modulus' is a signed value, see also setTimebaseMsbLsb()
    """
    msb = modulus / 256
    lsb = modulus % 256
    setTimebaseMsbLsb(divisorCode, msb, lsb)

# Additional notes:
# A value of 0 will be a 0% duty cycle, a value of modulus+1 = 100%
def setDutyCycleMsbLsb(polarity, valh, vall):
    """
    Sets the duty cycle for the pulse stream
    'polarity' specifies logic level of first portion of pulse
    'value' specifies width of the first portion, and valid values
    depend on the modulus value specified in setTimebaseXXX()
    """
    if polarity:
        poke(0x3B, 0x28) # Set TPM1C2SC for Edge PWM, High then Low
    else:
        poke(0x3B, 0x24) # Set TPM1C2SC for Edge PWM, Low then High
    poke(0x3c, valh) # Set TMP1C2VH
    poke(0x3d, vall) # Set TMP1C2VL

# See also setDutyCycleMsbLsb(), which this routine uses
def setDutyCycleWord(polarity, value):
    """
    Sets the duty cycle for the pulse stream
    'polarity' specifies logic level of first portion of pulse
    'value' specifies width of the first portion, and valid values
    depend on the modulus value specified in setTimebaseXXX()
    """
    msb = value / 256
    lsb = value % 256
    setDutyCycleMsbLsb(polarity, msb, lsb)
