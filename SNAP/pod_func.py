""" Author: Trent Smith
    Date: 2/23/14
    this is a helper file for pod.py. It contains the global
    values, and RPC functions.
"""
cmd = "0"                      # Tracks the current command
addr= "0"                      # Current address tallking too 
data = "0"                     # Tracks the data returned from a RPC

""" These values are updated from the pod to send to neighbors
        these values are updated every 10ms
"""
lTime = "0" #last time
lSped = "0" #last speed
lLoca = "0" #last location
lStat = "0" #last status

cTime = "0" #current Time
cSped = "0" #current speed
cLoca = "0" #current location
cStat = "0" #current status

def stop():         #status: incomplete
    print "stop"
def slow():         #status: incomplete
    print "slow"
def address():      #status: Done, tested
    print localAddr()
def putChar(char):  #status: Done, tested
    print char
def getUpdate(addr, time, speed, loca, stat): #status: Done, not tested
    """Returns the current stats of the Local to Caller through RPC
    """
    print "new,",addr,",",time,",",speed,",",loca,",",stat
    pass
def setCMD(x):      #status: Done, not tested
    """Used for testing CMD"""
    global cmd
    cmd = x
def getCMD():       #status: Done, not tested
    """Used for testing CMD"""
    global cmd
    print cmd
def hello():        #status: incomplete
    """Every second broad cast hello
    share your address and basic info
    with other nodes so they can find 
    you.
    """
    pass
def help():         #status: incomplete
    pass
def manualUp(time,sped, loca, stat): #status: Done, not tested
    global cTime
    global cSped
    global cLoca
    global cStat
    
    cTime = time
    cSped = sped
    cLoca = loca
    cStat = stat