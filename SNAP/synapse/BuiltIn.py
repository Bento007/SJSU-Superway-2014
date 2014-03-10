# (c) Copyright 2007-2012, Synapse
"""Built in SNAPpy functions"""


__cfuncs__ = {}


def _func_num(n):
    def add_func(f):
        global __cfuncs__
        __cfuncs__[f.func_name] = n
        return f
    return add_func


@_func_num(0)
def peek(addr):
    """Read a memory location"""
    pass

@_func_num(1)
def poke(addr, byteVal):
    """Write a memory location"""
    pass

@_func_num(2)
def rpc(dstAddr, remoteFnObj, args):
    """Remote Procedure Call (unicast)"""
    pass

@_func_num(3)
def mcastRpc(dstGroups, ttl, remoteFnObj, args):
    """Remote Procedure Call (multicast)"""
    pass

@_func_num(4)
def getMs():
    """Elapsed milliseconds since startup (16bit)"""
    pass

@_func_num(5)
def setPinDir(pin, isOutput):
    """Set direction for parallel I/O pin"""
    pass

@_func_num(6)
def setPinPullup(pin, isEnabled):
    """Enable pullup resistor (~25k) for Input pin"""
    pass

@_func_num(7)
def readPin(pin):
    """Read current level of pin"""
    pass

@_func_num(8)
def setPinSlew(pin, isRateControl):
    """Enable slew rate-control (~30ns) for Output pin"""

@_func_num(9)
def writePin(pin, isHigh):
    """Set Output pin level"""
    pass

@_func_num(10)
def monitorPin(pin, isMonitored):
    """Enable GPIN events on Input pin"""
    pass

@_func_num(11)
def pulsePin(pin, msWidth, isPositive):
    """Apply pulse to Output pin"""
    pass

@_func_num(12)
def readAdc(channel):
    """Sample ADC on specified input channel"""
    pass

@_func_num(13)
def localAddr():
    """Local network address"""
    pass

@_func_num(14)
def initUart(uartNum, baudRate):
    """Enable UART at specified rate (zero to disable)"""
    pass

@_func_num(15)
def flowControl(uartNum, isEnabled):
    """Enable RTS/CTS flow control"""
    pass

@_func_num(16)
def crossConnect(dataSrc1, dataSrc2):
    """Cross-connect SNAP data-sources"""
    pass

@_func_num(17)
def uniConnect(dst, src):
    """Connect src->dst SNAP data-sources"""
    pass

@_func_num(18)
def ucastSerial(dstAddr):
    """Set Serial transparent mode to unicast"""
    pass

@_func_num(19)
def mcastSerial(dstGroups, ttl):
    """Set Serial transparent mode to multicast"""
    pass

@_func_num(20)
def getLq():
    """Link Quality in (-) dBm"""
    pass

@_func_num(21)
def loadNvParam(id):
    """Load indexed parameter from NV storage"""
    pass

@_func_num(22)
def saveNvParam(id, obj):
    """Save object to indexed NV storage location"""
    pass

@_func_num(23)
def setChannel(channel):
    """Set radio channel (0-15)"""
    pass

@_func_num(24)
def getChannel():
    """Get radio channel (0-15)"""
    pass

@_func_num(25)
def reboot():
    """Reboot the device"""
    pass

@_func_num(26)
def ord(str):
    """Return the integer ordinal (ASCII) of a 1-character string"""
    pass

@_func_num(27)
def resetVm():
    """Reset the embedded virtual machine (prep for upload)"""
    pass

@_func_num(28)
def eraseImage():
    """Erase user-application FLASH memory"""
    pass

@_func_num(29)
def writeChunk(ofs, str):
    """Write string to user-application FLASH memory"""
    pass

@_func_num(30)
def initVm():
    """Initialize embedded virtual machine"""
    pass

@_func_num(31)
def setSegments(segments):
    """Set eval-board LED segments (clockwise bitmask)"""
    pass

@_func_num(32)
def rpcSourceAddr():
    """Originating address of current RPC context (None if called outside RPC)"""
    pass

@_func_num(33)
def sleep(mode,ticks):
    """Enter sleep mode for specified number of ticks\n(See SNAP Reference Manual for platform details)"""
    pass

@_func_num(34)
def rx(isEnabled):
    """Enable radio receiver"""
    pass

@_func_num(35)
def len(sequence):
    """Returns the length of the sequence"""
    pass

@_func_num(36)
def setNetId(netId):
    """Set Network ID (1-65535)"""
    pass

@_func_num(37)
def getNetId():
    """Current Network ID"""
    pass

@_func_num(38)
def bist():
    """Built-in self test"""
    pass

@_func_num(39)
def imageName():
    """Name of current Snappy image"""
    pass

@_func_num(40)
def errno():
    """Read and reset last error code"""
    pass

@_func_num(41)
def call(rawOpcodes, *args):
    """Call embedded C code"""
    pass

@_func_num(42)
def vmStat(statusCode, args):
    """Solicit a tellVmStat for system parameters"""
    pass

@_func_num(43)
def random():
    """Returns a pseudo-random number (0-4095)"""
    pass

@_func_num(44)
def stdinMode(mode, echo):
    """Setup STDIN for Line or Character based input"""
    pass

@_func_num(45)
def int(obj):
    """Returns the integer representation of the object"""
    pass

@_func_num(46)
def setRate(rateNum):
    """Adjusts the sampling rate used by monitorPin() (0-3 = Off, 100ms, 10ms, 1ms)"""
    pass

@_func_num(47)
def chr(number):
    """Returns the single-character-string representation of the number"""
    pass

@_func_num(48)
def cbusRd(numToRead):
    """Reads a string of bytes from the CBUS interface"""
    pass

@_func_num(49)
def cbusWr(byteStr):
    """Writes a string of bytes to the CBUS interface"""
    pass

@_func_num(50)
def getEnergy():
    """Energy Detected on current channel (integer) in (-) dBm"""
    pass

@_func_num(51)
def scanEnergy():
    """Energy Detected on all channels (string) in (-) dBm"""
    pass

@_func_num(52)
def txPwr(power):
    """Set TX power level (0-17)"""
    pass

@_func_num(53)
def i2cInit(pullups):
    """Setup for I2C, with internal (True) or external (False) pullup resistors"""
    pass

@_func_num(54)
def i2cWrite(byteStr,retries,ignoreFirstAck):
    """Send data out I2C - returns bytes actually sent"""
    pass

@_func_num(55)
def i2cRead(byteStr,numToRead,retries,ignoreFirstAck):
    """Send data out I2C then read response - returns response string"""
    pass

@_func_num(56)
def getI2cResult():
    """Returns status code from most recent I2C operation"""
    pass

@_func_num(57)
def spiInit(cpol,cpha,isMsbFirst,isFourWire):
    """Setup for SPI, with specified Clock Polarity, Clock Phase, Bit Order, and Physical Interface"""
    pass

@_func_num(58)
def spiWrite(byteStr,bitsInLastByte=8):
    """Send data out SPI - bitsInLastByte defaults to 8, can be less"""
    pass

@_func_num(59)
def spiRead(byteCount,bitsInLastByte=8):
    """Receive data in from SPI - returns response string (3 wire only)"""
    pass

@_func_num(60)
def spiXfer(byteStr,bitsInLastByte=8):
    """Bidirectional SPI transfer - returns response string (4 wire only)"""
    pass

@_func_num(61)
def getInfo(whichInfo):
    """
    Get details about the platform and operating environment

    whichInfo:
    0 = Vendor
    1 = Radio
    2 = CPU
    3 = Platform/Broad Firmware Category
    4 = Build
    5 = Version (Major)
    6 = Version (Minor)
    7 = Version (Build)
    8 = Encryption
    9 = RPC Packet Buffer Reference
    10 = Is Multicast
    11 = Remaining TTL
    12 = Remaining Tiny Strings
    13 = Remaining Medium Strings
    14 = Route Table Size
    15 = Routes in Route Table
    """
    pass

@_func_num(62)
def callback(callback, remoteFnObj, args):
    """Like rpc() but also invokes specified callback routine with the result of the remoteFnObj routine"""
    pass

@_func_num(63)
def getStat(whichStat):
    """
    Get details about how busy the node has been with processing packets

    whichStat:
    0) = Null Transmit Buffers
    1 = UART0 Receive Buffers
    2 = UART0 Transmit Buffers
    3 = UART1 Receive Buffers
    4 = UART1 Transmit Buffers
    5 = Transparent Receive Buffers
    6 = Transparent Transmit Buffers
    7 = Packet Serial Receive Buffers
    8 = Packet Serial Transmit Buffers
    9 = Radio Receive Buffers
    10 = Radio Transmit Buffers
    11 = Radio Forwarded Unicasts
    12 = Packet Serial Forwarded Unicasts
    13 = Radio Forwarded Multicasts
    14 = Packet Serial Forwarded Multicasts
    """
    pass

@_func_num(64)
def str(obj):
    """Returns a string representation of obj"""
    pass

@_func_num(65)
def lcdPlot(args):
    """Interface to eval board LCD panel"""
    pass

@_func_num(66)
def callout(dstAddr, callout, remoteFnObj, args):
    """Like callback() but final result is reported to the specified dstAddr"""
    pass

@_func_num(67)
def peekRadio(addr):
    """Returns the specified internal radio address"""
    pass

@_func_num(68)
def pokeRadio(addr, byteValue):
    """Writes the specified value to the specified radio address"""
    pass

@_func_num(69)
def setRadioRate(dataRateEnum):
    """Set data rate of radio"""
    pass

@_func_num(70)
def updateDmxBuf(args):
    """Update the DMX buffer"""
    pass

@_func_num(71)
def getDmxBuf(args):
    """Update the DMX buffer"""
    pass
