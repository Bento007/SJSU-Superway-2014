# Copyright (C) 2011 Synapse Wireless, Inc.
# Subject to your agreement of the disclaimer set forth below, permission is given by Synapse Wireless, Inc. ("Synapse") to you to freely modify, redistribute or include this SNAPpy code in any program. The purpose of this code is to help you understand and learn about SNAPpy by code examples.
# BY USING ALL OR ANY PORTION OF THIS SNAPPY CODE, YOU ACCEPT AND AGREE TO THE BELOW DISCLAIMER. If you do not accept or agree to the below disclaimer, then you may not use, modify, or distribute this SNAPpy code.
# THE CODE IS PROVIDED UNDER THIS LICENSE ON AN "AS IS" BASIS, WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, WITHOUT LIMITATION, WARRANTIES THAT THE COVERED CODE IS FREE OF DEFECTS, MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE OR NON-INFRINGING. THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE COVERED CODE IS WITH YOU. SHOULD ANY COVERED CODE PROVE DEFECTIVE IN ANY RESPECT, YOU (NOT THE INITIAL DEVELOPER OR ANY OTHER CONTRIBUTOR) ASSUME THE COST OF ANY NECESSARY SERVICING, REPAIR OR CORRECTION. UNDER NO CIRCUMSTANCES WILL SYNAPSE BE LIABLE TO YOU, OR ANY OTHER PERSON OR ENTITY, FOR ANY LOSS OF USE, REVENUE OR PROFIT, LOST OR DAMAGED DATA, OR OTHER COMMERCIAL OR ECONOMIC LOSS OR FOR ANY DAMAGES WHATSOEVER RELATED TO YOUR USE OR RELIANCE UPON THE SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES OR IF SUCH DAMAGES ARE FORESEEABLE. THIS DISCLAIMER OF WARRANTY AND LIABILITY CONSTITUTES AN ESSENTIAL PART OF THIS LICENSE. NO USE OF ANY COVERED CODE IS AUTHORIZED HEREUNDER EXCEPT UNDER THIS DISCLAIMER.

"""
An example script showing how to adapt to a mix of nodes that DO and DO NOT have a particular built-in.

In this example the built-in is the seven-segment display helper, setSegments().

Not all SNAP platforms have enough FLASH memory space to support this built-in, for example the Si1000.

By calling an alternate routine SetSegments(), scripts can automatically use the built-in when
it is available, but use an alternate (SNAPpy) implementation when it is not.

To use:

1) from synapse.sevenSegment import *
2) Invoke updateSevenSegmentDisplay() every 10 milliseconds (less often will "flicker")
3) Call SetSegments() instead of calling setSegments() directly.
"""
from synapse.platforms import *

def testPlatform():
    return platform


if (platform == 'RFEngine') or platform == 'RF100' or (platform == 'PAN4555_SE') or (platform == 'PAN4561_SE') or (platform == 'ZIC2410_SE'):
    def SetSegments(segments):
        # These platforms have built-in support for the seven-segment display
        setSegments(segments)

    def updateSevenSegmentDisplay():
        # placeholder routine - on these platforms the display refreshes automatically
        pass

else:
    # Some platforms do not have the FLASH space to support setSegments() in core
    # Here is an optimized SNAPpy equivalent.

    if platform == 'RF300' or platform == 'RF301':
        DISPLAY_CLOCK = GPIO_4
        DISPLAY_DATA = GPIO_3
    else:
        DISPLAY_CLOCK = GPIO_13
        DISPLAY_DATA = GPIO_14

    displayActive = False
    segmentValues = 0x0000

    def initSevenSegmentDriver():
        """This is called automatically the first time you invoke SetSegments()"""
        global segmentValues, topDigit

        segmentValues = 0x0000
        topDigit = True

        writePin(DISPLAY_CLOCK, False)
        setPinDir(DISPLAY_CLOCK, True)
        writePin(DISPLAY_DATA, False)
        setPinDir(DISPLAY_DATA, True)

    def SetSegments(segments):
        """This mimics the setSegments() built-in, for platforms that do not support it in core"""
        global segmentValues, displayActive
        global a,b,c,d,e,f,g,A,B,C,D,E,F,G

        if displayActive == False:
            initSevenSegmentDriver()
            displayActive = True

        segmentValues = segments

        # Precalculate segment values for speed
        a = not (segmentValues & 0x0001)
        b = not (segmentValues & 0x0002)
        c = not (segmentValues & 0x0004)
        d = not (segmentValues & 0x0008)
        e = not (segmentValues & 0x0010)
        f = not (segmentValues & 0x0020)
        g = not (segmentValues & 0x0040)

        A = not (segmentValues & 0x0100)
        B = not (segmentValues & 0x0200)
        C = not (segmentValues & 0x0400)
        D = not (segmentValues & 0x0800)
        E = not (segmentValues & 0x1000)
        F = not (segmentValues & 0x2000)
        G = not (segmentValues & 0x4000)


    def updateSevenSegmentDisplay():
        """This needs to get called every 10 milliseconds"""
        global topDigit

        if segmentValues != 0x0000:

            #
            # To understand this code, you need to know that DISPLAY_CLOCK also controls display ON/OFF,
            # and DISPLAY_DATA also controls RIGHT DIGIT/LEFT DIGIT

            writePin(DISPLAY_CLOCK, False) # keep display off as much as possible while we update it
            if topDigit:
                writePin(DISPLAY_DATA, G)
                pulsePin(DISPLAY_CLOCK, -1, True)

                writePin(DISPLAY_DATA, F)
                pulsePin(DISPLAY_CLOCK, -1, True)

                writePin(DISPLAY_DATA, E)
                pulsePin(DISPLAY_CLOCK, -1, True)

                writePin(DISPLAY_DATA, D)
                pulsePin(DISPLAY_CLOCK, -1, True)

                writePin(DISPLAY_DATA, C)
                pulsePin(DISPLAY_CLOCK, -1, True)

                writePin(DISPLAY_DATA, B)
                pulsePin(DISPLAY_CLOCK, -1, True)

                writePin(DISPLAY_DATA, A)
            else:
                writePin(DISPLAY_DATA, g)
                pulsePin(DISPLAY_CLOCK, -1, True)

                writePin(DISPLAY_DATA, f)
                pulsePin(DISPLAY_CLOCK, -1, True)

                writePin(DISPLAY_DATA, e)
                pulsePin(DISPLAY_CLOCK, -1, True)

                writePin(DISPLAY_DATA, d)
                pulsePin(DISPLAY_CLOCK, -1, True)

                writePin(DISPLAY_DATA, c)
                pulsePin(DISPLAY_CLOCK, -1, True)

                writePin(DISPLAY_DATA, b)
                pulsePin(DISPLAY_CLOCK, -1, True)

                writePin(DISPLAY_DATA, a)

            writePin(DISPLAY_CLOCK, True) # final rising edge, plus turn on display

            writePin(DISPLAY_DATA, topDigit) # put digit on left or right
            topDigit = not topDigit
        else:
            # Nothing to display anyway, just turn it off!
            writePin(DISPLAY_CLOCK, False)
