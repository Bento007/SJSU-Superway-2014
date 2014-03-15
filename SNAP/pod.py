"""
    Author: Trent Smith  Date: 2/17/2014
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
    initUart(DS_UART0,9600, 8, 'N',1)# set baudrate
    stdinMode(0, False)
    flowControl(1, False, True) # <= set flow control to True or False as needed
    crossConnect(DS_UART1, DS_STDIO)#connect uart1 to STDIO
@setHook(HOOK_STDIN)
def stdinEvent(buf):    #Status: Done, not tested
    """Receive handler for character input from SJONE to SNAP.
       The parameter 'buf' will contain one or more 
       received characters."""
 
    n = len(buf)
    
    global cmd, addr, data
    global cTime, cLoca, cStat, cSped
    
    setData(buf)
    
    if n == 1:      # mcastRPC command
        cmd = buf[0]
    elif n <= 4:    # RPC command, return data from wireless
        cmd = buf[0]
        addr = buf[1:3]
    else:            # Data is being transfered     
        cTime = buf[0:5] #HH:MM:SS
        cLoca = buf[6:8] #NN:P
        cStat = buf[9]   #current status
        cSped = buf[10:] #current speed

    #print "got:", buf,", length:", size
    #is it a status update?
        #update speed, status, position
    #is it a message to be sent
        #what kind of message
            #call needed RPC
@setHook(HOOK_STDOUT)
def printed():          #Status: not done, not tested
    pass
@setHook(HOOK_1S)
def hook_1s():          #Status: Done, not tested
    """Every second broad cast hello
    share your address and basic info
    with other nodes so they can find 
    you.
    """
    printData();
    pass
#@setHook(HOOK_10MS)
def COM():              #Status: IP
    """Every 10ms handle RPC messages
    """
    global lTime, lLoca, lSped, lStat
    global cmd
    global addr
    global data
                    #Desc        CMD    Target Address
    if cmd == 1:    #get update  0x01 
        rpc(addr,getUpdate,addr, lTime, lSped, lLoca, lStat)
    elif cmd == 2:  #stop        0x02    addr
        rpc(addr, stop)
    elif cmd == 3:  #slow        0x03    addr    
        rpc(addr, slow)
    elif cmd == 4:  #speed       0x04    addr 
        pass
    elif cmd == 10: #help        0x10    multicasted
        mcastRpc(1,2,help)
    elif cmd ==255: #local       0xFF
        pass
#@setHook(HOOK_100MS)
def locUpdate():        #Status: Done, not tested
    """Updates the local values from pod to share
        with other pods."""
    global lTime, lLoca, lSped, lStat
    global cTime, cLoca, cSped, cStat
    
    if lTime != cTime: #if the last update is the same as current don't change anything
        lTime = cTime
        lLoca = cLoca
        lSped = cSped
        lStat = cStat
def setData(stuff):
    global data
    data = stuff[:]
def printData():
    print data  #prints global data value