# Copyright (C) 2011 Synapse Wireless, Inc.
# Subject to your agreement of the disclaimer set forth below, permission is given by Synapse Wireless, Inc. ("Synapse") to you to freely modify, redistribute or include this SNAPpy code in any program. The purpose of this code is to help you understand and learn about SNAPpy by code examples.
# BY USING ALL OR ANY PORTION OF THIS SNAPPY CODE, YOU ACCEPT AND AGREE TO THE BELOW DISCLAIMER. If you do not accept or agree to the below disclaimer, then you may not use, modify, or distribute this SNAPpy code.
# THE CODE IS PROVIDED UNDER THIS LICENSE ON AN "AS IS" BASIS, WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, WITHOUT LIMITATION, WARRANTIES THAT THE COVERED CODE IS FREE OF DEFECTS, MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE OR NON-INFRINGING. THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE COVERED CODE IS WITH YOU. SHOULD ANY COVERED CODE PROVE DEFECTIVE IN ANY RESPECT, YOU (NOT THE INITIAL DEVELOPER OR ANY OTHER CONTRIBUTOR) ASSUME THE COST OF ANY NECESSARY SERVICING, REPAIR OR CORRECTION. UNDER NO CIRCUMSTANCES WILL SYNAPSE BE LIABLE TO YOU, OR ANY OTHER PERSON OR ENTITY, FOR ANY LOSS OF USE, REVENUE OR PROFIT, LOST OR DAMAGED DATA, OR OTHER COMMERCIAL OR ECONOMIC LOSS OR FOR ANY DAMAGES WHATSOEVER RELATED TO YOUR USE OR RELIANCE UPON THE SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES OR IF SUCH DAMAGES ARE FORESEEABLE. THIS DISCLAIMER OF WARRANTY AND LIABILITY CONSTITUTES AN ESSENTIAL PART OF THIS LICENSE. NO USE OF ANY COVERED CODE IS AUTHORIZED HEREUNDER EXCEPT UNDER THIS DISCLAIMER.
#
# pinWakeupSTM32W108xB.py - an example of using peek/poke commands to extend
# SNAPpy's capabilities, in this case to enable the "wakeup" features of the 24
# GPIO pins.
#
# Pro: All 24 io pins are capable of waking up the processor on this SNAP platform
# Con: Unlike many SNAP platforms, you have no control over polarity -
#         *ANY* transition on a pin will wakeup the processor
# (Depending on your application, this might be a plus instead of a minus)
#

from synapse.STM32W108xB_GPIO import * # to get IOBASE definition

# Data WAKEup registers for Ports A-C
GPIO_PAWAKE = 0xBC08
GPIO_PBWAKE = 0xBC0C
GPIO_PCWAKE = 0xBC10

def wakeupOn(io, isEnabled):
    if 0 <= io <= 7:
        GPIO_PxWAKE = GPIO_PAWAKE
        shift = io
    elif 8 <= io <= 15:
        GPIO_PxWAKE = GPIO_PBWAKE
        shift = io - 8
    elif 16 <= io <= 23:
        GPIO_PxWAKE = GPIO_PCWAKE
        shift = io - 16
    else:
        return # invalid pin specified
    temp = peek(IOBASE, GPIO_PxWAKE, 1) 
    if isEnabled:
        temp |= (1 << shift)
    else:
        temp &= ~(1 << shift)
    poke(IOBASE, GPIO_PxWAKE, 2, 0x0000, temp) 
