# Copyright (C) 2011 Synapse Wireless, Inc.
# Subject to your agreement of the disclaimer set forth below, permission is given by Synapse Wireless, Inc. ("Synapse") to you to freely modify, redistribute or include this SNAPpy code in any program. The purpose of this code is to help you understand and learn about SNAPpy by code examples.
# BY USING ALL OR ANY PORTION OF THIS SNAPPY CODE, YOU ACCEPT AND AGREE TO THE BELOW DISCLAIMER. If you do not accept or agree to the below disclaimer, then you may not use, modify, or distribute this SNAPpy code.
# THE CODE IS PROVIDED UNDER THIS LICENSE ON AN "AS IS" BASIS, WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, WITHOUT LIMITATION, WARRANTIES THAT THE COVERED CODE IS FREE OF DEFECTS, MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE OR NON-INFRINGING. THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE COVERED CODE IS WITH YOU. SHOULD ANY COVERED CODE PROVE DEFECTIVE IN ANY RESPECT, YOU (NOT THE INITIAL DEVELOPER OR ANY OTHER CONTRIBUTOR) ASSUME THE COST OF ANY NECESSARY SERVICING, REPAIR OR CORRECTION. UNDER NO CIRCUMSTANCES WILL SYNAPSE BE LIABLE TO YOU, OR ANY OTHER PERSON OR ENTITY, FOR ANY LOSS OF USE, REVENUE OR PROFIT, LOST OR DAMAGED DATA, OR OTHER COMMERCIAL OR ECONOMIC LOSS OR FOR ANY DAMAGES WHATSOEVER RELATED TO YOUR USE OR RELIANCE UPON THE SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES OR IF SUCH DAMAGES ARE FORESEEABLE. THIS DISCLAIMER OF WARRANTY AND LIABILITY CONSTITUTES AN ESSENTIAL PART OF THIS LICENSE. NO USE OF ANY COVERED CODE IS AUTHORIZED HEREUNDER EXCEPT UNDER THIS DISCLAIMER.

""" This include file is designed to be an abstraction layer for individual include
    files specific to each SNAP Engine implementation.

    Ex: the PAN4555 and PAN4561 modules both use the same core hardware (MC1321x)
    and thus the same SNAP build. However, each has a unique set of GPIO assignments.
    These designations can be found in the specific include file listed below.

    NOTE - The "SE" in _SE stands for SNAP Engine.
"""
from synapse.snapsys import *

if platform == "RFEngine" or platform == 'RF100':
    from synapse.RF100 import *

elif platform == "PAN4561_SE":
    from synapse.PAN4561_SE import *

elif platform == "PAN4555_SE":
    from synapse.PAN4555_SE import *

elif platform == "RF300" or platform == "RF301" or platform == 'Si100x':
    from synapse.RF300 import *

elif platform == "ZIC2410_SE":
    from synapse.ZIC2410_SE import *

elif platform == "RF200":
    from synapse.RF200 import *

elif platform == "SM700":
    from synapse.SM700 import *

elif platform == "ZIC2410":
    pass

else:
    from synapse.RF100 import *