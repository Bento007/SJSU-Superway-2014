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
PORTAL_ADDR = "\x00\x00\x01"
active = 0  #must be set before the pod can get directions

""" These values are updated from the pod to send to neighbors
    these values are updated every 50ms
    """
cTime = 0 #last time
cSped = 0 #last speed
cLoca = 0 #last location
cStat = 0 #last status
cTick = 0 #last ticks till merge. Tick needs a time stamp connected to it.

yields =0 #how many times the pods has yielded in a row. used in for merge priority

startup =1     #// initializes everything
ready   =2     #// waiting for direction
error   =3     #// error detected or initialize failed
roam    =4     #// no directions received driving around track
pickup  =5     #// directions received going to pick up location
load    =6     #// at stations loading
dropoff =7     #// in route to drop off
unload  =8     #// at destination and unloading passengers
emergency = 9  #// an emergency has occured

cDest = 0 #current Destination
nDest = 0 #next Destination #don't know what to do with it yet.

cETM = 0   #Estimated time till merge in seconds. Used in merging
rETM = 0  #estimnated time till merge in seconds of remote pod. Used in merge
# def COM(cmd,data):              #Status: WIP
#     """handle commands from StdInput
#     """
#     n = len(data)
#     param = 0
#     i = 0
#     j = 0
#     while(i < n):
#         c = data[i]
#         i += 1
#         if c == ',':
#             if param == 0:
#                 temp0 = data[j:i-1]
#             elif param == 1:
#                 temp1 = data[j:i-1]
#             elif param == 2:
#                 temp2 = data[j:i-1]
#             elif param == 3:
#                 temp3 = data[j:i-1]
#             elif param == 4:
#                 temp4 = data[j:i-1]
#             elif param == 5:
#                 temp5 = data[j:i-1]
#             j=i
#             param +=1
#                       #Desc        CMD    Target Address
#     if cmd == 'U':    #get update  U
#         setLocals(temp0, temp1, temp2, temp3,temp4, temp5)
#     elif cmd == 'M':  #merge       M
#         merge()
#     elif cmd == 'E':  #help        E    multicasted
#         setStatus(temp0,temp1)
#         emergency()
#     elif cmd == 'D':
#         getDest()
#     elif cmd == 'X':
#         rpc(PORTAL_ADDR,'callback','getTime','getTimeRPC')
#     elif cmd =='T':
#         test()
#     elif cmd =='P':
#         print data
#     elif cmd == 2:  #stop        0x02    addr
#         rpc(addr, stopRPC)
#     elif cmd == 3:  #slow        0x03    addr    
#         rpc(addr, slowRPC)
#     elif cmd == 4:  #speed       0x04    addr
#         pass
#     elif cmd ==255:   #local       0xFF
#         pass
        
#-----EMERGENCY FUNCTIONS
def emergency(): #status Done, not tested
    '''A pod call this if it is having an emergency.'''
    mcastRpc(NET_GROUP_ALL, 5, 'emergencyRPC', localAddr(), cLoca, cStat)
#-----EMERGENCY RESOLUTION FUNCITONS
def emergencyResolved(loc,weight): #Status: Done, not tested
    '''called by the master once the pod issueing a help is handled.'''
    mcastRpc(NET_GROUP_ALL, 5, 'emergencyResolvedRPC', loc,weight)
#**********RPC FUNCTIONS*******************************
def emergencyResolvedRPC(loc,weight):  #status: Done, not tested
    updateRPC(loc,weight)#sends an unblocks signal to MPU a location blocked by a help CMD.
def emergencyRPC(addr, loc, stats): #status: Done, not tested
    '''The address, and location of a pod with an emergency is rebroadcasted till it 
        reaches master and is handled. The location of emergency is avoided by the 
        pod.'''
    weight = -999
    updateRPC(loc,weight)#send location to MPU to prevent pod from routing that way.
def merge():    #Status: WIP 
    """ This determines the merge order"""
    mcastRpc(NET_ID, 1, 'mergeRPC', cETM, cLoca)
def mergeRPC(remoteETM,remoteLoca): #Status: WIP
    """This will be called remotely to 
        determine merge order
    """
    global cETM, rETM
    rETM=remoteETM
    if cLoca != remoteLoca: #if you will be merging
        return
    else:
        temp = cETM - remoteETM #do you need to worry?
        if temp > 4  or temp < -4 : # no worries
            return
        elif ( cETM < remoteETM ): #you're in front of sender you determine order
            temp = 4 + cETM #temp should be a negative number
            rpc( rpcSourceAddr, 'slowRPC', temp ) #tell other pod to slow
        elif( cETM == remoteETM ):   #whomever is to the right goes first
            #action = rpc( rpcSourceAddr,'callback', 'yieldResults','yieldRPC', yields ) #tell other pod to slow
            rpc( rpcSourceAddr,'callback', 'yieldResults','yieldRPC', yields ) #tell other pod to slow
            #if not action: #yield fails
            #    ETM = remoteETM + 4

def yieldResults( x ):
    """Used to get the return value of the yeildRPC call"""
    global cETM
    if x == 0:
        cETM = rETM + 4
        getETM()
def register():
    if not active:
        rpc( PORTAL_ADDR, 'registerRPC', cLoca)
def registerRPC():
    pass
# def stopRPC(time):  #Status: WIP
#     """tells a pod to stop ASAP"""
#     print "H "
def slow(newETM):  #Status: Done, not tested
    """tells a pod to slow down a time or speed"""
    global cETM
    cETM = newETM
    print "M", cETM #tell pod to slow
def slowRPC(newETM):  #Status: Done, not tested
    """tells a pod to slow down a time or speed"""
    global cETM
    cETM = cETM + newETM
    print "M", cETM
def updateRPC(loc,weight):  #status: Done, not tested
    """Updates the track on the MPUs memory"""
    print "U ",loc,weight
def yieldRPC(remoteYields): #Status: WIP
    """asks the pod to yield to sender
        if true the pods yields
        else the pods tells the other to yield
    """
    global yields, cETM
    if remoteYields > yields:
        yields = yields + 1 #increase the number of times yielded
        cETM = cETM + 4
        getETM()
        return 1
    else:
        return 0
#**********MISC FUNCTION*******************************
#-----GETTER FUNCTIONS ( use for sending to MPU )
# def getCMD():   #status: Done, Tested
#     """Used for testing CMD"""
#     print cmd
# def getData():  #status: Done, Tested
#     print data  #prints global data value
def getDest():  #status: Done, Tested
    """prints the current destination"""
    print "D",cDest
def getETM():
    print "M",cETM
def getGlobals():  #status: Done, Tested
    """Prints out all of the globals values"""
#     print "\ncmd: " , cmd
#     print "\naddr: ", addr
#     print "\ndata: ", data
    print "\ncTime: ", cTime
    print "\nlSped: ", cSped
    print "\nlLoca: ", cLoca
#     print "\nlTick: ", lTick
    print "\nlStat: ", cStat
    print "cDest: ", cDest
    print "nDest: ", nDest
    print "yields: ", yields 
#
#

#-----SETTER FUNCTIONS(for manually setting values from Portal)
# def setCMD(x):  #status: Done, Tested
#     """Used for testing CMD"""
#     global cmd
#     cmd = x
def setActive():
    global active#, cDest
    #cDest = 8
    active = 1
def getTimeRPC():
    #rpc(PORTAL_ADDR'callback','getTime','getTimeRPC')
    rpc(PORTAL_ADDR, 'getTimeRPC')
#def getTime(xTime):
#    print xTime[0],' ',xTime[1],' ',xTime[2],' ',xTime[3],' ',xTime[4],' ',xTime[5],' ',xTime[6],' ',xTime[7]
def getTime(yyyy,mm,dd,hh,min,ss,dow,doy):  #TODO finish
    print "X ",yyyy,' ',mm,' ',dd,' ',hh,' ',min,' ',ss,' ',dow,' ',doy
# def setDest(destIn):    #status: Done, Tested
#     global cDest
#     if cLoca == cDest: # if you're at your destination, then you can receive a new destination
#         cDest = destIn
#         getDest()
    
def setRoute(PU, DO):    #status: Done, Tested
    global cDest, nDest
    #if cLoca == cDest & nDest == 0 & cStat: # if you're at your destination, then you can receive a new destination
    if readyForNewRoute():
        cDest = PU
        nDest = DO
    getDest()
def setETM(x,y):
    global cETM, cTime
    cETM = y
    cTime = x
def setNetGroup(addr):  #status: Done, not tested
    """use to change the networks group
        it can by anything between 0x01 - 0xEF
    """
    saveNvParam(5,addr) #determine addr?
    saveNvParam(6,addr) #determine addr?
    reboot()
# def setTicks(ticks):
#     global lTicks
#     lTicks = ticks;
def setStatus(x,y):
    global cStat, cLoca
    cStat = x
    cLoca = y
def setLocals(Time, Loca, Dest, Stat, Sped,Tick, ETM): #Status: Done, not tested
    """Updates the local values from pod to share
        with other pods."""
    global cDest, nDest, cTime, cLoca, cSped, cStat,cTick, yields, cETM
    
    if cTime != Time: #if the last update is the same as current don't change anything
        cTime = Time
        cSped = Sped
        cStat = Stat
        cTick = Tick
        cETM = ETM
        if cDest != Dest:
            getDest()
        #if cLoca == cDest & cStat == ready: #check if at location
        if readyForNextDest(): #check if at location
            cDest = nDest
            nDest = 0  #if next location = 0, the pod can recieve a new "next destination"
            getDest()
        elif cLoca != Loca:
            yields = 0 

#-----FUNCTIONS FOR DEBUGGING AND TESTING
def putChar(char):  #status: Done, tested
    print char
def manualUp(time,sped, loca, stat):    #status: Done, Tested
    global cTime, cSped, cLoca, cStat
#     global cTime, cSped, cLoca, cStat, cTick

    cTime = time
    cSped = sped
    cLoca = loca
    cStat = stat
def test():
    
    d = 345
    c= 865
    b = 'A'
    a = "Hello"
    
    print a,' ',b,' ',c,' ',d
# def parseLocal(input): # Status: Done, not tested
#     n = len(input)
#     param = 0
#     i = 0
#     j = 0
#     while(i < n):
#         c = input[i]
#         i += 1
#         if c == ',':
#             if param ==0:
#                 cTime = input[j:i-1]
#             elif param ==1:
#                 cLoca = input[j:i-1]
#             elif param ==2:
#                 cStat = input[j:i-1]
#             elif param ==3:
#                 cSped = input[j:i-1]
#             j=i
#             param +=1
#     setLocals(cTime, cLoca,cStat, cSped)      
def printStatus():#sends the stats of the pod to the master.
    """ sends a update to the master: 
        -if ready and no destination the master will provide a 
        new pick up and drop off location as needed.
        -The master can react to the pod based off what it recieves
        """
    rpc(PORTAL_ADDR,'podStatus',cTime,cStat,cLoca,cSped,cTick,cDest)
    
def readyForNextDest():
    """ checks if the pod is ready to recieve a new destination"""
    if cLoca == cDest & cStat == dropoff:
        return 1
    else:
        return 0
def readyForNewRoute():
    """Check id the pod is reday to recieve a new destination
    from the master"""
    if cLoca == cDest & cStat == ready & nDest == 0:
        return 1
    else:
        return 0 