# Copyright (C) 2012 Synapse Wireless, Inc.
# Subject to your agreement of the disclaimer set forth below, permission is given by Synapse Wireless, Inc. ("Synapse") to you to freely modify, redistribute or include this SNAPpy code in any program. The purpose of this code is to help you understand and learn about SNAPpy by code examples.
# BY USING ALL OR ANY PORTION OF THIS SNAPPY CODE, YOU ACCEPT AND AGREE TO THE BELOW DISCLAIMER. If you do not accept or agree to the below disclaimer, then you may not use, modify, or distribute this SNAPpy code.
# THE CODE IS PROVIDED UNDER THIS LICENSE ON AN "AS IS" BASIS, WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, WITHOUT LIMITATION, WARRANTIES THAT THE COVERED CODE IS FREE OF DEFECTS, MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE OR NON-INFRINGING. THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE COVERED CODE IS WITH YOU. SHOULD ANY COVERED CODE PROVE DEFECTIVE IN ANY RESPECT, YOU (NOT THE INITIAL DEVELOPER OR ANY OTHER CONTRIBUTOR) ASSUME THE COST OF ANY NECESSARY SERVICING, REPAIR OR CORRECTION. UNDER NO CIRCUMSTANCES WILL SYNAPSE BE LIABLE TO YOU, OR ANY OTHER PERSON OR ENTITY, FOR ANY LOSS OF USE, REVENUE OR PROFIT, LOST OR DAMAGED DATA, OR OTHER COMMERCIAL OR ECONOMIC LOSS OR FOR ANY DAMAGES WHATSOEVER RELATED TO YOUR USE OR RELIANCE UPON THE SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES OR IF SUCH DAMAGES ARE FORESEEABLE. THIS DISCLAIMER OF WARRANTY AND LIABILITY CONSTITUTES AN ESSENTIAL PART OF THIS LICENSE. NO USE OF ANY COVERED CODE IS AUTHORIZED HEREUNDER EXCEPT UNDER THIS DISCLAIMER.

"""Utility functions to access the hardware timer MC1322x-based RF Engines"""

lastTimerReadHigh = 0
lastTimerReadLow = 0

def initHwTmr():
    """Initialize the hardware timer, and reset count"""
    # Included here for consistency with other HardTime modules
    pass

def resetTmr():
    """Reset hardware timer count to zero (timer runs continuously)"""
    global lastTimerReadHigh
    global lastTimerReadLow
    lastTimerReadHigh = peek(0x8000, 0x3028, 2)
    lastTimerReadLow = peek()

def tmrMs():
    """Return the elapsed timer count in milliseconds.
       Timer returns approx. 1,950 'ticks' per second.
    """
    tmrVal = tmrCount()
    if tmrVal <= 1638 and tmrVal >= 0: # Highest value where scaling fits 
        returnValue = tmrVal * 20
        returnValue /= 39
    elif tmrVal <= 16380 and tmrVal >= 0:
        returnValue = tmrVal * 2
        returnValue /= 39
        returnValue *= 10
    else:
        returnValue = tmrVal / 2
    return returnValue

def tmrCount():
    """Return the elapsed timer count in raw 'ticks'"""
    return peek(0x8000, 0x3028, 3) - lastTimerReadLow