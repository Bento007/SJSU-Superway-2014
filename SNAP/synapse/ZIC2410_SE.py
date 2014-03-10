# Copyright (C) 2011 Synapse Wireless, Inc.
# Subject to your agreement of the disclaimer set forth below, permission is given by Synapse Wireless, Inc. ("Synapse") to you to freely modify, redistribute or include this SNAPpy code in any program. The purpose of this code is to help you understand and learn about SNAPpy by code examples.
# BY USING ALL OR ANY PORTION OF THIS SNAPPY CODE, YOU ACCEPT AND AGREE TO THE BELOW DISCLAIMER. If you do not accept or agree to the below disclaimer, then you may not use, modify, or distribute this SNAPpy code.
# THE CODE IS PROVIDED UNDER THIS LICENSE ON AN "AS IS" BASIS, WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, WITHOUT LIMITATION, WARRANTIES THAT THE COVERED CODE IS FREE OF DEFECTS, MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE OR NON-INFRINGING. THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE COVERED CODE IS WITH YOU. SHOULD ANY COVERED CODE PROVE DEFECTIVE IN ANY RESPECT, YOU (NOT THE INITIAL DEVELOPER OR ANY OTHER CONTRIBUTOR) ASSUME THE COST OF ANY NECESSARY SERVICING, REPAIR OR CORRECTION. UNDER NO CIRCUMSTANCES WILL SYNAPSE BE LIABLE TO YOU, OR ANY OTHER PERSON OR ENTITY, FOR ANY LOSS OF USE, REVENUE OR PROFIT, LOST OR DAMAGED DATA, OR OTHER COMMERCIAL OR ECONOMIC LOSS OR FOR ANY DAMAGES WHATSOEVER RELATED TO YOUR USE OR RELIANCE UPON THE SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES OR IF SUCH DAMAGES ARE FORESEEABLE. THIS DISCLAIMER OF WARRANTY AND LIABILITY CONSTITUTES AN ESSENTIAL PART OF THIS LICENSE. NO USE OF ANY COVERED CODE IS AUTHORIZED HEREUNDER EXCEPT UNDER THIS DISCLAIMER.

"""GPIO-related assignments for the SNAP Engine based on the CEL ZIC2410 chip. 
   If you are using the ZIC2410 module directly (no SNAP Engine), you do not need this file.

   This file is typically included by using the SNAP 2.x 'platforms' feature.
   However, this file can also be included directly
"""


# GPIO enums to be used in example scripts, mapped to SNAPpy IO numbers as supported by the SNAP core firmware
GPIO_0	= 11
GPIO_1	= 18
GPIO_2	= 19
GPIO_3	= 16
GPIO_4	= 17
GPIO_5	= 21
GPIO_6	= 20
GPIO_7	= 8
GPIO_8	= 9
GPIO_9	= 23
GPIO_10	= 22
GPIO_11	= 7
GPIO_12	= 6
GPIO_13	= 5
GPIO_14	= 4
GPIO_15	= 3
GPIO_16	= 2
GPIO_17	= 1
GPIO_18	= 0


# List of pin assignments that can be iterated over in GPIO-order (i.e. GPIO_0 is 11, GPIO1 is 18, etc...)
GPIO_TO_IO_LIST = (11,18,19,16,17,21,20,8,9,23,22,7,6,5,4,3,2,1,0)

# List of pin assignments that can be iterated over in pin/IO-order (i.e. IO 0 is GPIO_6)
IO_TO_GPIO_LIST = (18,17,16,15,14,13,12,11,7,8,-1,0,-1,-1,-1,-1,3,4,1,2,6,5,10,9)	
