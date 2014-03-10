# Copyright (C) 2011 Synapse Wireless, Inc.
# Subject to your agreement of the disclaimer set forth below, permission is given by Synapse Wireless, Inc. ("Synapse") to you to freely modify, redistribute or include this SNAPpy code in any program. The purpose of this code is to help you understand and learn about SNAPpy by code examples.
# BY USING ALL OR ANY PORTION OF THIS SNAPPY CODE, YOU ACCEPT AND AGREE TO THE BELOW DISCLAIMER. If you do not accept or agree to the below disclaimer, then you may not use, modify, or distribute this SNAPpy code.
# THE CODE IS PROVIDED UNDER THIS LICENSE ON AN "AS IS" BASIS, WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, WITHOUT LIMITATION, WARRANTIES THAT THE COVERED CODE IS FREE OF DEFECTS, MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE OR NON-INFRINGING. THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE COVERED CODE IS WITH YOU. SHOULD ANY COVERED CODE PROVE DEFECTIVE IN ANY RESPECT, YOU (NOT THE INITIAL DEVELOPER OR ANY OTHER CONTRIBUTOR) ASSUME THE COST OF ANY NECESSARY SERVICING, REPAIR OR CORRECTION. UNDER NO CIRCUMSTANCES WILL SYNAPSE BE LIABLE TO YOU, OR ANY OTHER PERSON OR ENTITY, FOR ANY LOSS OF USE, REVENUE OR PROFIT, LOST OR DAMAGED DATA, OR OTHER COMMERCIAL OR ECONOMIC LOSS OR FOR ANY DAMAGES WHATSOEVER RELATED TO YOUR USE OR RELIANCE UPON THE SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES OR IF SUCH DAMAGES ARE FORESEEABLE. THIS DISCLAIMER OF WARRANTY AND LIABILITY CONSTITUTES AN ESSENTIAL PART OF THIS LICENSE. NO USE OF ANY COVERED CODE IS AUTHORIZED HEREUNDER EXCEPT UNDER THIS DISCLAIMER.

"""GPIO-related assignments for the SM700 surface-mount chip based on the SFSM-201.

   This file is typically included by using the SNAP 2.x 'platforms' feature.
   However, this file can also be included directly
"""


# GPIO enums to be used in example scripts, mapped to SNAPpy IO numbers as supported by the SNAP core firmware
GPIO_0	=  0
GPIO_1	=  1
GPIO_2	=  2
GPIO_3	=  3
GPIO_4	=  4
GPIO_5	=  5
GPIO_6	=  6
GPIO_7	=  7
GPIO_8	=  8
GPIO_9	=  9
GPIO_10	=  10
GPIO_11	=  11
GPIO_12	=  12
GPIO_13	=  13
GPIO_14	=  14
GPIO_15	=  15
GPIO_16	=  16
GPIO_17	=  17
GPIO_18	=  18
GPIO_19	=  19
GPIO_20	=  20
GPIO_21	=  21
GPIO_22	=  22
GPIO_23	=  23
GPIO_24	=  24
GPIO_25	=  25
GPIO_26	=  26
GPIO_27	=  27
GPIO_28	=  28
GPIO_29	=  29
GPIO_30	=  30
GPIO_31	=  31
GPIO_32	=  32
GPIO_33	=  33
GPIO_34	=  34
GPIO_35	=  35
GPIO_36	=  36
GPIO_37	=  37
GPIO_38	=  38
GPIO_39	=  39
GPIO_40	=  40
GPIO_41	=  41
GPIO_42	=  42
GPIO_43	=  43
GPIO_44	=  44
GPIO_45	=  45
GPIO_46	=  46
GPIO_47	=  47
GPIO_48	=  48
GPIO_49	=  49


# List of pin assignments that can be iterated over in GPIO-order (i.e. GPIO_0 is 0, GPIO_1 is 1, etc...)
GPIO_TO_IO_LIST = (0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49)

# List of pin assignments that can be iterated over in pin/IO-order (i.e. IO 0 is GPIO_0)
IO_TO_GPIO_LIST = (0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49)

# List of ADC pin assignments that can be iterated over in GPIO-order (i.e., to read the ADC on GPIO_11, use pin 7, etc.)
GPIO_TO_ADC_LIST = (-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,1,2,3,4,5,6,7)