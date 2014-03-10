# Copyright (C) 2011 Synapse Wireless, Inc.
# Subject to your agreement of the disclaimer set forth below, permission is given by Synapse Wireless, Inc. ("Synapse") to you to freely modify, redistribute or include this SNAPpy code in any program. The purpose of this code is to help you understand and learn about SNAPpy by code examples.
# BY USING ALL OR ANY PORTION OF THIS SNAPPY CODE, YOU ACCEPT AND AGREE TO THE BELOW DISCLAIMER. If you do not accept or agree to the below disclaimer, then you may not use, modify, or distribute this SNAPpy code.
# THE CODE IS PROVIDED UNDER THIS LICENSE ON AN "AS IS" BASIS, WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, WITHOUT LIMITATION, WARRANTIES THAT THE COVERED CODE IS FREE OF DEFECTS, MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE OR NON-INFRINGING. THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE COVERED CODE IS WITH YOU. SHOULD ANY COVERED CODE PROVE DEFECTIVE IN ANY RESPECT, YOU (NOT THE INITIAL DEVELOPER OR ANY OTHER CONTRIBUTOR) ASSUME THE COST OF ANY NECESSARY SERVICING, REPAIR OR CORRECTION. UNDER NO CIRCUMSTANCES WILL SYNAPSE BE LIABLE TO YOU, OR ANY OTHER PERSON OR ENTITY, FOR ANY LOSS OF USE, REVENUE OR PROFIT, LOST OR DAMAGED DATA, OR OTHER COMMERCIAL OR ECONOMIC LOSS OR FOR ANY DAMAGES WHATSOEVER RELATED TO YOUR USE OR RELIANCE UPON THE SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES OR IF SUCH DAMAGES ARE FORESEEABLE. THIS DISCLAIMER OF WARRANTY AND LIABILITY CONSTITUTES AN ESSENTIAL PART OF THIS LICENSE. NO USE OF ANY COVERED CODE IS AUTHORIZED HEREUNDER EXCEPT UNDER THIS DISCLAIMER.

"""GPIO-related assignments for the SNAP Engine based on the PAN4561 module. 
   If you are using the PAN4561 module directly (no SNAP Engine), you do not need this file.

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
# The SNAP Engine form factor only brings out 19 of the 33 total pins as GPIOs.
# Since many of the leftover pins are accessible via test points on the board,
# the following definitions have been added as a convenience.
GPIO_19	=  0
GPIO_20	=  1
GPIO_21	=  2
GPIO_22	=  3
GPIO_23	=  4
GPIO_24	=  11
GPIO_25	=  12
GPIO_26	=  13
GPIO_27	=  14
GPIO_28 =  20
GPIO_29	=  22
GPIO_30	=  23
GPIO_31	=  26
GPIO_32	=  28

# List of pin assignments that can be iterated over in GPIO-order (i.e. GPIO_0 is 24, GPIO1 is 19, etc...)
GPIO_TO_IO_LIST = (24,19,18,30,29,21,5,17,16,6,7,15,31,32,27,25,10,9,8,0,1,2,3,4,11,12,13,14,20,22,23,26,28)

# List of pin assignments that can be iterated over in pin/IO-order (i.e. IO 0 is GPIO_19)
IO_TO_GPIO_LIST	= (19,20,21,22,23,6,9,10,18,17,16,24,25,26,27,11,8,7,2,1,28,5,28,29,0,15,30,14,31,4,3,12,13)	

# List of ADC pin assignments that can be iterated over in GPIO-order (i.e., to read the ADC on GPIO_18, read ADC0, etc.)
GPIO_TO_ADC_LIST = (-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,7,-1,-1,-1,-1,2,1,0,-1,-1,-1,-1,-1,3,4,5,6)