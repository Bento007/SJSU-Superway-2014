"""
    Author: Trent Smith  
    Date: 3/19/2014
    The following code is used to control the communication
    between other pods on the Spartan Supeway PRT system.
    This code is to be uploaded to the RF266PC1 device
    attached to the SJONEv2 board.
    
    Messages sent to SNAP must end with \n or \0
"""

#imported libraries
from synapse.switchboard import *   # crossConnect()
from pod_func import *              #includes all the pods comands.

#global variables
PORTAL_ADDRESS = "\x00\x00\x01" # For immediate debug/stdout
NET_ID = "\x90\x01"
NET_GROUP_ALL = "\xFF\xFF"
"""messages need to be recieved from wireless and buffered. 
    The first byte the type and piority
    needed variables 
        Current Position
        Speed
        Status
        Name
    Optional or Future Variables
        Destination
        RouteNum
    How data will be sent
        send data from UART <-> SNAP 
        send data from SNAP <-> SNAP
        """
        
@setHook(HOOK_STARTUP)
def startupEvent():     #Status: Done, Tested
    """System startup code, invoked automatically 
    (do not call this manually)"""
    saveNvParam(3,NET_ID) #the default NET_ID
    initUart(DS_UART0,19200, 8, 'N',1)# set baudrate
    stdinMode(0, False)
    flowControl(1, False, True) # <= set flow control to True or False as needed
    crossConnect(DS_UART1, DS_STDIO)#connect uart1 to STDIO
    crossConnect(DS_UART1, DS_TRANSPARENT)#connect uart1 to wireless
@setHook(HOOK_STDIN)
def stdinEvent(buf):    #Status: Done, not tested
    """Receive handler for character input from SJONE to SNAP.
       The parameter 'buf' will contain one or more 
       received characters."""
    COM(buf[0],buf[1:])

@setHook(HOOK_STDOUT)
def printed():          #Status: not done, not tested
    pass
#@setHook(HOOK_1S)
def hook_1s():          #Status: Done, not tested
    """Every second broad cast hello
    share your address and basic info
    with other nodes so they can find 
    you.
    """
    pass
#@setHook(HOOK_10MS)
def hook_10ms():
    pass
#@setHook(HOOK_100MS)
def hook_100MS():
    pass