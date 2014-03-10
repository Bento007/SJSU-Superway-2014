# Copyright (C) 2011 Synapse Wireless, Inc.
# Subject to your agreement of the disclaimer set forth below, permission is given by Synapse Wireless, Inc. ("Synapse") to you to freely modify, redistribute or include this SNAPpy code in any program. The purpose of this code is to help you understand and learn about SNAPpy by code examples.
# BY USING ALL OR ANY PORTION OF THIS SNAPPY CODE, YOU ACCEPT AND AGREE TO THE BELOW DISCLAIMER. If you do not accept or agree to the below disclaimer, then you may not use, modify, or distribute this SNAPpy code.
# THE CODE IS PROVIDED UNDER THIS LICENSE ON AN "AS IS" BASIS, WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, WITHOUT LIMITATION, WARRANTIES THAT THE COVERED CODE IS FREE OF DEFECTS, MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE OR NON-INFRINGING. THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE COVERED CODE IS WITH YOU. SHOULD ANY COVERED CODE PROVE DEFECTIVE IN ANY RESPECT, YOU (NOT THE INITIAL DEVELOPER OR ANY OTHER CONTRIBUTOR) ASSUME THE COST OF ANY NECESSARY SERVICING, REPAIR OR CORRECTION. UNDER NO CIRCUMSTANCES WILL SYNAPSE BE LIABLE TO YOU, OR ANY OTHER PERSON OR ENTITY, FOR ANY LOSS OF USE, REVENUE OR PROFIT, LOST OR DAMAGED DATA, OR OTHER COMMERCIAL OR ECONOMIC LOSS OR FOR ANY DAMAGES WHATSOEVER RELATED TO YOUR USE OR RELIANCE UPON THE SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES OR IF SUCH DAMAGES ARE FORESEEABLE. THIS DISCLAIMER OF WARRANTY AND LIABILITY CONSTITUTES AN ESSENTIAL PART OF THIS LICENSE. NO USE OF ANY COVERED CODE IS AUTHORIZED HEREUNDER EXCEPT UNDER THIS DISCLAIMER.

"""Utility functions to access the real-time hardware timer on Si1000-based SNAP
   modules. Timers 1 and 2 are used by the system. Timers 0 and 3 cannot be
   sufficiently manipulated without affecting timers 1 and 2. This, then, leaves
   the real-time timer, which is somewhat more complicated to access. The RT
   timer ticks at 32,768 Hz."""

RTC0CN = 0x04
RTC0ADR = -0xAC
RTC0DAT = -0xAD
RTC0KEY = -0xAE

def initHwTmr():
  """Initialize the hardware timer, and reset count"""
  # Initializing the hardware timer is cumbersome. Sleep already does this, so we piggyback.
  sleep(1, 1)
  resetTmr()

def resetTmr():
  """Reset hardware timer count to zero (timer runs continuously)"""
  tmrUnlock()
  poke(RTC0ADR, 0)
  poke(RTC0DAT, 0)
  poke(RTC0ADR, 1)
  poke(RTC0DAT, 0)
  poke(RTC0ADR, RTC0CN)
  poke(RTC0DAT, 0xD2)
  tmrLock()
  
def tmrMs():
  """Return the elapsed timer count in milliseconds."""
  return (tmrCount() / 328) * 10

def tmrCount():
  """Return the elapsed timer count in raw 'ticks' at 32,768 Hz"""
  tmrUnlock()
  poke(RTC0ADR, RTC0CN)
  poke(RTC0DAT, 0xD1)
  poke(RTC0ADR, 0x80)
  lowByte = peek(RTC0DAT)
  poke(RTC0ADR, 0x81)
  highByte = peek(RTC0DAT)
  tmrLock()
  return (highByte << 8) | lowByte

def tmrUnlock():
  poke(RTC0KEY, 0xA5)
  poke(RTC0KEY, 0xF1)

def tmrLock():
  poke(RTC0KEY, 0)