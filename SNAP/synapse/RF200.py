# Copyright (C) 2011 Synapse Wireless, Inc.
# Subject to your agreement of the disclaimer set forth below, permission is given by Synapse Wireless, Inc. ("Synapse") to you to freely modify, redistribute or include this SNAPpy code in any program. The purpose of this code is to help you understand and learn about SNAPpy by code examples.
# BY USING ALL OR ANY PORTION OF THIS SNAPPY CODE, YOU ACCEPT AND AGREE TO THE BELOW DISCLAIMER. If you do not accept or agree to the below disclaimer, then you may not use, modify, or distribute this SNAPpy code.
# THE CODE IS PROVIDED UNDER THIS LICENSE ON AN "AS IS" BASIS, WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, WITHOUT LIMITATION, WARRANTIES THAT THE COVERED CODE IS FREE OF DEFECTS, MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE OR NON-INFRINGING. THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE COVERED CODE IS WITH YOU. SHOULD ANY COVERED CODE PROVE DEFECTIVE IN ANY RESPECT, YOU (NOT THE INITIAL DEVELOPER OR ANY OTHER CONTRIBUTOR) ASSUME THE COST OF ANY NECESSARY SERVICING, REPAIR OR CORRECTION. UNDER NO CIRCUMSTANCES WILL SYNAPSE BE LIABLE TO YOU, OR ANY OTHER PERSON OR ENTITY, FOR ANY LOSS OF USE, REVENUE OR PROFIT, LOST OR DAMAGED DATA, OR OTHER COMMERCIAL OR ECONOMIC LOSS OR FOR ANY DAMAGES WHATSOEVER RELATED TO YOUR USE OR RELIANCE UPON THE SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES OR IF SUCH DAMAGES ARE FORESEEABLE. THIS DISCLAIMER OF WARRANTY AND LIABILITY CONSTITUTES AN ESSENTIAL PART OF THIS LICENSE. NO USE OF ANY COVERED CODE IS AUTHORIZED HEREUNDER EXCEPT UNDER THIS DISCLAIMER.

"""GPIO-related assignments for the SNAP Engine based on the ATMEL AT128RFA1 chip.
   If you are using the AT128RFA1 chip directly (no SNAP Engine), you do not need this file.

   This file is typically included by using the SNAP 2.x 'platforms' feature.
   However, this file can also be included directly
"""


# GPIO enums to be used in example scripts, mapped to SNAPpy IO numbers as supported by the SNAP core firmware
GPIO_0	= 7
GPIO_1	= 6
GPIO_2	= 5
GPIO_3	= 16
GPIO_4	= 17
GPIO_5	= 20
GPIO_6	= 21
GPIO_7	= 10
GPIO_8	= 11
GPIO_9	= 12
GPIO_10	= 23
GPIO_11	= 24
GPIO_12	= 25
GPIO_13	= 26
GPIO_14	= 18 # with PA else 27
GPIO_15	= 28
GPIO_16	= 29
GPIO_17	= 30
GPIO_18	= 31
GPIO_19 = 19

# List of pin assignments that can be iterated over in GPIO-order (i.e. GPIO_0 is 7, GPIO1 is 6, etc...)
GPIO_TO_IO_LIST = (7,6,5,16,17,20,21,10,11,12,23,24,25,26,18,28,29,30,31,19)

# List of pin assignments that can be iterated over in pin/IO-order (i.e. IO 5 is GPIO_2)
IO_TO_GPIO_LIST = (-1,-1,-1,-1,-1,2,1,0,-1,-1,7,8,9,-1,-1,-1,3,4,14,19,5,6,-1,10,11,12,13,-1,15,16,17,18,-1,-1,-1,-1,-1,-1)	

# List of ADC pin assignments that can be iterated over in GPIO-order (i.e., to read the ADC on GPIO_0, use pin 17, etc.)
GPIO_TO_ADC_LIST = (-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,1,2,-1,4,5,6,7,-1)
