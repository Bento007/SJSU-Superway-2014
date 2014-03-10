# Copyright (C) 2011 Synapse Wireless, Inc.
# Subject to your agreement of the disclaimer set forth below, permission is given by Synapse Wireless, Inc. ("Synapse") to you to freely modify, redistribute or include this SNAPpy code in any program. The purpose of this code is to help you understand and learn about SNAPpy by code examples.
# BY USING ALL OR ANY PORTION OF THIS SNAPPY CODE, YOU ACCEPT AND AGREE TO THE BELOW DISCLAIMER. If you do not accept or agree to the below disclaimer, then you may not use, modify, or distribute this SNAPpy code.
# THE CODE IS PROVIDED UNDER THIS LICENSE ON AN "AS IS" BASIS, WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, WITHOUT LIMITATION, WARRANTIES THAT THE COVERED CODE IS FREE OF DEFECTS, MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE OR NON-INFRINGING. THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE COVERED CODE IS WITH YOU. SHOULD ANY COVERED CODE PROVE DEFECTIVE IN ANY RESPECT, YOU (NOT THE INITIAL DEVELOPER OR ANY OTHER CONTRIBUTOR) ASSUME THE COST OF ANY NECESSARY SERVICING, REPAIR OR CORRECTION. UNDER NO CIRCUMSTANCES WILL SYNAPSE BE LIABLE TO YOU, OR ANY OTHER PERSON OR ENTITY, FOR ANY LOSS OF USE, REVENUE OR PROFIT, LOST OR DAMAGED DATA, OR OTHER COMMERCIAL OR ECONOMIC LOSS OR FOR ANY DAMAGES WHATSOEVER RELATED TO YOUR USE OR RELIANCE UPON THE SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES OR IF SUCH DAMAGES ARE FORESEEABLE. THIS DISCLAIMER OF WARRANTY AND LIABILITY CONSTITUTES AN ESSENTIAL PART OF THIS LICENSE. NO USE OF ANY COVERED CODE IS AUTHORIZED HEREUNDER EXCEPT UNDER THIS DISCLAIMER.

"""Synapse Evaluation Board base definitions
This module provides easy initialization of the evaluation boards:
  Demonstration Boards (SN163, SN111), and
  Proto Board (SN171) and SNAPstick (SN132)

Call the function 'detectEvalBoards()' to detect and intialize one of these
boards in the default configuration.
"""

from switchboard import *
from nvparams import *
from platforms import *

# Demo board pin definitions
DEMO_LED_PIN    = GPIO_0
DEMO_BUTTON_PIN = GPIO_1
BATT_SENSE      = GPIO_3
RELAY_SET_PIN   = GPIO_16
RELAY_RESET_PIN = GPIO_17

# Proto board pin definitions
PROTO_LED_GRN_PIN = GPIO_1
PROTO_LED_YLW_PIN = GPIO_2
PROTO_BUTTON_PIN  = GPIO_5
PROTO_BUZZER_PIN  = GPIO_9

if platform == 'SS200':
    #SNAPStick pin definitions for SS200
    STICK_LED_RED_PIN = 5
    STICK_LED_GRN_PIN = 6
else:
    #SNAPStick pin definitions for other-than SS200
    STICK_LED_RED_PIN = GPIO_0
    STICK_LED_GRN_PIN = GPIO_1

# Generic pin definitions (set during board-type detection)
LED_PIN = None
BUTTON_PIN = None
deviceType = None

#
# Not all SNAP Engines support setSegments() directly.
# This script masks the difference
#
from sevenSegment import *

def detectEvalBoards():
    """Detect and initialize Evaluation Boards to default configuration"""
    global deviceType
    if isRF30x():
        defaultUart = 0 # Only one available on the RF30x series
    else:
        defaultUart = 1   # UART1 is the RS232 (DB9) connection
    
    initDeviceType()
   
    # Perform base eval-board initialization
    if deviceType == 'Buzz':
        initProtoHw()
    elif deviceType == 'Stick':
        initStickHw()
    elif deviceType == 'SS200':
        initSNAPStickHw()
    else:
        initDemoHw()
        # Initialize LED display to 00
        display2digits(0)

        if deviceType == 'Bridge':
            defaultUart = 0  # Use USB connection

    # Make the default UART port accessible to Portal
    crossConnect(DS_UART0 + defaultUart, DS_PACKET_SERIAL)
    saveNvParam(NV_DEFAULT_UART_ID, defaultUart)

def initDemoHw():
    """Initialize Demonstration Board (SN111/163) hardware peripherals"""
    global relayState, BUTTON_PIN, LED_PIN
    if isRF30x():
        initDeviceTypeFromNV()
    LED_PIN = DEMO_LED_PIN
    setPinDir(LED_PIN, True)
    BUTTON_PIN = DEMO_BUTTON_PIN
    setPinDir(BUTTON_PIN, False)
    setPinPullup(BUTTON_PIN, True)

    #Init relay
    writePin(RELAY_SET_PIN, False)
    setPinDir(RELAY_SET_PIN, True)
    writePin(RELAY_RESET_PIN, False)
    setPinDir(RELAY_RESET_PIN, True)
    relayState = None
    
def initProtoHw():
    """Initialize ProtoBoard (SN171) hardware peripherals"""
    global BUTTON_PIN, LED_PIN
    if isRF30x():
        initDeviceTypeFromNV()
    LED_PIN = PROTO_LED_YLW_PIN
    setPinDir(LED_PIN, True)
    setPinDir(PROTO_LED_GRN_PIN, True)
    BUTTON_PIN = PROTO_BUTTON_PIN
    setPinDir(BUTTON_PIN, False)
    setPinPullup(BUTTON_PIN, True)
    setPinDir(PROTO_BUZZER_PIN, True)
    writePin(PROTO_BUZZER_PIN, False)

def initSNAPStickHw():
    """Initialize SNAPStick (e.g., SS200) hardware peripherals"""
    global LED_PIN
    LED_PIN = STICK_LED_RED_PIN
    setPinDir(LED_PIN, True)
    setPinDir(STICK_LED_GRN_PIN, True)

def initStickHw():
    """Initialize SNAPStick USB board (SN132) hardware peripherals"""
    global LED_PIN
    if isRF30x():
        initDeviceTypeFromNV()
    LED_PIN = STICK_LED_RED_PIN # guaranteed only 1 LED (choose Red for this board)
    setPinDir(LED_PIN, True)
    setPinDir(STICK_LED_GRN_PIN, True)

def blinkLed(msDuration):
    """Blink LED for specified duration in milliseconds"""
    global deviceType
    if deviceType == "Stick" or deviceType == 'SS200':
        pulsePin(LED_PIN, msDuration, False)
    else:
        pulsePin(LED_PIN, msDuration, True)

def blinkLed2(msDuration):
    """Blink LED2 (if it exists) for specified duration in milliseconds"""
    global deviceType
    if deviceType == "Stick" or deviceType == 'SS200':
        pulsePin(STICK_LED_GRN_PIN, msDuration, False)
    elif deviceType == "Buzz":
        pulsePin(PROTO_LED_GRN_PIN, msDuration, True)
    else:
        blinkLed(msDuration)

def blinkLed3(msDuration):
    """Blink LED3 (if it exists) for specified duration in milliseconds"""
    global deviceType
    if deviceType == "Stick" or deviceType == 'SS200':
        pulsePin(STICK_LED_GRN_PIN, msDuration, False)
        pulsePin(STICK_LED_RED_PIN, msDuration, False)
    elif deviceType == "Buzz":
        pulsePin(PROTO_LED_GRN_PIN, msDuration, True)
        pulsePin(PROTO_LED_YLW_PIN, msDuration, True)
    else:
        blinkLed(msDuration)

def lightLed():
    """Light the first LED"""
    global deviceType
    if deviceType == "Stick" or deviceType == 'SS200':
        writePin(LED_PIN, False)
    else:
        writePin(LED_PIN, True)

def lightLed2():
    """Light LED2 (if it exists)"""
    global deviceType
    if deviceType == "Stick" or deviceType == 'SS200':
        writePin(STICK_LED_GRN_PIN, False)
    elif deviceType == "Buzz":
        writePin(PROTO_LED_GRN_PIN, True)
    else:
        writePin(LED_PIN, True)

def lightLed3():
    """Light LED3 (if itexists)"""
    global deviceType
    if deviceType == "Stick" or deviceType == 'SS200':
        writePin(STICK_LED_GRN_PIN, False)
        writePin(STICK_LED_RED_PIN, False)
    elif deviceType == "Buzz":
        writePin(PROTO_LED_GRN_PIN, True)
        writePin(PROTO_LED_YLW_PIN, True)
    else:
        writePin(LED_PIN, True)

def ledsOff():
    """Turn off all LEDs"""
    global deviceType
    if deviceType == "Stick" or deviceType == 'SS200':
        writePin(STICK_LED_GRN_PIN, True)
        writePin(STICK_LED_RED_PIN, True)
    elif deviceType == "Buzz":
        writePin(PROTO_LED_GRN_PIN, False)
        writePin(PROTO_LED_YLW_PIN, False)
    else:
        writePin(LED_PIN, False)

def setRelayState(isSet):
    """Set the relay (if present) to the given state (True='ON', False='OFF')"""
    global relayState
    if isSet == True:
        if relayState == False or relayState is None:
            pulsePin(RELAY_SET_PIN, 30, True)
            relayState = True
    else:
        if relayState == True or relayState is None:
            pulsePin(RELAY_RESET_PIN, 30, True)
            relayState = False

def digitToSegments(digit):
    """Maps decimal digits to 7-seg display"""
    numericFont = '\x3F\x06\x5B\x4F\x66\x6D\x7D\x07\x7F\x67'
    if digit < 0:
        return 0
    if digit > 9:
        return 0
    return ord(numericFont[digit])

def display2digits(value):
    """Display numeric decimal value on the 7-seg LED"""
    if value < 0:
        value = 0
    if value > 99:
        value = 99
    msd = value / 10
    mss = digitToSegments(msd)
    lsd = value % 10
    lss = digitToSegments(lsd)
    segmentValue = (mss << 8) + lss
    SetSegments(segmentValue)
    return

def checkRelay():
    """Check for Relay presence:  return True if present"""
    setPinDir(RELAY_SET_PIN, False)
    readPin(RELAY_SET_PIN)
    setPinPullup(RELAY_SET_PIN, True)
    return not readPin(RELAY_SET_PIN)

def isDemoBridge():
    """Detect the SN163 Bridge Demonstration Board based on Relay presence"""
    return not checkRelay()

def isDemoBoard():
    """Detect the Demonstration Boards based on LED control lines"""
    SER_CLK = GPIO_13  # pulled low on Demo boards
    SER_IN  = GPIO_14  # pulled low on Demo boards
    # DEMO_BUTTON_PIN    # pulled high on Demo boards
    setPinDir(SER_CLK, False)
    setPinPullup(SER_CLK, False)

    setPinDir(SER_IN, False)
    setPinPullup(SER_IN, False)

    setPinDir(DEMO_BUTTON_PIN, False)
    setPinPullup(DEMO_BUTTON_PIN, False)

    return (not readPin(SER_CLK)) and (not readPin(SER_IN)) and readPin(DEMO_BUTTON_PIN) and (not isSnapStick()) and (not isSS200())

def isSnapStick():
    """Detect the SNAPStick Boards based on LED pins"""
            
    if platform == "PAN4561_SE" or platform == "PAN4555_SE":
        #Let the pins settle out before looking
        spinCount = 0
        while spinCount < 3000:
            spinCount += 1
    
    # On the SNAPStick: GPIO 5 should be low (pwr LED), as are the other LED pins (active low)    
    retVal = False
    setPinDir(STICK_LED_GRN_PIN, False)
    setPinDir(STICK_LED_RED_PIN, False)
    if not readPin(GPIO_5):
        if readPin(STICK_LED_RED_PIN):
            if readPin(STICK_LED_GRN_PIN):
                retVal = True
    return retVal

def isSS200():
    """Detect the SNAP Stick 200 based on getInfo and platform"""
    return platform == 'SS200' and getInfo(0) == 4 and getInfo(1) == 0 and getInfo(2) == 5 and getInfo(3) == 6
    
def isRF30x():
    """Detect the RF30x based on getInfo and platform"""
    return (platform == 'RF300' or platform == 'RF301')

def initDeviceType():
    """Set default device types for Evaluation boards"""
    global deviceType
    
    if isRF30x():
        initDeviceTypeFromNV()
    else:
        if isDemoBoard():
            if isDemoBridge():
                deviceType = "Bridge"
            else:
                deviceType = "Photo"
        elif isSS200():
            deviceType = 'SS200'
        elif isSnapStick():
            deviceType = "Stick"
        else:
            deviceType = "Buzz"

        # Store detected device type string to reserved system NV param
        saveNvParam(NV_DEVICE_TYPE_ID, deviceType)

def initDeviceTypeFromNV():
    """Detect and initialize Evaluation Boards to default configuration"""
    global deviceType
    # The device type will be hard-coded for each node
    deviceType = loadNvParam(NV_DEVICE_TYPE_ID)
    # Store detected device type string to reserved system NV param
    saveNvParam(NV_DEVICE_TYPE_ID, deviceType)
         
    

