# Copyright (C) 2011 Synapse Wireless, Inc.
# Subject to your agreement of the disclaimer set forth below, permission is given by Synapse Wireless, Inc. ("Synapse") to you to freely modify, redistribute or include this SNAPpy code in any program. The purpose of this code is to help you understand and learn about SNAPpy by code examples.
# BY USING ALL OR ANY PORTION OF THIS SNAPPY CODE, YOU ACCEPT AND AGREE TO THE BELOW DISCLAIMER. If you do not accept or agree to the below disclaimer, then you may not use, modify, or distribute this SNAPpy code.
# THE CODE IS PROVIDED UNDER THIS LICENSE ON AN "AS IS" BASIS, WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, WITHOUT LIMITATION, WARRANTIES THAT THE COVERED CODE IS FREE OF DEFECTS, MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE OR NON-INFRINGING. THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE COVERED CODE IS WITH YOU. SHOULD ANY COVERED CODE PROVE DEFECTIVE IN ANY RESPECT, YOU (NOT THE INITIAL DEVELOPER OR ANY OTHER CONTRIBUTOR) ASSUME THE COST OF ANY NECESSARY SERVICING, REPAIR OR CORRECTION. UNDER NO CIRCUMSTANCES WILL SYNAPSE BE LIABLE TO YOU, OR ANY OTHER PERSON OR ENTITY, FOR ANY LOSS OF USE, REVENUE OR PROFIT, LOST OR DAMAGED DATA, OR OTHER COMMERCIAL OR ECONOMIC LOSS OR FOR ANY DAMAGES WHATSOEVER RELATED TO YOUR USE OR RELIANCE UPON THE SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES OR IF SUCH DAMAGES ARE FORESEEABLE. THIS DISCLAIMER OF WARRANTY AND LIABILITY CONSTITUTES AN ESSENTIAL PART OF THIS LICENSE. NO USE OF ANY COVERED CODE IS AUTHORIZED HEREUNDER EXCEPT UNDER THIS DISCLAIMER.

"""This SNAPpy script implements "wakeup on GPIO" capability"""
from synapse.platforms import *

# Note the use of one routine (with multiple parameters) to save memory

def wakeupOn(pin, isEnabled, polarityIsRising):
  """Controls "wakeup" feature for the specified GPIO (6, 7, 8, 9, 10, 12, 13, 14, 15)"""
  if pin == GPIO_6: # P0.0
    mask = 0x01
    register = -0xc7
  elif pin == GPIO_7: # P0.5
    mask = 0x20
    register = -0xc7
  elif pin == GPIO_8: # P0.4
    mask = 0x10
    register = -0xc7
  elif pin == GPIO_9: # P0.3
    mask = 0x08
    register = -0xc7
  elif pin == GPIO_10: # P0.2
    mask = 0x04
    register = -0xc7
  elif pin == GPIO_12: # P1.7
    mask = 0x80
    register = -0xbf
  elif pin == GPIO_13: # P1.5
    mask = 0x20
    register = -0xbf
  elif pin == GPIO_14: # P1.6
    mask = 0x40
    register = -0xbf
  elif pin == GPIO_15: # P0.6
    mask = 0x40
    register = -0xc7
  else:
    return # only pins 6, 7, 8, 9, 10, 12, 13, 14, and 15 can do this trick

  match = register - 0x10 # -0xc7 -> -0xd7. -0xbf -> -0xcf.

  if polarityIsRising:
    # Poke P0MAT or P1MAT (match) with the appropriate masking bit
    poke(match, peek(match) & ~mask)
  else:
    poke(match, peek(match) | mask)

  if isEnabled:  
    # Poke P0MASK or P1MASK (register) with the appropriate masking bit
    poke(register, peek(register) | mask)
  else:
    poke(register, peek(register) & ~mask)
