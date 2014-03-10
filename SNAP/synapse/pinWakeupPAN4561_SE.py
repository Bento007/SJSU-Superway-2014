# Copyright (C) 2011 Synapse Wireless, Inc.
# Subject to your agreement of the disclaimer set forth below, permission is given by Synapse Wireless, Inc. ("Synapse") to you to freely modify, redistribute or include this SNAPpy code in any program. The purpose of this code is to help you understand and learn about SNAPpy by code examples.
# BY USING ALL OR ANY PORTION OF THIS SNAPPY CODE, YOU ACCEPT AND AGREE TO THE BELOW DISCLAIMER. If you do not accept or agree to the below disclaimer, then you may not use, modify, or distribute this SNAPpy code.
# THE CODE IS PROVIDED UNDER THIS LICENSE ON AN "AS IS" BASIS, WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, WITHOUT LIMITATION, WARRANTIES THAT THE COVERED CODE IS FREE OF DEFECTS, MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE OR NON-INFRINGING. THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE COVERED CODE IS WITH YOU. SHOULD ANY COVERED CODE PROVE DEFECTIVE IN ANY RESPECT, YOU (NOT THE INITIAL DEVELOPER OR ANY OTHER CONTRIBUTOR) ASSUME THE COST OF ANY NECESSARY SERVICING, REPAIR OR CORRECTION. UNDER NO CIRCUMSTANCES WILL SYNAPSE BE LIABLE TO YOU, OR ANY OTHER PERSON OR ENTITY, FOR ANY LOSS OF USE, REVENUE OR PROFIT, LOST OR DAMAGED DATA, OR OTHER COMMERCIAL OR ECONOMIC LOSS OR FOR ANY DAMAGES WHATSOEVER RELATED TO YOUR USE OR RELIANCE UPON THE SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES OR IF SUCH DAMAGES ARE FORESEEABLE. THIS DISCLAIMER OF WARRANTY AND LIABILITY CONSTITUTES AN ESSENTIAL PART OF THIS LICENSE. NO USE OF ANY COVERED CODE IS AUTHORIZED HEREUNDER EXCEPT UNDER THIS DISCLAIMER.

"""This SNAPpy script implements "wakeup on GPIO" capability for the PAN4561 based SNAP Engine.

   Note that we support pins beyond GPIO_18, as a convenience to the user.
   Although not brought out as GPIO, these "extended GPIO" pins ARE brought out to test points.
"""

from synapse.PAN4561_SE import *
# Note the use of one routine (with multiple parameters) to save memory

def wakeupOn(pin, isEnabled, polarityIsRising): # default to enabling, falling edge
  """Controls "wakeup" feature for the specified GPIO (6,9,10,19,20,21,22, or 23)"""
  canSetPolarity = True # default to the most common case
  if pin==GPIO_19:
    mask = 0x01 # KBI0
    canSetPolarity = False
  elif pin==GPIO_20:
    mask = 0x02 # KBI1
    canSetPolarity = False
  elif pin==GPIO_21:
    mask = 0x04 # KBI2
    canSetPolarity = False
  elif pin==GPIO_22:
    mask = 0x08 # KBI3
    canSetPolarity = False
  elif pin==GPIO_23:
    mask = 0x10 # KBI4
  elif pin==GPIO_6:
    mask = 0x20 # KBI5
  elif pin==GPIO_9:
    mask = 0x40 # KBI6 
  elif pin==GPIO_10:
    mask = 0x80 # KBI7
  else:
    return # only pins 6,9,10,19,20,21,22, and 23 can do this trick

  if canSetPolarity: # only pins 23,6,9 and 10 can do this trick
    if polarityIsRising:
      poke(0x16, peek(0x16) | mask) # 0x16 is KBI1SC
    else:
      poke(0x16, peek(0x16) & ~mask)

  if isEnabled:  
    poke(0x17, peek(0x17) | mask) # 0x17 is KBI1PE
  else:
    poke(0x17, peek(0x17) & ~mask)
