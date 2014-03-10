# Copyright (C) 2011 Synapse Wireless, Inc.
# Subject to your agreement of the disclaimer set forth below, permission is given by Synapse Wireless, Inc. ("Synapse") to you to freely modify, redistribute or include this SNAPpy code in any program. The purpose of this code is to help you understand and learn about SNAPpy by code examples.
# BY USING ALL OR ANY PORTION OF THIS SNAPPY CODE, YOU ACCEPT AND AGREE TO THE BELOW DISCLAIMER. If you do not accept or agree to the below disclaimer, then you may not use, modify, or distribute this SNAPpy code.
# THE CODE IS PROVIDED UNDER THIS LICENSE ON AN "AS IS" BASIS, WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, WITHOUT LIMITATION, WARRANTIES THAT THE COVERED CODE IS FREE OF DEFECTS, MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE OR NON-INFRINGING. THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE COVERED CODE IS WITH YOU. SHOULD ANY COVERED CODE PROVE DEFECTIVE IN ANY RESPECT, YOU (NOT THE INITIAL DEVELOPER OR ANY OTHER CONTRIBUTOR) ASSUME THE COST OF ANY NECESSARY SERVICING, REPAIR OR CORRECTION. UNDER NO CIRCUMSTANCES WILL SYNAPSE BE LIABLE TO YOU, OR ANY OTHER PERSON OR ENTITY, FOR ANY LOSS OF USE, REVENUE OR PROFIT, LOST OR DAMAGED DATA, OR OTHER COMMERCIAL OR ECONOMIC LOSS OR FOR ANY DAMAGES WHATSOEVER RELATED TO YOUR USE OR RELIANCE UPON THE SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES OR IF SUCH DAMAGES ARE FORESEEABLE. THIS DISCLAIMER OF WARRANTY AND LIABILITY CONSTITUTES AN ESSENTIAL PART OF THIS LICENSE. NO USE OF ANY COVERED CODE IS AUTHORIZED HEREUNDER EXCEPT UNDER THIS DISCLAIMER.

"""This SNAPpy script implements "wakeup on GPIO" capability"""

# Note the use of one routine (with multiple parameters) to save memory

def wakeupOn(pin, isEnabled, polarityIsRising):
    """Controls "wakeup" feature for the specified pin (26, 27, 28, 29)"""
    if pin > 25 and pin < 30:
        # pin 26 = KBI4. pin 29 = KBI7.
        # Note, we're configuring here for level, not edge, wakes.
        # This can be adjusted in EXT_WU_EDGE if you need to do so.
        
        theMask = 1 << (pin - 22)
        reg_WU_CNTL_high = peek(0x8000, 0x3004, 2)
        reg_WU_CNTL_low = peek()
        
        mask_EXT_WU_POL = theMask << 8
        mask_EXT_WU_EN = theMask
        
        if isEnabled:
            # Adjust the polarity
            if polarityIsRising:
                reg_WU_CNTL_low |= mask_EXT_WU_POL
            else:
                reg_WU_CNTL_low &= ~mask_EXT_WU_POL
            # Enable the wakeup
            reg_WU_CNTL_low |= mask_EXT_WU_EN
        else:
            reg_WU_CNTL_low &= ~mask_EXT_WU_EN
            reg_WU_CNTL_low |= mask_EXT_WU_POL # Defaults to 1
        
        poke(0x8000, 0x3004, 2, reg_WU_CNTL_high, reg_WU_CNTL_low)
