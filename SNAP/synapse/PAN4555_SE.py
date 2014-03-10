# Copyright (C) 2011 Synapse Wireless, Inc.
# Subject to your agreement of the disclaimer set forth below, permission is given by Synapse Wireless, Inc. ("Synapse") to you to freely modify, redistribute or include this SNAPpy code in any program. The purpose of this code is to help you understand and learn about SNAPpy by code examples.
# BY USING ALL OR ANY PORTION OF THIS SNAPPY CODE, YOU ACCEPT AND AGREE TO THE BELOW DISCLAIMER. If you do not accept or agree to the below disclaimer, then you may not use, modify, or distribute this SNAPpy code.
# THE CODE IS PROVIDED UNDER THIS LICENSE ON AN "AS IS" BASIS, WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, WITHOUT LIMITATION, WARRANTIES THAT THE COVERED CODE IS FREE OF DEFECTS, MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE OR NON-INFRINGING. THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE COVERED CODE IS WITH YOU. SHOULD ANY COVERED CODE PROVE DEFECTIVE IN ANY RESPECT, YOU (NOT THE INITIAL DEVELOPER OR ANY OTHER CONTRIBUTOR) ASSUME THE COST OF ANY NECESSARY SERVICING, REPAIR OR CORRECTION. UNDER NO CIRCUMSTANCES WILL SYNAPSE BE LIABLE TO YOU, OR ANY OTHER PERSON OR ENTITY, FOR ANY LOSS OF USE, REVENUE OR PROFIT, LOST OR DAMAGED DATA, OR OTHER COMMERCIAL OR ECONOMIC LOSS OR FOR ANY DAMAGES WHATSOEVER RELATED TO YOUR USE OR RELIANCE UPON THE SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES OR IF SUCH DAMAGES ARE FORESEEABLE. THIS DISCLAIMER OF WARRANTY AND LIABILITY CONSTITUTES AN ESSENTIAL PART OF THIS LICENSE. NO USE OF ANY COVERED CODE IS AUTHORIZED HEREUNDER EXCEPT UNDER THIS DISCLAIMER.

"""GPIO-related assignments for the SNAP Engine based on the PAN4555 module. 
   If you are using the PAN4555 module directly (no SNAP Engine), you do not need this file.

   This file is typically included by using the SNAP 2.2 'platforms' feature.
   However, this file can also be included directly
"""


# GPIO enums to be used in example scripts, mapped to SNAPpy IO numbers as supported by the SNAP core firmware
GPIO_0	=  24
GPIO_1	=  19
GPIO_2	=  18
GPIO_3	=  30
GPIO_4	=  29
GPIO_5	=  21
GPIO_6	=  5
GPIO_7	=  17
GPIO_8	=  16
GPIO_9	=  6
GPIO_10	=  7
GPIO_11	=  15
GPIO_12	=  31
GPIO_13	=  32
GPIO_14	=  27
GPIO_15	=  25
GPIO_16	=  10
GPIO_17	=  9
GPIO_18	=  8

# ADC enums to be used in example scripts, mapped to GPIOs. Read ADC_11 to get an ADC reading from pin GPIO_11.
ADC_11 = 7
ADC_16 = 2
ADC_17 = 1
ADC_18 = 0

# List of pin assignments that can be iterated over in GPIO-order (i.e. GPIO_0 is 24, GPIO1 is 19, etc...)
GPIO_TO_IO_LIST = (24,19,18,30,29,21,5,17,16,6,7,15,31,32,27,25,10,9,8)

# List of pin assignments that can be iterated over in pin/IO-order (i.e. IO 0 is GPIO_19)
IO_TO_GPIO_LIST = (-1,-1,-1,-1,-1,6,9,10,18,17,16,-1,-1,-1,-1,11,8,7,2,1,-1,5,-1,-1,0,15,-1,14,-1,4,3,12,13)

# List of ADC pin assignments that can be iterated over in GPIO-order (i.e., to read the ADC on GPIO_0, use pin 17, etc.)
GPIO_TO_ADC_LIST = (-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,7,-1,-1,-1,-1,2,1,0)
