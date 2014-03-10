# Copyright (C) 2011 Synapse Wireless, Inc.
# Subject to your agreement of the disclaimer set forth below, permission is given by Synapse Wireless, Inc. ("Synapse") to you to freely modify, redistribute or include this SNAPpy code in any program. The purpose of this code is to help you understand and learn about SNAPpy by code examples.
# BY USING ALL OR ANY PORTION OF THIS SNAPPY CODE, YOU ACCEPT AND AGREE TO THE BELOW DISCLAIMER. If you do not accept or agree to the below disclaimer, then you may not use, modify, or distribute this SNAPpy code.
# THE CODE IS PROVIDED UNDER THIS LICENSE ON AN "AS IS" BASIS, WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, WITHOUT LIMITATION, WARRANTIES THAT THE COVERED CODE IS FREE OF DEFECTS, MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE OR NON-INFRINGING. THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE COVERED CODE IS WITH YOU. SHOULD ANY COVERED CODE PROVE DEFECTIVE IN ANY RESPECT, YOU (NOT THE INITIAL DEVELOPER OR ANY OTHER CONTRIBUTOR) ASSUME THE COST OF ANY NECESSARY SERVICING, REPAIR OR CORRECTION. UNDER NO CIRCUMSTANCES WILL SYNAPSE BE LIABLE TO YOU, OR ANY OTHER PERSON OR ENTITY, FOR ANY LOSS OF USE, REVENUE OR PROFIT, LOST OR DAMAGED DATA, OR OTHER COMMERCIAL OR ECONOMIC LOSS OR FOR ANY DAMAGES WHATSOEVER RELATED TO YOUR USE OR RELIANCE UPON THE SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES OR IF SUCH DAMAGES ARE FORESEEABLE. THIS DISCLAIMER OF WARRANTY AND LIABILITY CONSTITUTES AN ESSENTIAL PART OF THIS LICENSE. NO USE OF ANY COVERED CODE IS AUTHORIZED HEREUNDER EXCEPT UNDER THIS DISCLAIMER.

"""This SNAPpy script implements "wakeup on GPIO" capability for the ZIC2410"""

ZIC_IE = -0xA8 # <- notice how SFRs are addressed from scripts
ZIC_TCON = -0x88 # <- another SFR

ZIC_PDM = 0x22f0 # memory, not SFR

def wakeupOn(pin, isEnabled, polarityIsRising):
  """Controls "wakeup" feature for the specified "IO" (IO 18=EXT0, IO 19=EXT1)"""
  if pin==18: # GPIO_1
    mask = 0x01
    mask2 = 0x20 # polarity bit in PDM register is different
  elif pin==19: # GPIO_2
    mask = 0x04
    mask2 = 0x10 # polarity bit in PDM register is different
  else:
    return # only IO 18 and 19 can do this trick

  # Disable all interrupts while we are configuring external interrupts
  oldEAbit = peek(ZIC_IE) & 0x80
  poke(ZIC_IE, peek(ZIC_IE) & 0x7F)

  # Polarity
  if polarityIsRising:
    poke(ZIC_PDM, peek(ZIC_PDM) | mask2)
  else:
    poke(ZIC_PDM, peek(ZIC_PDM) & ~mask2)

  # Level triggered because Edge triggered doesn't wake us up from sleep mode (hardware restriction)
  poke(ZIC_TCON, peek(ZIC_TCON) & ~mask)
  
  if isEnabled:  
    poke(ZIC_IE, peek(ZIC_IE) | mask)
  else:
    poke(ZIC_IE, peek(ZIC_IE) & ~mask)

  # Restore the previous interrupt enable setting
  poke(ZIC_IE, peek(ZIC_IE) | oldEAbit)
