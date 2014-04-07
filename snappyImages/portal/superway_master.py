""" Author: Trent Smith
    Date: 4/4/14
    This defines the main functions for the master
    of the SJSU Superway PRT system.
    
    The Master oversees the system as a whole. Main
    functions include: 
        -registering the pods as they come online
        -send pods the current time
        -send pods graph updates
        -send pods new destinations
        -update local graph to reflect realtime conditions
        -display system
"""
#**********Libraries********************************************
from snapconnect import snap
import time
#**********Globals**********************************************
#-----Network addresses and groups
NET_ID = "\x01\x90"
NET_GROUP_ALL = "\xFF\xFF"

pods = 0    #number of registered pods
requests = 0#number of current requests
graph = 0   #handler for the file.

def COM(cmd,data):              #Status: WIP
    """handle commands from StdInput
    """
                    #Desc        CMD    Target Address
    if cmd == 'U':    #get update  U
        parseLocal(data)
    if cmd == 'T':
          setETM(data)
#         rpc(addr,getLocals,addr, lTime, lSped, lLoca, lStat)
    elif cmd == 'M':  #merge       M
        setETM(data)
        merge()
    elif cmd == 'E':  #help        E    multicasted
        setStatus(data)
        emergency()
    elif cmd == 'D':
        getDest()

        
#-----EMERGENCY FUNCTIONS
def emergency(): #status Done, not tested
    '''A pod call this if it is having an emergency.'''
    pass
#-----EMERGENCY RESOLUTION FUNCITONS
def emergencyResolved(loc): #Status: Done, not tested
    '''called by the master once the pod issueing a help is handled.'''
    pass
#**********RPC FUNCTIONS*******************************
def emergencyResolvedRPC(loc):  #status: Done, not tested
    pass
def emergencyRPC(addr, loc, stats): #status: Done, not tested
    '''The address, and location of a pod with an emergency is rebroadcasted till it 
        reaches master and is handled. The location of emergency is avoided by the 
        pod.'''
    pass
def merge():    #Status: WIP 
    """ This determines the merge order"""
    pass
def mergeRPC(remoteETM,remoteLoca): #Status: WIP
    """This will be called remotely to 
        determine merge order
    """
    pass
def stopRPC(time):  #Status: WIP
    """tells a pod to stop ASAP"""
    pass
def slow(newETM):  #Status: Done, not tested
    """tells a pod to slow down a time or speed"""
    pass
def slowRPC(newETM):  #Status: Done, not tested
    """tells a pod to slow down a time or speed"""
    pass
def updateRPC(loc,weight):  #status: Done, not tested
    """Updates the track on the MPUs memory"""
    pass
def yieldRPC(remoteYields): #Status: WIP
    """asks the pod to yield to sender
        if true the pods yields
        else the pods tells the other to yield
    """
    pass
#**********MISC FUNCTION*******************************
#-----GETTER FUNCTIONS ( use for sending to MPU )
def getDest():  #status: Done, Tested
    """prints the current destination"""
    pass
def getETM():
    pass
def getGlobals():  #status: Done, Tested
    pass
#-----SETTER FUNCTIONS(for manually setting values from Portal)
def setDest(destIn):    #status: Done, Tested
    global cDest
    cDest = destIn
def setETM(x):
    pass
def setNetGroup(addr):  #status: Done, not tested
    """use to change the networks group
        it can by anything between 0x01 - 0xEF
    """
    pass

def setStatus(x):
    pass
def setLocals(cTime, cLoca, cStat, cSped): #Status: Done, not tested
    """Updates the local values from pod to share
        with other pods."""
    pass
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
def hello():    #status: WIP
    """Every second broad cast hello share your address and basic info
        with other nodes so they can find you.
    """
    pass
     