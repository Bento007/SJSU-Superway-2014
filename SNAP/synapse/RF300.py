# Copyright (C) 2011 Synapse Wireless, Inc.
# Subject to your agreement of the disclaimer set forth below, permission is given by Synapse Wireless, Inc. ("Synapse") to you to freely modify, redistribute or include this SNAPpy code in any program. The purpose of this code is to help you understand and learn about SNAPpy by code examples.
# BY USING ALL OR ANY PORTION OF THIS SNAPPY CODE, YOU ACCEPT AND AGREE TO THE BELOW DISCLAIMER. If you do not accept or agree to the below disclaimer, then you may not use, modify, or distribute this SNAPpy code.
# THE CODE IS PROVIDED UNDER THIS LICENSE ON AN "AS IS" BASIS, WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, WITHOUT LIMITATION, WARRANTIES THAT THE COVERED CODE IS FREE OF DEFECTS, MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE OR NON-INFRINGING. THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE COVERED CODE IS WITH YOU. SHOULD ANY COVERED CODE PROVE DEFECTIVE IN ANY RESPECT, YOU (NOT THE INITIAL DEVELOPER OR ANY OTHER CONTRIBUTOR) ASSUME THE COST OF ANY NECESSARY SERVICING, REPAIR OR CORRECTION. UNDER NO CIRCUMSTANCES WILL SYNAPSE BE LIABLE TO YOU, OR ANY OTHER PERSON OR ENTITY, FOR ANY LOSS OF USE, REVENUE OR PROFIT, LOST OR DAMAGED DATA, OR OTHER COMMERCIAL OR ECONOMIC LOSS OR FOR ANY DAMAGES WHATSOEVER RELATED TO YOUR USE OR RELIANCE UPON THE SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES OR IF SUCH DAMAGES ARE FORESEEABLE. THIS DISCLAIMER OF WARRANTY AND LIABILITY CONSTITUTES AN ESSENTIAL PART OF THIS LICENSE. NO USE OF ANY COVERED CODE IS AUTHORIZED HEREUNDER EXCEPT UNDER THIS DISCLAIMER.

"""GPIO-related assignments for the SNAP Engine based on the Silicon Labs Si1000 chip. 
   If you are using the Si1000 chip directly (no SNAP Engine), you do not need this file.

   This file is typically included by using the SNAP 2.x 'platforms' feature.
   However, this file can also be included directly
"""


# GPIO enums to be used in example scripts, mapped to SNAPpy IO numbers as supported by the SNAP core firmware
GPIO_0	= 10
GPIO_1	= 11
GPIO_2	= 12
GPIO_3	= 13
GPIO_4	= 14
GPIO_5	= 15
GPIO_6	= 0
GPIO_7	= 4
GPIO_8	= 3
GPIO_9	= 2
GPIO_10	= 1
GPIO_11	= 9
GPIO_12	= 8
GPIO_13	= 6
GPIO_14	= 7
GPIO_15	= 5
GPIO_16	= 16
GPIO_17	= 17
GPIO_18	= 18

# ADC enums to be used in example scripts, mapped to GPIOs. Read ADC_0 to get an ADC reading from pin GPIO_0.
ADC_0 = 17
ADC_1 = 18
ADC_2 = 19
ADC_3 = 20
ADC_4 = 21
ADC_5 = 22
ADC_6 = 0
ADC_7 = 5
ADC_8 = 4
ADC_9 = 3
ADC_10 = 2
ADC_11 = 16
ADC_12 = 15
ADC_13 = 13
ADC_14 = 14
ADC_15 = 6


# List of pin assignments that can be iterated over in GPIO-order (i.e. GPIO_0 is 10, GPIO1 is 11, etc...)
GPIO_TO_IO_LIST = (10,11,12,13,14,15,0,4,3,2,1,9,8,6,7,5,16,17,18)

# List of pin assignments that can be iterated over in pin/IO-order (i.e. IO 0 is GPIO_6)
IO_TO_GPIO_LIST = (6,10,9,8,7,15,13,14,12,11,0,1,2,3,4,5,16,17,18)	

# List of ADC pin assignments that can be iterated over in GPIO-order (i.e., to read the ADC on GPIO_0, use pin 17, etc.)
GPIO_TO_ADC_LIST = (17,18,19,20,21,22,0,5,4,3,2,16,15,13,14,6)