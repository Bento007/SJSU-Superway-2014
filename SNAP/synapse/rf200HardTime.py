# Copyright (C) 2011 Synapse Wireless, Inc.
# Subject to your agreement of the disclaimer set forth below, permission is given by Synapse Wireless, Inc. ("Synapse") to you to freely modify, redistribute or include this SNAPpy code in any program. The purpose of this code is to help you understand and learn about SNAPpy by code examples.
# BY USING ALL OR ANY PORTION OF THIS SNAPPY CODE, YOU ACCEPT AND AGREE TO THE BELOW DISCLAIMER. If you do not accept or agree to the below disclaimer, then you may not use, modify, or distribute this SNAPpy code.
# THE CODE IS PROVIDED UNDER THIS LICENSE ON AN "AS IS" BASIS, WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, WITHOUT LIMITATION, WARRANTIES THAT THE COVERED CODE IS FREE OF DEFECTS, MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE OR NON-INFRINGING. THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE COVERED CODE IS WITH YOU. SHOULD ANY COVERED CODE PROVE DEFECTIVE IN ANY RESPECT, YOU (NOT THE INITIAL DEVELOPER OR ANY OTHER CONTRIBUTOR) ASSUME THE COST OF ANY NECESSARY SERVICING, REPAIR OR CORRECTION. UNDER NO CIRCUMSTANCES WILL SYNAPSE BE LIABLE TO YOU, OR ANY OTHER PERSON OR ENTITY, FOR ANY LOSS OF USE, REVENUE OR PROFIT, LOST OR DAMAGED DATA, OR OTHER COMMERCIAL OR ECONOMIC LOSS OR FOR ANY DAMAGES WHATSOEVER RELATED TO YOUR USE OR RELIANCE UPON THE SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES OR IF SUCH DAMAGES ARE FORESEEABLE. THIS DISCLAIMER OF WARRANTY AND LIABILITY CONSTITUTES AN ESSENTIAL PART OF THIS LICENSE. NO USE OF ANY COVERED CODE IS AUTHORIZED HEREUNDER EXCEPT UNDER THIS DISCLAIMER.

"""Hardware timer API for AVR, using Timer 1"""

TMR_FREQ = 16000000/1024   # Ticks per sec
TMR_TICKS_PER_MS = 16
TCCR1B = 0x81  # Control register
TCNT1H = 0x85  # High byte
TCNT1L = 0x84  # Low byte

def initHwTmr():
    """Initialize the hardware timer, and reset count"""
    poke(TCCR1B, 5)  # Run counter-mode with prescaler at divide-by 1024
    resetTmr()

def resetTmr():
    """Reset hardware timer count to zero (timer runs continuously)"""
    # Note: Must read LO byte before HI byte
    peek(TCNT1L)
    peek(TCNT1H)
    # Note: Must write HI byte before LO byte
    poke(TCNT1H, 0)
    poke(TCNT1L, 0)

def tmrTicks():
    # Note: Must read LO byte before HI byte
    return  peek(TCNT1L) | (peek(TCNT1H) << 8)

def tmrMs():
    """Return the elapsed timer count in milliseconds.
       Note:  count goes negative after 2097ms, and wraps at 4194ms
    """
    return tmrTicks() / TMR_TICKS_PER_MS

