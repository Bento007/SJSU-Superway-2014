""" Author: Trent Smith
    Date: 3/19/14
    this is a helper file for pod.py. It contains the global
    values, and RPC functions.
    Distance between ticks should be uniform.
"""
#**********Globals**********************************************
#-----Network addresses and groups
NET_ID = "\x01\x90"
NET_GROUP_ALL = "\xFF\xFF"

#-----Last command and parameters
cmd = 0                     # Tracks the current command
addr= 0                     # Current address tallking too 
data = 0                    # Tracks the data returned from a RPC

""" These values are updated from the pod to send to neighbors
    these values are updated every 100ms
    """
lTime = 0 #last time
lSped = 0 #last speed
lLoca = 0 #last location
lTick = 0 #last ticks till merge. Tick needs a time stamp connected to it.
lStat = 0 #last status

yields =0 #how many times the pods has yielded in a row. used in for merge priority
 
#cTime = 0 #current Time
#cSped = 0 #current speed
#cLoca = 0 #current location
#cTick = 0 #current ticks till merge
#cStat = 0 #current status

cDest = 0 #current Destination
nDest = 0 #next Destination #don't know what to do with it yet.

ETM = 0   #Estimated time till merge in seconds. Used in merging

def COM(cmd,data):              #Status: WIP
    """Every 10ms handle RPC messages
    """
                    #Desc        CMD    Target Address
    if cmd == 1:    #get update  0x01 
        rpc(addr,getLocals,addr, lTime, lSped, lLoca, lStat)
    elif cmd == 2:  #stop        0x02    addr
        rpc(addr, stopRPC)
    elif cmd == 3:  #slow        0x03    addr    
        rpc(addr, slowRPC)
    elif cmd == 4:  #speed       0x04    addr
        pass
    elif cmd == 5:  #merge       0x05
        merge()
    elif cmd == 10: #help        0x10    multicasted
        emergency()
    elif cmd ==255: #local       0xFF
        pass
        
#-----EMERGENCY FUNCTIONS
def emergency(): #status Done, not tested
    '''A pod call this if it is having an emergency.'''
    mcastRpc(NET_GROUP_ALL, 5, emergencyRPC, localAddr(), lLoca, lStat)
#-----EMERGENCY RESOLUTION FUNCITONS
def emergencyResolved(loc): #Status: Done, not tested
    '''called by the master once the pod issueing a help is handled.'''
    mcastRpc(NET_GROUP_ALL, 5, emergencyResolvedRPC, loc)
#**********RPC FUNCTIONS*******************************
def emergencyResolvedRPC(loc):  #status: Done, not tested
    print "unblock", loc #sends an unblocks signal to MPU a location blocked by a help CMD.
def emergencyRPC(addr, loc, stats): #status: Done, not tested
    '''The address, and location of a pod with an emergency is rebroadcasted till it 
        reaches master and is handled. The location of emergency is avoided by the 
        pod.'''
    print "block: ", loc #send location to MPU to prevent pod from routing that way.
def merge():    #Status: WIP 
    """ This determines the merge order"""
    mcastRpc(NET_ID, 1, mergeRPC, ETM, lLoca)
def mergeRPC(remoteETM,remoteLoca): #Status: WIP
    """This will be called remotely to 
        determine merge order
    """
    global ETM
    
    if lLoca != remoteLoca: #if you will be merging
        return
    else:
        temp = ETM - remoteETM #do you need to worry?
        if temp > 4  or temp < -4 : # no worries
            return
        elif ( ETM < remoteETM ): #you're in front of sender you determine order
            temp = 4 + ETM #temp should be a negative number
            rpc( rpcSourceAddr, slowRPC, temp ) #tell other pod to slow
        elif( ETM == remoteETM ):   #whomever is to the right goes first
            action = rpc( rpcSourceAddr, yieldRPC, yields ) #tell other pod to slow
            if not action: #yield fails
                ETM = remoteETM + 4
            
def stopRPC(time):  #Status: WIP
    """tells a pod to stop ASAP"""
    print "stop"
def slow(newETM):  #Status: Done, not tested
    """tells a pod to slow down a time or speed"""
    global ETM
    ETM = newETM
    print "slow", ETM #tell pod to slow
def slowRPC(newETM):  #Status: Done, not tested
    """tells a pod to slow down a time or speed"""
    global ETM
    ETM = ETM + newETM
    print "slow", ETM
def updateRPC(loc,weight):  #status: Done, not tested
    """Updates the track on the MPUs memory"""
    print "loc:",loc,",weight:",weight
def yieldRPC(remoteYields): #Status: WIP
    """asks the pod to yield to sender
        if true the pods yields
        else the pods tells the other to yield
    """
    global yields, ETM
    if remoteYields > yields:
        yields = yields + 1 #increase the number of times yielded
        ETM = ETM + 4
        return 1
    else:
        return 0
#**********MISC FUNCTION*******************************
#-----GETTER FUNCTIONS ( use for sending to MPU )
def getCMD():   #status: Done, Tested
    """Used for testing CMD"""
    print cmd
def getData():  #status: Done, Tested
    print data  #prints global data value
def getDest():  #status: Done, Tested
    """prints the current destination"""
    print cDest
def getGlobals():  #status: Done, Tested
    """Prints out all of the globals values"""
    print "\ncmd: " , cmd
    print "\naddr: ", addr
    print "\ndata: ", data
    print "\nlTime: ", lTime
    print "\nlSped: ", lSped
    print "\nlLoca: ", lLoca
    print "\nlTick: ", lTick
    print "\nlStat: ", lStat
    print "cDest: ", cDest
    print "nDest: ", nDest
    print "yields: ", yields
def getLocals(addr, time, speed, loca, stat): #status: Done, not tested
    """Returns the current stats of the Local to Caller through RPC"""
    print "new",addr,",",lTime,",",lSped,",",lLoca,",",lStat
#-----SETTER FUNCTIONS(for manually setting values from Portal)
def setCMD(x):  #status: Done, Tested
    """Used for testing CMD"""
    global cmd
    cmd = x

def setDest(destIn):    #status: Done, Tested
    global cDest
    cDest = destIn
def setNetGroup(addr):  #status: Done, not tested
    """use to change the networks group
    """
    saveNvParam(5,addr) #determine addr?
    saveNvParam(6,addr) #determine addr?
    reboot()
def setLocals(cTime, cLoca, cTick, cStat, cSped): #Status: Done, not tested
    """Updates the local values from pod to share
        with other pods."""
    global lTime, lLoca, lSped, lStat, lTick, yields
    #global cTime, cLoca, cSped, cStat, cTick
    
    if lTime != cTime: #if the last update is the same as current don't change anything
        lTime = cTime
        lSped = cSped
        lStat = cStat
        lTick = cTick
        if lLoca != cLoca:
            #change networks forwarding and processing and reboot
            yields = 0
            setNetGroup(addr)
            reboot()   

#-----FUNCTIONS FOR DEBUGGING AND TESTING
def putChar(char):  #status: Done, tested
    print char
def manualUp(time,sped, loca, stat):    #status: Done, Tested

    global cTime, cSped, cLoca, cTick, cStat

    cTime = time
    cSped = sped
    cLoca = loca
    cStat = stat
def hello():    #status: WIP
    """Every second broad cast hello share your address and basic info
        with other nodes so they can find you.
    """
    pass