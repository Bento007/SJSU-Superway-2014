# Copyright (C) 2011 Synapse Wireless, Inc.
# Subject to your agreement of the disclaimer set forth below, permission is given by Synapse Wireless, Inc. ("Synapse") to you to freely modify, redistribute or include this SNAPpy code in any program. The purpose of this code is to help you understand and learn about SNAPpy by code examples.
# BY USING ALL OR ANY PORTION OF THIS SNAPPY CODE, YOU ACCEPT AND AGREE TO THE BELOW DISCLAIMER. If you do not accept or agree to the below disclaimer, then you may not use, modify, or distribute this SNAPpy code.
# THE CODE IS PROVIDED UNDER THIS LICENSE ON AN "AS IS" BASIS, WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, WITHOUT LIMITATION, WARRANTIES THAT THE COVERED CODE IS FREE OF DEFECTS, MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE OR NON-INFRINGING. THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE COVERED CODE IS WITH YOU. SHOULD ANY COVERED CODE PROVE DEFECTIVE IN ANY RESPECT, YOU (NOT THE INITIAL DEVELOPER OR ANY OTHER CONTRIBUTOR) ASSUME THE COST OF ANY NECESSARY SERVICING, REPAIR OR CORRECTION. UNDER NO CIRCUMSTANCES WILL SYNAPSE BE LIABLE TO YOU, OR ANY OTHER PERSON OR ENTITY, FOR ANY LOSS OF USE, REVENUE OR PROFIT, LOST OR DAMAGED DATA, OR OTHER COMMERCIAL OR ECONOMIC LOSS OR FOR ANY DAMAGES WHATSOEVER RELATED TO YOUR USE OR RELIANCE UPON THE SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES OR IF SUCH DAMAGES ARE FORESEEABLE. THIS DISCLAIMER OF WARRANTY AND LIABILITY CONSTITUTES AN ESSENTIAL PART OF THIS LICENSE. NO USE OF ANY COVERED CODE IS AUTHORIZED HEREUNDER EXCEPT UNDER THIS DISCLAIMER.

"""Example routines for accessing the two PWM pins on a ZIC2410"""

# PWM 2 is port 3.6 => IO 22
PWM2_PIN = 22
# PWM 3 is port 3.7 => IO 23
PWM3_PIN = 23

# ZIC2410 Timer 2/3 register addresses (negative specifies internal registers)
T23CON	= -0xA9
TH2		= -0xAA
TH3		= -0xAB
TL2		= -0xAC
TL3		= -0xAD

def setPwm(pin, freq, highDuty):
    """Set PWM frequency and duty-cycle
        (sysFreq = 16MHz)
        PWMfreq = sysFreq / (256 * (THx + 1))
        PWMduty = TLx / 256
    """
    # Calculate divisor to approximate requested frequency
    freq /= 20
    n = ((31250/freq) + 5) / 10

    t23con = peek(T23CON)
    if pin == PWM2_PIN:
        poke(TH2, n)
        poke(TL2, highDuty)
        t23con |= 0x03  # TR2=1, PWM2 Enable=1
    elif pin == PWM3_PIN:
        poke(TH3, n)
        poke(TL3, highDuty)
        t23con |= 0x0c  # TR3=1, PWM3 Enable=1
    else: # PWM2 and PWM3 are the only ones
        return
    poke(T23CON, t23con)

def setDutyCycle(pin, highDuty):
    """Set duty-cycle only (0=0%, 255 = 99.6%)"""
    if pin == PWM2_PIN:
        poke(TL2, highDuty)
    elif pin == PWM3_PIN:
        poke(TL3, highDuty)
    else: # PWM2 and PWM3 are the only ones
        return

def stopPwm(pin):
    """Turn off the specified PWM"""
    t23con = peek(T23CON)
    if pin == PWM2_PIN:
        t23con &= ~0x03
    elif pin == PWM3_PIN:
        t23con &= ~0x0C
    else: # PWM2 and PWM3 are the only ones
        return
    poke(T23CON, t23con)
